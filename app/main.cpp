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

using namespace Interrupts;

void init();
void registerTimerInterrupt();
void setupTempoTimer();
void setupTimingManager();

static u8 txBuffer[] = {1, 2, 3, 4};
static u8 byteIndex = 0;

int main()
{
	init();

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_16);
	spi.setBitOrder(SpiBitOrder::LsbFirst);

    InterruptHandler spiTransferISR = []
    {
		if(byteIndex < 4)
		{
			spi.sendByte(txBuffer[byteIndex]);
			byteIndex++;
		}
    };

    InterruptRequest spiIRQ = InterruptRequest::SpiTransferComplete;
    setHandlerForInterrupt(spiTransferISR, spiIRQ);

	while (1)
	{
		spi.sendByte(txBuffer[0]);
		byteIndex = 1;
		_delay_ms(6);
	}
}

void init()
{
	enableInterruptsGlobally();
	registerTimerInterrupt();
	setupTempoTimer();
	setupTimingManager();
}

void registerTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	setHandlerForInterrupt(timerISR, timerIRQ);
}

void setupTempoTimer()
{
	tim1.enablePeriodicInterrupts();
	tempoTimer.setTempo(BeatsPerMinute(120));
	tempoTimer.start();
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
