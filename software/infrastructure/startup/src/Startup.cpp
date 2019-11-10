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
#include "charlieplex_mapped_led_group.h"
#include "microsecond_period_millisecond_timer.h"
#include "matrix_mapped_button_group.h"

/* Drivers */
#include "gpiopin.h"
#include "timer0.h"
#include "charlieplex_matrix.h"

/* Infrastructure */
#include "interrupts.h"

/* Instantiations --------------------------------------------------------------------------------*/
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
static Timer0 tim0;
static constexpr u16 microsecondPeriod = 100; // IF THIS IS LESS THAN 100us BUTTON GROUP WON'T WORK!
static MicrosecondPeriodMillisecondTimer microsecondTimer(tim0, microsecondPeriod);
static constexpr MillisecondTimer::milliseconds buttonDebounceTime = 0; // ms
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

/* Private function declarations -----------------------------------------------------------------*/
static void setupTimer0();
static void registerTimer0InterruptHandlers();

/* Public function definitions -------------------------------------------------------------------*/
LedGroup& Startup::getStepLeds()
{
	return stepLeds;
}

ButtonGroup& Startup::getStepButtons()
{
	return stepButtons;
}

/* Configure all objects instantiated by the Startup module. NB: this function
 * MUST be called before using any object to guarantee correct behavior! */
void Startup::init()
{
	Interrupts::enableInterruptsGlobally();

	setupTimer0();
	registerTimer0InterruptHandlers();
}

/* Private function definitions ------------------------------------------------------------------*/
/* Configure timer0 so that periodical interrupts will trigger. */
void setupTimer0()
{
	tim0.enablePeriodicInterrupts();
	tim0.start();
}

/* The 74HC595 four digit segment display can only display one digit at a time.
 * This function attaches a lambda to Timer0CompareMatch IRQ that cycles what
 * current digit [0-3] that the display should output to use persistence of
 * vision to make it appear as though all digits are on at the same time. */
void registerTimer0InterruptHandlers()
{
	InterruptHandler timerISR = []
	{
		ledMatrix.outputNextLed();
		microsecondTimer.countPeriod();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer0CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}
