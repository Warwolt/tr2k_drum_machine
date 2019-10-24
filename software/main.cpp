/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
*
* This current version of the sequencer contains a user interface for setting the playback tempo in
* beats per minute using a rotary encoder and viewing the set tempo on a 4-digit display. An LED is
* then pulsed once every 16th step, to demonstrate that the tempo timing manager works.
****************************************************************************************************
*/

#include "startup.h"
#include "charlieplex_matrix.h"
#include "gpiopin.h"

int main()
{
	TempoControlView& tempoControlView = Startup::getTempoControlView();
	TempoTimingManager& tempoTimingManager = Startup::getTempoTimingManager();
	CharlieplexMatrix<GpioPin> ledMatrix = Startup::getLedMatrix(); // only temporary

	/* Initialize the tempo control view and the tempo timing manager, along
	 * with all of their dependencies. */
	Startup::init();

	// quick and dirty way to make sure all leds are on, while trying out the
	// charlieplex matrix in hardware.
	for(int i = 0; i < 16; i++)
	{
		ledMatrix.setLed(i);
	}

	while(1)
	{
		/* Update view that consists of a 4-digit display showing the current
		 * tempo in beats per minute, and a rotary encoder to set the tempo.*/
		tempoControlView.handleTempoControl();

		/* Manager checks with the tempo timer if next playback step is due
		 * (16th note), and if it is due the manager calls all the playback
		 * handler functions registered in the Startup::init() function. */
		tempoTimingManager.handlePlayback();

		// quick and dirty way of testing that led output works, should be
		// moved into a timer compare interrupt running every 50 microseconds!
		ledMatrix.outputNextLed();
	}
}
