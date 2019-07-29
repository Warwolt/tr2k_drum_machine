/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#ifndef SPI_H
#define SPI_H

#include "linuxtypes.h"
#include <avr/io.h>

class Spi
{
public:
	void initialize();
	void setPinDirectionRegister(u8* reg);

private:
	u8* pinDirectionRegister;
};

#endif /* SPI_H */
