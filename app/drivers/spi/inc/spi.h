/*
****************************************************************************************************
* brief : Driver for the SPI peripheral
****************************************************************************************************
*/

#ifndef SPI_H
#define SPI_H

#include "linuxtypes.h"
#include <avr/io.h>
#include <stddef.h>
#include "r2k/vector.h"

enum class SpiBitOrder {LsbFirst, MsbFirst};
enum class SpiClockSpeed {SysFreq_over_2, SysFreq_over_4, SysFreq_over_8, SysFreq_over_16,
	SysFreq_over_32, SysFreq_over_64, SysFreq_over_128};

class Spi
{
public:
	Spi();
	Spi(u8 &pinDirReg, u8 &controlReg, u8 &statReg, u8 &dataReg);

	void setClockSpeed(SpiClockSpeed);
	void setBitOrder(SpiBitOrder);

	void sendByte(u8 txByte);
	bool transferIsComplete();

	void setTxBuffer(r2k::ivector<u8> &buffer);
	void sendNextByteInBuffer();
	bool txBufferIsEmpty();

private:
	volatile u8* pinDirectionRegister;
	volatile u8* controlRegister;
	volatile u8* statusRegister;
	volatile u8* dataRegister;

	r2k::vector<u8, 255> txBuffer;
	u8 txByteIndex = 0;

	void initialize();
	void setupPins();
	void enableInterrupts();
	void useMasterMode();
	void enableSpi();

	u8 getClockSelectionNumber(SpiClockSpeed clockSpeed);
	void writeSelectionNumToRegs(bool digit0, bool digit1, bool digit2);
};

#endif /* SPI_H */
