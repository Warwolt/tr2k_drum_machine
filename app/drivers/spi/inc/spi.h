/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#ifndef SPI_H
#define SPI_H

#include "linuxtypes.h"
#include <avr/io.h>

enum class SpiClockSpeed {SysFreq_over_2, SysFreq_over_4, SysFreq_over_8, SysFreq_over_16,
	SysFreq_over_32, SysFreq_over_64, SysFreq_over_128};

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

	u8 getClockSelectionNumber(SpiClockSpeed clockSpeed);
	void writeSelectionNumToRegs(bool digit0, bool digit1, bool digit2);
};

#endif /* SPI_H */
