/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

/* Includes --------------------------------------------------------------------------------------*/
#include "startup2.h"
#include <util/delay.h>
#include <avr/io.h>

/* Hardware Abstraction Layer */
#include "charlieplex_mapped_led_group.h"
#include "microsecond_period_millisecond_timer.h"

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
// NOTE: While mucking about with testing the button matrix in hardware I'm
// currently reusing some of the gpio pins that were assigned above! Later on I
// will have to make an implementation of the button matrix that uses a shift
// register for gpio expansion, but until then I will run without the 4 digit
// display and the rotary encoder and just use the buttons and leds.

static Timer0 tim0; // timer0 interrupts used to cycle through digits in segment display
static constexpr u16 microsecondPeriod = 52; // 19200 Hz, 1200 Hz per LED for 16 LEDs (enough for flicker fusion)
static MicrosecondPeriodMillisecondTimer microsecondTimer(tim0, microsecondPeriod);
static constexpr MillisecondTimer::milliseconds buttonDebounceTime = 5; // ms
static constexpr u8 numButtonColumns = 4;
static constexpr u8 numButtonRows = 5;
static GpioPin buttonColumnPins[numButtonColumns] = {GpioPin(Pin3, PortB), GpioPin(Pin2, PortB),
	GpioPin(Pin1, PortB), GpioPin(Pin0, PortB)};
static GpioPin buttonRowPins[numButtonRows] = {GpioPin(Pin6, PortD), GpioPin(Pin5, PortD),
	GpioPin(Pin4, PortD), GpioPin(Pin3, PortD), GpioPin(Pin2, PortD)};
static GpioMatrix<GpioPin> buttonMatrix = GpioMatrix<GpioPin>(buttonColumnPins, numButtonColumns,
	buttonRowPins, numButtonRows, microsecondTimer, buttonDebounceTime);

/* Private function declarations -----------------------------------------------------------------*/
static void setupTimer0();
static void registerTimer0InterruptHandlers();

/* Public function definitions -------------------------------------------------------------------*/
LedGroup& Startup2::getStepLeds()
{
	return stepLeds;
}

GpioMatrix<GpioPin>& Startup2::getButtonMatrix()
{
	return buttonMatrix;
}

/* Configure all objects instantiated by the Startup module. NB: this function
 * MUST be called before using any object to guarantee correct behavior! */
void Startup2::init()
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
		// quick and dirty way of testing that led output works, should be moved
		// into a timer compare interrupt running every 50 microseconds!
		// TODO move this somewhere much more fitting!
		ledMatrix.outputNextLed();

		// quick and dirty way of testing that microsecond period millisecond
		// timer works for debouncing buttons in the gpio matrix.
		microsecondTimer.countPeriod();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer0CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

