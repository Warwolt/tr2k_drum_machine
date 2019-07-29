#include "interrupts.h"
#include "gpiopin.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"
#include <util/delay.h>

static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager timingManager = TempoTimingManager(tempoTimer);
// static GpioPin ledPin = GpioPin(Pin5, PortB, DigitalOutput);
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

	/* Initialize SPI peripheral */
    // set MOSI, SCK and SS as Output
    DDRB = (1 << 5) | (1 << 3) | (1 << 2);
    // enable transfer complete interrupts
    SPCR |= (1 << SPIE);
	// configure SPI pins
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << DORD);

    InterruptHandler spiTransferISR = []
    {
		if(byteIndex < 4)
		{
			SPDR = txBuffer[byteIndex];
			byteIndex++;
		}
    };
    InterruptRequest spiIRQ = InterruptRequest::SpiTransferComplete;
    setHandlerForInterrupt(spiTransferISR, spiIRQ);

	while (1)
	{
		SPDR = txBuffer[0]; // write to triggers first interrupt
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
