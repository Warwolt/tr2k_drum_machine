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
				scheduler.scheduleCallback([](u16 x){ stepLeds.clearLed(x);}, i, 1000);
			}
		}

		scheduler.checkSchedule();
	}
}
