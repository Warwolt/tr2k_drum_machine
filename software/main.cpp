/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static BlinkableLedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
using milliseconds = MillisecondTimer::milliseconds;

int main()
{
	Startup::init();
	constexpr milliseconds blinkPeriod = 1500;

	while (1)
	{
		for (int buttonNum = 0; buttonNum < stepButtons.getNumButtons(); buttonNum++)
		{
			if (stepButtons.buttonPressedNow(buttonNum))
			{
				stepLeds.blinkLed(buttonNum, blinkPeriod);
			}
		}

		scheduler.checkSchedule();
	}
}
