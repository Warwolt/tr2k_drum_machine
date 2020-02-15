/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

/* Includes --------------------------------------------------------------------------------------*/
#include "Startup.h"
#include <util/delay.h>
#include <avr/io.h>

/* Hardware Abstraction Layer */
#include "CharlieplexMappedLedGroup.h"
#include "MicrosecondPeriodMillisecondTimer.h"
#include "MatrixMappedButtonGroup.h"

/* Drivers */
#include "GpioPin.h"
#include "Timer0.h"
#include "CharlieplexMatrix.h"

/* Infrastructure */
#include "Interrupts.h"

// debugging
#include "Timer1.h"
#include "TempoTimer16Bit.h"
#include "TempoTimingManager.h"

/* Instantiations --------------------------------------------------------------------------------*/
/* Drivers and Hardware Abstraction Layer */
static Timer0 tim0;
static constexpr u16 microsecondPeriod = 100; // IF THIS IS LESS THAN 100us BUTTON GROUP WON'T WORK!
static MicrosecondPeriodMillisecondTimer microsecondTimer(tim0, microsecondPeriod);

/* Infrastructure */
static CallbackScheduler callbackScheduler {microsecondTimer};

/* Pattern edit LEDs */
static PinStatePair fivePinLut[20] =
{
	{1,0},{0,1},{2,1},{1,2},{2,0},{0,2},{3,2},{2,3},{3,1},{1,3},
	{3,0},{0,3},{4,3},{3,4},{4,2},{2,4},{4,1},{1,4},{4,0},{0,4}
};
static GpioPin ledPins[5] = {GpioPin(Pin1, PortC), GpioPin(Pin2, PortC), GpioPin(Pin3, PortC),
	GpioPin(Pin4, PortC), GpioPin(Pin5, PortC)};
static CharlieplexMatrix<GpioPin> ledMatrix = CharlieplexMatrix<GpioPin>(5, ledPins, fivePinLut);
constexpr u8 numStepLeds = 16;
static CharlieplexMappedLedGroup<GpioPin> charlieStepLeds = CharlieplexMappedLedGroup<GpioPin>(numStepLeds, ledMatrix);
static LedGroup& stepLeds = charlieStepLeds;

/* Pattern edit buttons*/
static constexpr MillisecondTimer::milliseconds buttonDebounceTime = 10; // ms
static constexpr u8 numButtonColumns = 4;
static constexpr u8 numButtonRows = 5;
static GpioPin buttonColumnPins[numButtonColumns] = {GpioPin(Pin3, PortB), GpioPin(Pin2, PortB),
	GpioPin(Pin1, PortB), GpioPin(Pin0, PortB)};
static GpioPin buttonRowPins[numButtonRows] = {GpioPin(Pin6, PortD), GpioPin(Pin5, PortD),
	GpioPin(Pin4, PortD), GpioPin(Pin3, PortD), GpioPin(Pin2, PortD)};
static GpioMatrix<GpioPin> buttonMatrix = GpioMatrix<GpioPin>(buttonColumnPins, numButtonColumns,
	buttonRowPins, numButtonRows, microsecondTimer, buttonDebounceTime);
static constexpr u8 numStepButtons = 16;
static MatrixMappedButtonGroup<GpioPin> stepButtons = MatrixMappedButtonGroup<GpioPin>(buttonMatrix, numStepButtons, 0);

/* Rhythm Playback */
static Timer1 tim1;
static TempoTimer16Bit tempoTimer {tim1};
static TempoTimingManager timingManager {tempoTimer};

/* Private function declarations -----------------------------------------------------------------*/
static void setupTimers();
static void registerTimer0InterruptHandlers();
static void registerTimer1InterruptHandlers();

/* Public function definitions -------------------------------------------------------------------*/
LedGroup& Startup::getStepLeds()
{
	return stepLeds;
}

ButtonGroup& Startup::getStepButtons()
{
	return stepButtons;
}

CallbackScheduler& Startup::getCallbackScheduler()
{
	return callbackScheduler;
}

TempoTimingManager& Startup::getTempoTimingManager()
{
	return timingManager;
}

/* Configure all objects instantiated by the Startup module. NB: this function
 * MUST be called before using any object to guarantee correct behavior! */
void Startup::init()
{
	Interrupts::enableInterruptsGlobally();
	setupTimers();
}

/* Private function definitions ------------------------------------------------------------------*/
/**
 *
 */
static void setupTimers()
{
	/* Set up hardware timers */
	registerTimer0InterruptHandlers();
	registerTimer1InterruptHandlers();
	tim0.enablePeriodicInterrupts();
	tim1.enablePeriodicInterrupts();

	/* Set up software timers */
	tempoTimer.setTempo(120); // we have to set tempo else timer doesn't start

	/* Start timers */
	tim0.start(); // starts msec timer
	tempoTimer.start();
}

/**
 * @brief Register timing based actions for HW timer tim0
 */
static void registerTimer0InterruptHandlers()
{
	InterruptHandler timerISR = []
	{
		ledMatrix.outputNextLed();
		microsecondTimer.countPeriod();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer0CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

/**
 * @brief Register timing based actions for HW timer tim1
 */
static void registerTimer1InterruptHandlers()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}
