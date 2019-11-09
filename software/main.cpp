/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "startup2.h"

int main()
{
	Startup2::init();
	LedGroup& stepLeds = Startup2::getStepLeds();
	ButtonGroup& stepButtons = Startup2::getStepButtons();

	while(1)
	{
		for(int i = 0; i < stepButtons.getNumButtons(); i++)
		{
			if(stepButtons.buttonPressedNow(i))
			{
				stepLeds.toggleLed(i);
			}
		}
	}
}
