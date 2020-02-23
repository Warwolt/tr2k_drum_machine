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

	while (1)
	{
		editView.update();
		scheduler.checkSchedule();
		playbackManager.handlePlayback();
	}
}
