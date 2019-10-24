/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

/* Includes --------------------------------------------------------------------------------------*/
#include "startup.h"
#include <util/delay.h>
#include <avr/io.h>

/* Presentation */
#include "rhythm_playback_controller.h"
#include "digital_tempo_knob.h"

/* Application */
#include "tempo_control_view.h"

/* Domain */
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"

/* Drivers */
#include "gpiopin.h"
#include "spi.h"
#include "timer0.h"
#include "timer1.h"
#include "rotary_encoder.h"
#include "segmentdisplay.h"
#include "charlieplex_matrix.h"

/* Infrastructure */
#include "interrupts.h"


/* Instantiations --------------------------------------------------------------------------------*/
/* Pattern edit interface (buttons + LEDs) */
static PinStatePair fivePinLut[20] =
{
	{1,0},{0,1},{2,1},{1,2},{2,0},{0,2},{3,2},{2,3},{3,1},{1,3},
	{3,0},{0,3},{4,3},{3,4},{4,2},{2,4},{4,1},{1,4},{4,0},{0,4}
};
static GpioPin ledPins[5] = {GpioPin(Pin1, PortC), GpioPin(Pin2, PortC), GpioPin(Pin3, PortC),
	GpioPin(Pin4, PortC), GpioPin(Pin5, PortC)};
static CharlieplexMatrix<GpioPin> ledMatrix = CharlieplexMatrix<GpioPin>(5, ledPins, fivePinLut);
/* Rotary encoder (input) */
static GpioPin encoderPinA = GpioPin(Pin2, PortD, DigitalInput); // triggers IRQ on voltage change
static GpioPin encoderPinB = GpioPin(Pin0, PortC, DigitalInput);
static RotaryEncoder<GpioPin> rotaryEncoder = RotaryEncoder<GpioPin>(encoderPinA, encoderPinB);
static DigitalTempoKnob<GpioPin> tempoKnob = DigitalTempoKnob<GpioPin>(rotaryEncoder);

/* Display for tempo (output) */
static Spi spi;
static Timer0 tim0; // timer0 interrupts used to cycle through digits in segment display
static SegmentDisplay74HC595 tempoDisplay = SegmentDisplay74HC595(spi);
static GpioPin dataLatchPin = GpioPin(Pin2, PortB, DigitalOutput); // slave select for display spi

/* Timing manager and LED (model) */
static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager tempoTimingManager = TempoTimingManager(tempoTimer);
static GpioPin rhythmLedPin = GpioPin(Pin3, PortD, DigitalOutput); // blinked by timing manager

/* UI for controlling tempo (view and controller) */
static RhythmPlaybackController playbackCtrl = RhythmPlaybackController(tempoTimer);
static TempoControlView tempoControlView = TempoControlView(playbackCtrl, tempoKnob, tempoDisplay);

/* Private function declarations -----------------------------------------------------------------*/
/* Spi */
static void setupSpi();
static void registerSpiInterrupt();

/* Tempo Timing */
static void setupTempoTimer();
static void registerTempoTimerInterrupt();
static void registerPlaybackHandlers();

/* Tempo display */
static void setupMillisecondTimer();
static void registerDisplayDriverInterrupt();

/* Tempo Knob */
static void setupExternalPinInterrupt();

// TODO: move this to Interrupts, currently used for rotary encoder
ISR(INT0_vect)
{
	_delay_us(100); // give time for signals to stabilize
	rotaryEncoder.handleEdge();
}

/* Public function definitions -------------------------------------------------------------------*/
TempoControlView& Startup::getTempoControlView()
{
	return tempoControlView;
}

TempoTimingManager& Startup::getTempoTimingManager()
{
	return tempoTimingManager;
}

CharlieplexMatrix<GpioPin>& Startup::getLedMatrix()
{
	return ledMatrix;
}

/* Configure all objects instantiated by the Startup module. NB: this function
 * MUST be called before using any object to guarantee correct behavior! */
void Startup::init()
{
	Interrupts::enableInterruptsGlobally();

	/* Spi */
	setupSpi();

	/* Tempo timng */
	setupTempoTimer();
	registerPlaybackHandlers();

	/* Tempo tempoDisplay */
	setupMillisecondTimer();
	registerDisplayDriverInterrupt();
	tempoDisplay.enableDecimalPoint(1);

	/* Rotary encoder interrupts */
	setupExternalPinInterrupt();
}

/* Private function definitions ------------------------------------------------------------------*/
/* Configure the Spi peripheral object with some default settings. */
void setupSpi()
{
	spi.setBitOrder(SpiBitOrder::MsbFirst);
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_128);
	registerSpiInterrupt();
}

/* The SPI peripheral supports transfer of an enire byte buffer. This function
 * attaches a lamba to the SpiTransferComplete IRQ that triggers sending the
 * next byte in the buffer, to create an asynchronous transfer from the
 * perspective of the spi client code. */
void registerSpiInterrupt()
{
	InterruptHandler spiTransferISR = []
	{
		if(!spi.txBufferIsEmpty())
		{
			spi.sendNextByteInBuffer();
		}
		else
		{
			dataLatchPin.set();
			dataLatchPin.clear();
		}
	};

	InterruptRequest spiIRQ = InterruptRequest::SpiTransferComplete;
	Interrupts::setHandlerForInterrupt(spiTransferISR, spiIRQ);
}

/* Setup the tempo timer object by enabling timer1, setting a default tempo and
 * starting the timer. */
void setupTempoTimer()
{
	registerTempoTimerInterrupt();
	tim1.enablePeriodicInterrupts();
	tempoTimer.setTempo(BeatsPerMinute(120));
	tempoTimer.start();
}

/* Attach a lambda to the timer1 compare match IRQ to connect hw timer with the
 * tempo timer object, so that the tempo can be kept by that object. */
void registerTempoTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

/* Attach all functions that should be called once every tempo step, i.e. be
 * called once every 16th note. */
void registerPlaybackHandlers()
{
	rhythmLedPin.set(); // debugging
	static u8 counter = 0;
	/* Define a dummy handler that just blinks an LED as a proof of concept. */
	tempoTimingManager.addPlaybackStepHandler([]
	{
		if(counter == 2)
		{
			rhythmLedPin.toggle();
			counter = 0;
		}
		counter++;
	});
}

/* Configure the millisecond timer to trigger once every millisecond by using
 * the microcontrollers 16 MHz CPU clock (1/16 us period), scaling that to
 * 250Khz (1/250 ms per tick) and setting a period to 250 ticks. */
void setupMillisecondTimer()
{
	tim0.enablePeriodicInterrupts();
	tim0.setPrescaler(Timer8Bit::PrescaleOption::_64);
	tim0.setPeriod(250);
	tim0.start();
}

/* The 74HC595 four digit segment display can only display one digit at a time.
 * This function attaches a lambda to Timer0CompareMatch IRQ that cycles what
 * current digit [0-3] that the display should output to use persistence of
 * vision to make it appear as though all digits are on at the same time. */
void registerDisplayDriverInterrupt()
{
	static u8 currentDigit = 0;
	InterruptHandler timerISR = []
	{
		tempoDisplay.outputDigit(currentDigit);
		currentDigit = (currentDigit + 1) % 5;
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer0CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

/* Enable interrupts triggered on pin D2, used to make the rotary encoder
 * implementation of tempo knob to work, which relies on these interrupts to
 * catch voltage changes generated by the encoder hardware upon rotation. */
void setupExternalPinInterrupt()
{
	EIMSK |= 0x1 << INT0; // enable external interrupt request 0 (Pin D2)
	EICRA |= 0x2 << ISC00; // trigger on falling edge
}
