/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "startup.h"

int main()
{
	TempoControlView& tempoControlView = Startup::getTempoControlView();
	TempoTimingManager& tempoTimingManager = Startup::getTempoTimingManager();

	Startup::init();

	while(1)
	{
		tempoControlView.handleTempoControl();
		tempoTimingManager.handlePlayback();
	}
}
