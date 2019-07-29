/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#include "spi.h"

/**
 * Sets MOSI, SCLK, and SS pins as ooutputs.
 */
void Spi::initialize()
{
	*pinDirectionRegister |= (0x1 << 1); // set Slave Select (SS) as output
	*pinDirectionRegister |= (0x1 << 3); // set Master Out Slave Input (MOSI) as output
	*pinDirectionRegister |= (0x1 << 5); // set Serial Clock as output (SCK)
}

/**
 * Used for testing.
 * @param reg  pointer to variable mocking the DDRB register.
 */
void Spi::setPinDirectionRegister(u8* reg)
{
	pinDirectionRegister = reg;
}
