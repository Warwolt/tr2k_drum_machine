/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
static TempoTimingManager& timingManager = Startup::getTempoTimingManager();
static MatrixMappedButtonGroup<GpioPin>& transportButtons = Startup::getTransportButtons();
static RhythmPlaybackController& playbackController = Startup::getPlaybackController();
static GpioPin boardLed {Pin5, PortB, DataDirection::DigitalOutput};

static constexpr u8 startButton = 0;
static constexpr u8 stopButton = 1;

int main()
{
	Startup::init();

	/* Setup LED to blink ones per quarter note */
	static u8 tempoLedCounter = 0;
	timingManager.addPlaybackStepHandler([]() {
		if (tempoLedCounter == 0)
		{
			boardLed.toggle();
			scheduler.scheduleCallback([](){ boardLed.toggle();}, 50);
		}
		tempoLedCounter = (tempoLedCounter + 1) % 4;
	});

	/* Setup step LEDs to blink */
	static u8 stepLedCounter = 0;
	timingManager.addPlaybackStepHandler([]() {
		stepLeds.toggleLed(stepLedCounter);
		scheduler.scheduleCallback([](u16 x){ stepLeds.toggleLed(x);}, stepLedCounter, 40);
		stepLedCounter = (stepLedCounter + 1) % 16;
	});

	bool playbackIsOngoing = false;
	u8 currentStepButton = 0;
	while (1)
	{
		/* Play button */
		if (transportButtons.buttonPressedNow(startButton))
		{
			playbackIsOngoing = true;
			if (playbackIsOngoing)
			{
				/* Reset playback state */
				tempoLedCounter = 0;
				stepLedCounter = 0;
			}

			playbackController.restartPlayback();
		}

		/* Stop/continue button */
		if (transportButtons.buttonPressedNow(stopButton))
		{
			/* Stop */
			if (playbackIsOngoing)
			{
				playbackController.stopPlayback();
				playbackIsOngoing = false;
			}
			/* Continue */
			else
			{
				playbackController.continuePlayback();
				playbackIsOngoing = true;
			}
		}

		/* Step buttons */
		if (stepButtons.buttonPressedNow(currentStepButton))
		{
			stepLeds.toggleLed(currentStepButton);
		}
		currentStepButton = (currentStepButton + 1) % stepLeds.getNumLeds();

		scheduler.checkSchedule();
		timingManager.handlePlayback();
	}
}
