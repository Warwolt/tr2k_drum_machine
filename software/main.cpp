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

	while(1)
	{
		for(int i = 0; i < stepButtons.getNumButtons(); i++)
		{
			if(stepButtons.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
				scheduler.scheduleCallback([=]()
				{
					stepLeds.clearLed(i);
				}, 1500);
			}
		}

		scheduler.checkSchedule();
	}
}
