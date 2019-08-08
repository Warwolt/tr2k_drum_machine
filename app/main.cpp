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

void init();
void registerSpiInterrupt();
void registerTempoTimerInterrupt();
void registerMillisecondTimerInterrupt();
void setupSpi();
void setupTempoTimer();
void setupTimingManager();
void setupMillisecondTimer();

int main()
{
	init();

	display.setNumber(1234);
	display.enableDecimalPoint(1);

	while (1)
	{

	}
}

void init()
{
	Interrupts::enableInterruptsGlobally();

	/* Spi */
	registerSpiInterrupt();
	setupSpi();

	/* Tempo timng */
	registerTempoTimerInterrupt();
	setupTempoTimer();
	setupTimingManager();

	/* Tempo display */
	registerMillisecondTimerInterrupt();
	setupMillisecondTimer();
}

void registerSpiInterrupt()
{
	InterruptHandler spiTransferISR = []
	{
		if (!spi.txBufferIsEmpty())
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

void registerTempoTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

void setupSpi()
{
	spi.setBitOrder(SpiBitOrder::MsbFirst);
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_128);
}

void setupTempoTimer()
{
	tim1.enablePeriodicInterrupts();
	tempoTimer.setTempo(BeatsPerMinute(120));
	// tempoTimer.start();
}

void setupTimingManager()
{
	static	u8 counter = 0;
	timingManager.addPlaybackStepHandler([]
	{
		if (counter == 2)
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

void registerMillisecondTimerInterrupt()
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

