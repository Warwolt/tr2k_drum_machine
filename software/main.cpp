/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
*
* This current version of the sequencer contains a user interface for setting the playback tempo in
* beats per minute using a rotary encoder and viewing the set tempo on a 4-digit display. An LED is
* then pulsed once every 16th step, to demonstrate that the tempo timing manager works.
****************************************************************************************************
*/

#include "startup2.h"

int main()
{
	LedGroup& stepLeds = Startup2::getStepLeds();
	GpioMatrix<GpioPin>& buttonMatrix = Startup2::getButtonMatrix();

	/* Initialize the tempo control view and the tempo timing manager, along
	 * with all of their dependencies. */
	Startup2::init();

	constexpr u8 numButtons = 20;
	while(1)
	{
		for(int i = 0; i < numButtons; i++)
		{
			if(buttonMatrix.readElement(i) == LogicState::High)
			{
				stepLeds.setLed(i);
			}
			else
			{
				stepLeds.clearLed(i);
			}

		}
	}
}
