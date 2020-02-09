/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();

int main()
{
	Startup::init();

	while (1)
	{
		for (int buttonNum = 0; buttonNum < stepButtons.getNumButtons(); buttonNum++)
		{
			if (stepButtons.buttonPressedNow(buttonNum))
			{
				stepLeds.toggleLed(buttonNum);
				scheduler.scheduleCallback([=]() { stepLeds.clearLed(buttonNum); }, 1500);
			}
		}

		scheduler.checkSchedule();
	}
}
