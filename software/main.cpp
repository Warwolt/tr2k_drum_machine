/*
****************************************************************************************************
* brief : Main program file for "tr2k drum machine" step sequencer software.
****************************************************************************************************
*/

#include "Startup.h"
// while prototyping
#include "r2k/function.h"
#include "r2k/vector.h"
#include "MicrosecondPeriodMillisecondTimer.h"

static LedGroup& stepLeds = Startup::getStepLeds();
static ButtonGroup& stepButtons = Startup::getStepButtons();
static CallbackScheduler& scheduler = Startup::getCallbackScheduler();
static TempoTimingManager& timingManager = Startup::getTempoTimingManager();
static MatrixMappedButtonGroup<GpioPin>& transportButtons = Startup::getTransportButtons();
static RhythmPlaybackController& playbackController = Startup::getPlaybackController();
extern MicrosecondPeriodMillisecondTimer microsecondTimer;

static constexpr u8 startButton = 0;
static constexpr u8 stopButton = 1;

// made global while debugging
GpioPin boardLed {Pin5, PortB, DataDirection::DigitalOutput};

// prototype a r2k::function based callback scheduler
class CallbackScheduler2
{
public:
    using CallbackFunction = r2k::function<void()>;
    CallbackScheduler2(MillisecondTimer& timer) : timer(timer) {}

	// Add new function
	void scheduleCallback(CallbackFunction function, MillisecondTimer::milliseconds waitTime)
	{
		if (schedule.size() < schedule.capacity())
		{
			auto startTime = timer.getCurrentTime();
			schedule.push_back({function, startTime, waitTime, false});
			boardLed.set();
		}
	}

	// Check if functions should be called
	void checkSchedule()
	{
		// Call all due functions
		auto timeNow = timer.getCurrentTime();
		for (auto& scheduling : schedule)
		{
			auto elapsedTime = timeNow - scheduling.startTime;
			if (elapsedTime >= scheduling.waitTime)
			{
				scheduling.function();
				scheduling.handled = true;
			}
		}

		// Remove called functions
		for (auto& scheduling : schedule)
		{
			if (scheduling.handled)
			{
				scheduling = schedule.back();
				schedule.pop_back();
			}
		}
	}

private:
    struct ScheduleInfo
    {
		CallbackFunction function;
        MillisecondTimer::milliseconds startTime;
        MillisecondTimer::milliseconds waitTime;
		bool handled;
	};
	MillisecondTimer& timer;
	static constexpr u8 maxNumCallbacks = 32;
	r2k::vector<ScheduleInfo, maxNumCallbacks> schedule;
};

int main()
{
	Startup::init();
	CallbackScheduler2 scheduler2(microsecondTimer);
	scheduler2.scheduleCallback([](){ boardLed.set(); }, 0); // schedule immediate
	// scheduler2.checkSchedule();

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
		scheduler2.checkSchedule(); // testing out prototype
		timingManager.handlePlayback();
	}
}
