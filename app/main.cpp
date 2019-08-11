#include "interrupts.h"
#include "spi.h"
#include "gpiopin.h"
#include "timer0.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"
#include <util/delay.h>
#include "math.h"
#include "segmentdisplay.h"

static Spi spi;
static Timer0 tim0;
static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager timingManager = TempoTimingManager(tempoTimer);
static SegmentDisplay74HC595 display = SegmentDisplay74HC595(spi);
static GpioPin ledPin = GpioPin(Pin5, PortC, DigitalOutput);
static GpioPin dataLatchPin = GpioPin(Pin2, PortB, DigitalOutput);
static GpioPin rotaryEncoderPin = GpioPin(Pin3, PortC, DigitalInput);

static GpioPin rotaryEncoderPinA = GpioPin(Pin2, PortD, DigitalInput);
static GpioPin rotaryEncoderPinB = GpioPin(Pin3, PortC, DigitalInput);

/* Startup */
void init();
/* Spi */
void setupSpi();
void registerSpiInterrupt();
/* Tempo Timing */
void setupTempoTimer();
void registerTempoTimerInterrupt();
void setupTimingManager();
/* Tempo Display */
void setupMillisecondTimer();
void registerDisplayDriverInterrupt();

static volatile s16 encoderRotations;
ISR(INT0_vect)
{
	_delay_us(100); // give time for signals to stabilize

	LogicState stateA = rotaryEncoderPinA.read();
	LogicState stateB = rotaryEncoderPinB.read();

	if(stateA == LogicLow and stateB == LogicLow) // turn right
	{
		encoderRotations++;
	}
	else if(stateA == LogicLow and stateB == LogicHigh) // turn left
	{
		encoderRotations--;
	}
}

int main()
{
	init();

	/* Setup rotary encoder */
	EIMSK |= 0x1 << INT0; // enable external interrupt request 0
	EICRA |= 0x2 << ISC00; // trigger on falling edge

	display.enableDecimalPoint(1);

	while(1)
	{
		// Display stuff
		u8 currentBpm = 120 + encoderRotations;
		display.setNumber(currentBpm*10);

		// Handle playback
		tempoTimer.setTempo(BeatsPerMinute(currentBpm));
		timingManager.handlePlayback();
	}
}

void init()
{
	Interrupts::enableInterruptsGlobally();

	/* Spi */
	setupSpi();

	/* Tempo timng */
	setupTempoTimer();
	setupTimingManager();

	/* Tempo display */
	setupMillisecondTimer();
	registerDisplayDriverInterrupt();
}

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

void registerTempoTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

void setupTimingManager()
{
	static	u8 counter = 0;
	timingManager.addPlaybackStepHandler([]
	{
		if(counter == 2)
		{
			ledPin.toggle();
			counter = 0;
		}
		counter++;
	});
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
		display.outputDigit(currentDigit);
		currentDigit = (currentDigit + 1) % 5;
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer0CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

