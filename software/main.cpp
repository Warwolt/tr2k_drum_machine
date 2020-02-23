/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

// TODO: clean this mess up after implementing views
static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& callbackScheduler = Startup::getCallbackScheduler();
static RhythmPlaybackManager& playbackManager = Startup::getRhythmPlaybackManager();
static MatrixMappedButtonGroup<GpioPin>& transportButtons = Startup::getTransportButtons();
static RhythmPlaybackController& playbackController = Startup::getPlaybackController();
static GpioPin boardLed {Pin5, PortB, DataDirection::DigitalOutput};
static GpioPin outputPin2 {Pin0, PortC, DataDirection::DigitalOutput};

static PatternEditView& patternEditView = Startup::getPatternEditView();
static PlaybackControlView& playbackControlView = Startup::getPlaybackControlView();

// TODO: implement signal output with a class instead of directly in main, then remove this extern
#include "RhythmPatternManager.h"
extern RhythmPatternManager patternManager;

static void registerPlaybackHandlers();

int main()
{
	Startup::init();
	registerPlaybackHandlers();

	while (1)
	{
		patternEditView.update();
		playbackControlView.update();
		callbackScheduler.checkSchedule();
		playbackManager.handlePlayback();
	}
}

void registerPlaybackHandlers()
{
	/* Setup LED to blink according to programmed pattern */
	playbackManager.addPlaybackHandler([]()
	{
		RhythmPattern activePattern = patternManager.getPattern(0);
		if (stepActiveInPattern(playbackManager.getPlaybackPosition(), activePattern))
		{
			boardLed.toggle();
			callbackScheduler.scheduleCallback([](){ boardLed.toggle();}, 50);
		}
	});

	playbackManager.addPlaybackHandler([]()
	{
		RhythmPattern activePattern = patternManager.getPattern(1);
		if (stepActiveInPattern(playbackManager.getPlaybackPosition(), activePattern))
		{
			outputPin2.toggle();
			callbackScheduler.scheduleCallback([](){ outputPin2.toggle();}, 50);
		}
	});

	/* Setup step LEDs to blink */
	static u8 stepLedCounter = 0;
	playbackManager.addPlaybackHandler(
	{
		.handlePlaybackStep = []()
		{
			stepLedCounter = (stepLedCounter + 1) % 16;
		},
		.resetPlayback = [] ()
		{
			stepLedCounter = 0;
		}
	});
}
