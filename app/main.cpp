#include "interrupts.h"
#include "gpiopin.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"

static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager timingManager = TempoTimingManager(tempoTimer);
static GpioPin ledPin = GpioPin(Pin5, PortB, DigitalOutput);

using namespace Interrupts

void init();
void registerTimerInterrupt();
void setupTempoTimer();
void setupTimingManager();

int main()
{
	init();

	while (1)
	{
		timingManager.handlePlayback();
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