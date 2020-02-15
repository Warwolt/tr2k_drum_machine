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
	ledPin.clear();
	static u8 counter = 0;
	timingManager.addPlaybackStepHandler([]() {
		counter++;
		if (counter == 3)
		{
			ledPin.set();
			scheduler.scheduleCallback([](){ ledPin.clear();}, 50);
			counter = 0;
		}
	});

	while(1)
	{
		for (int i = 0; i < stepButtons.getNumButtons(); i++)
		{
			if (stepButtons.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
				scheduler.scheduleCallback([](u16 x){ stepLeds.clearLed(x);}, i, 1000);
			}
		}

		scheduler.checkSchedule();
		timingManager.handlePlayback();
	}
}
