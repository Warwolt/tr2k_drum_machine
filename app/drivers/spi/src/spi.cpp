/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#include "spi.h"
#include "traceprint.h"

/**
 * Enables the SPI peripheral, enables "transfer complete"-interrupts, sets SPI
 * peripheral as master, and sets up MOSI, SCLK, and SS pins as outputs.
 */
void Spi::initialize()
{
	enableSpi();
	enableInterrupts();
	useMasterMode();
	setupPins();
}

inline void Spi::enableSpi()
{
	*controlRegister |= (0x1 << SPE);
}

inline void Spi::useMasterMode()
{
	*controlRegister |= (0x1 << MSTR);
}

inline void Spi::enableInterrupts()
{
	*controlRegister |= (0x1 << SPIE); // enable spi transfer complete interrupts
}

inline void Spi::setupPins()
{
	*pinDirectionRegister |= (0x1 << 2); // set Slave Select (SS) as output
	*pinDirectionRegister |= (0x1 << 3); // set Master Out Slave Input (MOSI) as output
	*pinDirectionRegister |= (0x1 << 5); // set Serial Clock as output (SCK)
}

/**
 * Sets the speed of the serial clock used to drive the SPI slave, by assigning
 * bit SPR0, SPR1 and SPI2X to form the entries in table 23-5 "Relationship
 * between SCK and Oscillator Frequency" at page 222 in the "Atmel-42735B-
 * ATmega328/P_Datasheet_Complete-11/2016" document.
 *
 *    Selection Number | SPI Clock Frequency
 *   ----------------------------------------
 *           0         |    Sys Freq / 4
 *           1         |    Sys Freq / 16
 *           2         |    Sys Freq / 64
 *           3         |    Sys Freq / 128
 *           4         |    Sys Freq / 2
 *           5         |    Sys Freq / 8
 *           6         |    Sys Freq / 32
 *           7         |    Sys Freq / 64
 *
 * @param clockSpeed  selected with a prescaler for the system clock.
 */
void Spi::setClockSpeed(SpiClockSpeed clockSpeed)
{
	u8 tableNumber = getClockSelectionNumber(clockSpeed);

	bool digit0 = static_cast<bool>(tableNumber & 0x1);
	bool digit1 = static_cast<bool>(tableNumber & 0x2);
	bool digit2 = static_cast<bool>(tableNumber & 0x4);

	writeSelectionNumToRegs(digit0, digit1, digit2);
}

inline u8 Spi::getClockSelectionNumber(SpiClockSpeed clockSpeed)
{
	switch(clockSpeed)
	{
		case (SpiClockSpeed::SysFreq_over_2):   return 4;
		case (SpiClockSpeed::SysFreq_over_4):   return 0;
		case (SpiClockSpeed::SysFreq_over_8):   return 5;
		case (SpiClockSpeed::SysFreq_over_16):  return 1;
		case (SpiClockSpeed::SysFreq_over_32):  return 6;
		case (SpiClockSpeed::SysFreq_over_64):  return 2;
		case (SpiClockSpeed::SysFreq_over_128): return 3;
	}

	return 0; // should never get here
}

inline void Spi::writeSelectionNumToRegs(bool digit0, bool digit1, bool digit2)
{
	/* Reset digit positions */
	*controlRegister &= ~((0x1 << SPR1) | (0x1 << SPR0));
	*statusRegister &= ~(0x1 << SPI2X);

	/* Write digits to registers */
	*controlRegister |= (digit1 << SPR1) | (digit0 << SPR0);
	*statusRegister |= digit2 << SPI2X;
}

/**
 * Used for testing.
 * @param reg  pointer to variable mocking the DDRB register.
 */
void Spi::setPinDirectionRegister(u8* reg)
{
	pinDirectionRegister = reg;
}

/**
 * Used for testing.
 * @param reg  pointer to variable mocking the SPCR register.
 */
void Spi::setControlRegister(u8* reg)
{
	controlRegister = reg;
}

/**
 * Used for testing.
 * @param reg  pointer to variable mocking the SPSR register.
 */
void Spi::setStatusRegister(u8* reg)
{
	statusRegister = reg;
}
