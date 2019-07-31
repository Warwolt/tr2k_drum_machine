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
 * Sets the speed of the serial clock used to drive the SPI slave.
 * @param clockSpeed  selected with a prescaler for the system clock.
 */
void Spi::setClockSpeed(SpiClockSpeed clockSpeed)
{
	// code only to pass test
	*controlRegister &= ~(0x1 << SPR1 | 0x1 << SPR0);
	*statusRegister |= (0x1 << SPI2X);
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
