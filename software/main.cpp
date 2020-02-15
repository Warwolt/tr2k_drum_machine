/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

// static LedGroup& stepLeds = Startup::getStepLeds();
// static ButtonGroup& stepButtons = Startup::getStepButtons();
// static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
// static GpioPin ledPin2 {Pin4, PortB, DataDirection::DigitalOutput}; // global only while doing some testing

int main()
{
	Startup::init();

	while(1)
	{
	// 	for (int i = 0; i < stepButtons.getNumButtons(); i++)
	// 	{
	// 		if (stepButtons.buttonPressedNow(i))
	// 		{
	// 			// stepLeds.toggleLed(i);
	// 			// scheduler.scheduleCallback([](u16 x){ stepLeds.clearLed(x);}, i, 1500);
	// 			ledPin.set();
	// 			scheduler.scheduleCallback([](){ ledPin.clear();}, 100);
	// 		}
	// 	}

	// 	scheduler.checkSchedule();
	}
}
