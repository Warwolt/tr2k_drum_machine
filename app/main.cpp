#include "interrupts.h"
#include "spi.h"
#include "gpiopin.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"
#include <util/delay.h>

static Spi spi;
static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager timingManager = TempoTimingManager(tempoTimer);
static GpioPin ledPin = GpioPin(Pin5, PortC, DigitalOutput);

void init();
void registerSpiInterrupt();
void registerTimerInterrupt();
void setupSpi();
void setupTempoTimer();
void setupTimingManager();

int main()
{
	init();

	r2k::vector<u8, 4> txBuffer = {1, 2, 3, 4};

	while (1)
	{
		spi.setTxBuffer(txBuffer);
		spi.sendNextByteInBuffer();
		_delay_us(100);
	}
}

void init()
{
	Interrupts::enableInterruptsGlobally();
	registerSpiInterrupt();
	registerTimerInterrupt();
	setupSpi();
	setupTempoTimer();
	setupTimingManager();
}

void registerSpiInterrupt()
{
	InterruptHandler spiTransferISR = []
	{
		if (!spi.txBufferIsEmpty())
		{
			spi.sendNextByteInBuffer();
		}
	};

	InterruptRequest spiIRQ = InterruptRequest::SpiTransferComplete;
	Interrupts::setHandlerForInterrupt(spiTransferISR, spiIRQ);
}

void registerTimerInterrupt()
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
	spi.setBitOrder(SpiBitOrder::LsbFirst);
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_2);
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
