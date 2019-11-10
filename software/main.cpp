/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();

int main()
{
	Startup::init();

	// quick test of callback scheduler
	CallbackScheduler& scheduler = Startup::getCallbackScheduler();

	while(1)
	{
		for(int i = 0; i < stepButtons.getNumButtons(); i++)
		{
			if(stepButtons.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
				// No access to closures so we just run with it and set the Led arg to zero
				scheduler.scheduleCallback([](){ stepLeds.clearLed(0);}, 1000);
			}
		}

		scheduler.checkSchedule();
	}
}
