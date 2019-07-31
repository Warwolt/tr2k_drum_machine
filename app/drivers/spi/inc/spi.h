/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#ifndef SPI_H
#define SPI_H

#include "linuxtypes.h"
#include <avr/io.h>

enum class SpiClockSpeed {SysFreq_over_2};

class Spi
{
public:
	void initialize();
	void setClockSpeed(SpiClockSpeed);

	void setControlRegister(u8* reg);
	void setPinDirectionRegister(u8* reg);
	void setStatusRegister(u8* reg);

private:
	volatile u8* pinDirectionRegister;
	volatile u8* controlRegister;
	volatile u8* statusRegister;

	void enableInterrupts();
	void enableSpi();
	void useMasterMode();
	void setupPins();
};

#endif /* SPI_H */
