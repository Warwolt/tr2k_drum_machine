/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

// TODO: clean this mess up after implementing views
static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
static RhythmPlaybackManager& playbackManager = Startup::getRhythmPlaybackManager();
static MatrixMappedButtonGroup<GpioPin>& transportButtons = Startup::getTransportButtons();
static RhythmPlaybackController& playbackController = Startup::getPlaybackController();
static GpioPin boardLed {Pin5, PortB, DataDirection::DigitalOutput};

static PatternEditView& editView = Startup::getPatternEditView();

static constexpr u8 startButton = 0;
static constexpr u8 stopButton = 1;
static constexpr u8 clearButton = 3;

int main()
{
	Startup::init();
	static u16 playbackPattern = 0x0;

	/* Setup LED to blink according to programmed pattern */
	static u8 playbackPosition = 0;
	playbackManager.addPlaybackStepHandler([]() {
		if (playbackPattern & (0x1 << playbackPosition))
		{
			boardLed.toggle();
			scheduler.scheduleCallback([](){ boardLed.toggle();}, 50);
		}
		playbackPosition = (playbackPosition + 1) % 16;
	});

	/* Setup step LEDs to blink */
	static u8 stepLedCounter = 0;
	playbackManager.addPlaybackStepHandler([]() {
		stepLedCounter = (stepLedCounter + 1) % 16;
	});

	// bool playbackIsOngoing = false;
	// u8 currentStepIndex = 0; // use this as 'for loop' index
	// quick test of pattern edit view
	while (1)
	{
		editView.update();
		scheduler.checkSchedule();
		playbackManager.handlePlayback();
	}

	// while (1)
	// {
	// 	/* Play button */
	// 	if (transportButtons.buttonPressedNow(startButton))
	// 	{
	// 		playbackIsOngoing = true;
	// 		if (playbackIsOngoing)
	// 		{
	// 			/* Reset playback state */
	// 			playbackPosition = 0;
	// 			stepLedCounter = 0;
	// 		}

	// 		playbackController.restartPlayback();
	// 	}

	// 	/* Stop button */
	// 	if (transportButtons.buttonIsDown(stopButton))
	// 	{
	// 		/* Stop */
	// 		playbackIsOngoing = false;
	// 		playbackController.stopPlayback();
	// 	}

	// 	/* Clear button */
	// 	if (transportButtons.buttonIsDown(clearButton))
	// 	{
	// 		if (playbackIsOngoing)
	// 		{
	// 			/* Clear whatever the playback position passes */
	// 			playbackPattern &= ~(0x1 << playbackPosition);
	// 		}
	// 		else
	// 		{
	// 			/* Immediately clear the whole pattern */
	// 			playbackPattern = 0;
	// 		}
	// 	}

	// 	/* Step buttons */
	// 	if (stepButtons.buttonPressedNow(currentStepIndex))
	// 	{
	// 		playbackPattern ^= (0x1 << currentStepIndex);
	// 	}

	// 	/* Display pattern and playback position on step leds */
	// 	// fill draw buffer
	// 	u16 drawBuffer = 0;
	// 	for (size_t i = 0; i < 16; i++)
	// 	{
	// 		/* Rhythm pattern */
	// 		drawBuffer |= (playbackPattern & (0x1 << i));
	// 	}
	// 	if (playbackIsOngoing)
	// 	{
	// 		/* Playback position */
	// 		drawBuffer ^= (0x1 << playbackPosition);
	// 	}
	// 	// output draw buffer on leds
	// 	for (size_t i = 0; i < 16; i++)
	// 	{
	// 		(drawBuffer >> i) & 0x1 ? stepLeds.setLed(i) : stepLeds.clearLed(i);
	// 	}

	// 	/* Select next step to handle */
	// 	currentStepIndex = (currentStepIndex + 1) % stepLeds.getNumLeds();

	// 	/* Check timing managers */
	// 	scheduler.checkSchedule();
	// 	playbackManager.handlePlayback();
	// }
}
