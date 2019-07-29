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
	void setControlRegister(u8* reg);
	void setPinDirectionRegister(u8* reg);

private:
	u8* pinDirectionRegister;
	u8* controlRegister;

	void enableInterrupts();
	void enableSpi();
	void useMasterMode();
	void setupPins();
};

#endif /* SPI_H */
