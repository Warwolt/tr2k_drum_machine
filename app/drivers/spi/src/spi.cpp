/*
****************************************************************************************************
* brief : Driver for the SPI peripheral.
****************************************************************************************************
*/

#include "spi.h"
#include <string.h>

/**
 * Constructor used in release build.
 */
Spi::Spi() : pinDirectionRegister(&DDRB), controlRegister(&SPCR), statusRegister(&SPSR),
	dataRegister(&SPDR)
{
	initialize();
}

/**
 *  Constructor used in unit tests.
 */
Spi::Spi(u8& pinDirReg, u8& controlReg, u8& statReg, u8& dataReg) :
	pinDirectionRegister(&pinDirReg), controlRegister(&controlReg), statusRegister(&statReg),
	dataRegister(&dataReg)
{
	initialize();
}

/**
 * Enables the SPI peripheral, enables "transfer complete"-interrupts, sets SPI
 * peripheral as master, and sets up MOSI, SCLK, and SS pins as outputs.
 */
inline void Spi::initialize()
{
	setupPins();
	enableInterrupts();
	useMasterMode();
	enableSpi();
}

inline void Spi::setupPins()
{
	*pinDirectionRegister |= (0x1 << 2); // set Slave Select (SS) as output
	*pinDirectionRegister |= (0x1 << 3); // set Master Out Slave Input (MOSI) as output
	*pinDirectionRegister |= (0x1 << 5); // set Serial Clock as output (SCK)
}

inline void Spi::enableInterrupts()
{
	*controlRegister |= (0x1 << SPIE); // enable interrupt on completed transfer
}

inline void Spi::useMasterMode()
{
	*controlRegister |= (0x1 << MSTR);
}

inline void Spi::enableSpi()
{
	*controlRegister |= (0x1 << SPE);
}

/**
 * Sets the speed of the serial clock used to drive the SPI slave by writing
 * a number from a table to designated register bits.
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

/**
 * The speed of the serial clock used to drive the SPI slave is set by
 * assigning bit SPR0, SPR1 and SPI2X to form the entries in table 23-5
 * "Relationship between SCK and Oscillator Frequency" at page 222 in the
 * "Atmel-42735B-ATmega328/P_Datasheet_Complete-11/2016" document.
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
inline u8 Spi::getClockSelectionNumber(SpiClockSpeed clockSpeed)
{
	switch (clockSpeed)
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

/**
 * Writes a three digit binary number to the appropriate places in memory
 * to set the corresponding Clock Speed Selection Number in the SPI peripheral.
 */
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
 * Determines what order to transmit bits, least significant bit first or
 * most significant bit first.
 * @param  enum determining LSB or MSB first.
 */
void Spi::setBitOrder(SpiBitOrder order)
{
	if (order == SpiBitOrder::LsbFirst)
	{
		*controlRegister |= 0x1 << DORD;
	}

	if (order == SpiBitOrder::MsbFirst)
	{
		*controlRegister &=  ~(0x1 << DORD);
	}
}

/**
 * Sends a single byte over SPI.
 * @param byte to transfer.
 */
void Spi::sendByte(u8 txByte)
{
	*dataRegister = txByte;
}

/**
 * Set a buffer of bytes to use for transferring data.
 *
 * Note Bene: Use the "txBufferIsEmpty"-method to check if more bytes need to
 * be transfered, and transfer them using the "sendNextBufferByte" method.
 *
 * @param buffer  reference to byte buffer to transfer.
 */
void Spi::setTxBuffer(r2k::ivector<u8> &buffer)
{
	memcpy(txBuffer.begin(), buffer.begin(), buffer.size());
	txBuffer.resize(buffer.size());
	txByteIndex = 0;
}

/**
 * Sends the next byte in the txBuffer and updates the byte index if buffer
 * none-empty, otherwise sends a zero.
 */
void Spi::sendNextByteInBuffer()
{
	if (!txBufferIsEmpty())
	{
		*dataRegister = txBuffer[txByteIndex++];
	}
	else
	{
		*dataRegister = 0;
	}
}

/**
 * @return true if all bytes in tx buffer have ben transferred
 */
bool Spi::txBufferIsEmpty()
{
	return (txByteIndex >= txBuffer.size());
}

/**
 * @return  true if transfer complete interrupt flag is set.
 */
bool Spi::transferIsComplete()
{
	return (*statusRegister & (0x1 << SPIF));
}
