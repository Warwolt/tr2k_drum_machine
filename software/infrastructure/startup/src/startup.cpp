/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

/* Includes --------------------------------------------------------------------------------------*/
#include "startup.h"
#include <util/delay.h>

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

/* Infrastructure */
#include "interrupts.h"


/* Instantiations --------------------------------------------------------------------------------*/
static Spi spi;
static Timer0 tim0;
static SegmentDisplay74HC595 tempoDisplay = SegmentDisplay74HC595(spi);
static GpioPin dataLatchPin = GpioPin(Pin2, PortB, DigitalOutput);

static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager tempoTimingManager = TempoTimingManager(tempoTimer);
static GpioPin rhythmLedPin = GpioPin(Pin5, PortC, DigitalOutput);

static GpioPin encoderPinA = GpioPin(Pin2, PortD, DigitalInput);
static GpioPin encoderPinB = GpioPin(Pin3, PortC, DigitalInput);
static RotaryEncoder<GpioPin> rotaryEncoder = RotaryEncoder<GpioPin>(encoderPinA, encoderPinB);
static DigitalTempoKnob<GpioPin> tempoKnob = DigitalTempoKnob<GpioPin>(rotaryEncoder);

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
void setupSpi()
{
	spi.setBitOrder(SpiBitOrder::MsbFirst);
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_128);
	registerSpiInterrupt();
}

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

void setupTempoTimer()
{
	registerTempoTimerInterrupt();
	tim1.enablePeriodicInterrupts();
	tempoTimer.setTempo(BeatsPerMinute(120));
	tempoTimer.start();
}

void registerPlaybackHandlers()
{
	rhythmLedPin.set(); // debugging
	static u8 counter = 0;
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

void registerTempoTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

void setupMillisecondTimer()
{
	tim0.enablePeriodicInterrupts();
	tim0.setPrescaler(Timer8Bit::PrescaleOption::_64);
	tim0.setPeriod(250);
	tim0.start();
}

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

void setupExternalPinInterrupt()
{
	EIMSK |= 0x1 << INT0; // enable external interrupt request 0 (Pin D2)
	EICRA |= 0x2 << ISC00; // trigger on falling edge
}
