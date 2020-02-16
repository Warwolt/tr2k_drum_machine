/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
static TempoTimingManager& tempoTimingManager = Startup::getTempoTimingManager();
static MatrixMappedButtonGroup<GpioPin>& transportButtons = Startup::getTransportButtons();
static RhythmPlaybackController& playbackController = Startup::getPlaybackController();
static GpioPin boardLed {Pin5, PortB, DataDirection::DigitalOutput};

static constexpr u8 startButton = 0;
static constexpr u8 stopButton = 1;

int main()
{
	Startup::init();
	static u16 playbackPattern = 0x0;

	/* Setup LED to blink according to programmed pattern */
	static u8 playbackPosition = 0;
	tempoTimingManager.addPlaybackStepHandler([]() {
		if (playbackPattern & (0x1 << playbackPosition))
		{
			boardLed.toggle();
			scheduler.scheduleCallback([](){ boardLed.toggle();}, 50);
		}
		playbackPosition = (playbackPosition + 1) % 16;
	});

	/* Setup step LEDs to blink */
	static u8 stepLedCounter = 0;
	tempoTimingManager.addPlaybackStepHandler([]() {
		stepLedCounter = (stepLedCounter + 1) % 16;
	});

	bool playbackIsOngoing = false;
	u8 currentStepIndex = 0; // use this as 'for loop' index
	while (1)
	{
		/* Play button */
		if (transportButtons.buttonPressedNow(startButton))
		{
			playbackIsOngoing = true;
			if (playbackIsOngoing)
			{
				/* Reset playback state */
				playbackPosition = 0;
				stepLedCounter = 0;
			}

			playbackController.restartPlayback();
		}

		/* Stop button */
		if (transportButtons.buttonIsDown(stopButton))
		{
			/* Stop */
			playbackIsOngoing = false;
			playbackController.stopPlayback();
		}

		/* Step buttons */
		if (stepButtons.buttonPressedNow(currentStepIndex))
		{
			playbackPattern ^= (0x1 << currentStepIndex);
		}

		/* Display pattern and playback position on step leds */
		u16 drawBuffer = 0;
		for (size_t i = 0; i < 16; i++)
		{
			drawBuffer |= (playbackPattern & (0x1 << i));
		}
		if (playbackIsOngoing)
		{
			drawBuffer ^= (0x1 << playbackPosition);
		}
		for (size_t i = 0; i < 16; i++)
		{
			(drawBuffer >> i) & 0x1 ? stepLeds.setLed(i) : stepLeds.clearLed(i);
		}
		// TODO: unroll this loop
		// for (size_t i = 0; i < 16; i++)
		// {
		// 	bool setPatternLed = (playbackPattern & (0x1 << i));
		// 	setPatternLed ? stepLeds.setLed(i) : stepLeds.clearLed(i);
		// }
		// if (playbackIsOngoing)
		// {
		// 	u8 positionLed = playbackPosition;
		// 	bool setPositionled = !(playbackPattern & (0x1 << playbackPosition));
		// 	setPositionled ? stepLeds.setLed(positionLed) : stepLeds.clearLed(positionLed);
		// }
		// Clear
		// for (size_t i = 0; i < stepLeds.getNumLeds(); i++) // TODO: unroll this loop so that it's not blocking!
		// {
		// 	stepLeds.clearLed(i);
		// }
		// Rhythm pattern
		// bool setPatternLed = (playbackPattern & (0x1 << currentStepIndex));
		// u8 patternLedNum = currentStepIndex;
		// setPatternLed ? stepLeds.setLed(patternLedNum) : stepLeds.clearLed(patternLedNum);
		// Playback position
		// if (playbackIsOngoing)
		// {
		// 	u8 playbackLed = playbackPosition;
		// 	bool setPlaybackLed = !(playbackPattern & (0x1 << playbackPosition));
		// 	setPlaybackLed ? stepLeds.setLed(playbackLed) : stepLeds.clearLed(playbackLed);
		// }
		/* Select next step to handle */
		currentStepIndex = (currentStepIndex + 1) % stepLeds.getNumLeds();

		/* Check timing managers */
		scheduler.checkSchedule();
		tempoTimingManager.handlePlayback();
	}
}
