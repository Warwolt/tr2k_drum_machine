#include "gpiopin.h"
#include "timer1.h"
#include "interrupts.h"
#include "tempotimer16bit.h"

static GpioPin ledPin = GpioPin(Pin5, PortB, DigitalOutput);
static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);

void init();
void registerTimerInterrupt();
void setupTimer1();
void setupTempoTimer();

int main()
{
	init();

	while(1)
	{
		if(tempoTimer.playbackStepIsDue())
		{
			ledPin.toggle();
			tempoTimer.startCountingNextStep();
		}
	}
}

void init()
{
	Interrupts::enableInterruptsGlobally();
	registerTimerInterrupt();
	setupTimer1();
	setupTempoTimer();
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

void setupTimer1()
{
	tim1.enablePeriodicInterrupts();
	tim1.setPrescaler(Timer16Bit::PrescaleOption::_1);
}

void setupTempoTimer()
{
	tempoTimer.setTempo(BeatsPerMinute(120));
	tempoTimer.start();
}