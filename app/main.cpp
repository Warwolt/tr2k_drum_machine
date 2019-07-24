#include "interrupts.h"
#include "gpiopin.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"

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

int main()
{
	init();

	/* Initialize SPI peripheral */
    // set MOSI, SCK as Output
    DDRB = (1<<5) | (1<<3) | (1<<2);
	// configure SPI pins
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);

	while (1)
	{
		// initiate transfer of 1 byte
		SPDR = 0x1;

		// wait for transfer to complete
		while (!(SPSR & (0x1 << SPIF)));
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