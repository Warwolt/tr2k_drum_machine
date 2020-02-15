/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
static TempoTimingManager& timingManager = Startup::getTempoTimingManager();
static GpioPin ledPin {Pin5, PortB, DataDirection::DigitalOutput};

int main()
{
	Startup::init();

	/* Setup LED to blink ones per quarter note */
	static u8 tempoLedCounter = 0;
	timingManager.addPlaybackStepHandler([]() {
		if (tempoLedCounter == 0)
		{
			ledPin.toggle();
			scheduler.scheduleCallback([](){ ledPin.toggle();}, 50);
		}
		tempoLedCounter = (tempoLedCounter + 1) % 4;
	});

	/* Setup step LEDs to blink */
	static u8 stepLedCounter = 0;
	timingManager.addPlaybackStepHandler([]() {
		stepLeds.toggleLed(stepLedCounter);
		scheduler.scheduleCallback([](u16 x){ stepLeds.toggleLed(x);}, stepLedCounter, 40);
		stepLedCounter = (stepLedCounter + 1) % 16;
	});

	while (1)
	{
		for (int i = 0; i < stepButtons.getNumButtons(); i++)
		{
			if (stepButtons.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
			}
		}

		scheduler.checkSchedule();
		timingManager.handlePlayback();
	}
}
