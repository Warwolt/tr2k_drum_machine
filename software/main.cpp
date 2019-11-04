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
#include "matrix_mapped_button_group.h"

int main()
{
	LedGroup& stepLeds = Startup2::getStepLeds();
	GpioMatrix<GpioPin>& buttonMatrix = Startup2::getButtonMatrix();
	// quick test of button group
	constexpr u8 numButtons = 16;
	MatrixMappedButtonGroup<GpioPin> buttonGroup(buttonMatrix, numButtons, 0);

	/* Initialize the tempo control view and the tempo timing manager, along
	 * with all of their dependencies. */
	Startup2::init();

	while(1)
	{
		for(int i = 0; i < numButtons; i++)
		{
			if(buttonGroup.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
			}
		}
	}
}
