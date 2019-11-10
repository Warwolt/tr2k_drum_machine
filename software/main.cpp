/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

int main()
{
	Startup::init();
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
