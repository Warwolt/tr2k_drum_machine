/*
****************************************************************************************************
* brief : Unit test for serial peripheral interface (SPI) driver code
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "spi.h"

constexpr u8 MOSI_PIN_NUMBER = 3;
constexpr u8 SCK_PIN_NUMBER = 5;
constexpr u8 SS_PIN_NUMBER = 2;

class SpiTest : public ::testing::Test
{
public:
	u8 pinDirectionReg = 0;
	u8 controlReg = 0;
	u8 statusReg = 0;
	u8 dataReg = 0;
	Spi spi = Spi(pinDirectionReg, controlReg, statusReg, dataReg);

	/**
	 * Combines bits SPR0, SPR1 and SPI2X to form the entries in table 23-5
	 * "Relationship between SCK and Oscillator Frequency" at page 222 in
	 * the "Atmel-42735B-ATmega328/P_Datasheet_Complete-11/2016" document.
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
	 */
	u8 getClockSpeedFromRegisters()
	{
		u8 digit0 = ((controlReg >> SPR0) & 0x1) << 0;
		u8 digit1 = ((controlReg >> SPR1) & 0x1) << 1;
		u8 digit2 = ((statusReg >> SPI2X) & 0x1) << 2;

		return digit2 | digit1 | digit0;
	}
};

TEST_F(SpiTest, Spi_enabled_during_construction)
{
	EXPECT_TRUE(controlReg & (0x1 << SPE));
}

TEST_F(SpiTest, Peripheral_selected_as_master_during_construction)
{
	EXPECT_TRUE(controlReg & (0x1 << MSTR));
}

TEST_F(SpiTest, Transfer_complete_interrupts_enabled_during_construction)
{
	EXPECT_TRUE(controlReg & (0x1 << SPIE));
}

TEST_F(SpiTest, Master_Out_Slave_In_set_as_output_during_construction)
{
	EXPECT_TRUE(pinDirectionReg & (0x1 << MOSI_PIN_NUMBER));
}

TEST_F(SpiTest, Serial_Clock_set_as_output_during_construction)
{
	EXPECT_TRUE(pinDirectionReg & (0x1 << SCK_PIN_NUMBER));
}

TEST_F(SpiTest, Slave_Select_set_as_output_during_construction)
{
	EXPECT_TRUE(pinDirectionReg & (0x1 << SS_PIN_NUMBER));
}

TEST_F(SpiTest, Spi_clock_speed_can_be_set)
{
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_4);
	EXPECT_EQ(0, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_16);
	EXPECT_EQ(1, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_64);
	EXPECT_EQ(2, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_128);
	EXPECT_EQ(3, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_2);
	EXPECT_EQ(4, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_8);
	EXPECT_EQ(5, getClockSpeedFromRegisters());

	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_32);
	EXPECT_EQ(6, getClockSpeedFromRegisters());
}

TEST_F(SpiTest, Bit_order_can_be_set_to_least_significant_bit_first)
{
	spi.setBitOrder(SpiBitOrder::LsbFirst);
	EXPECT_TRUE(controlReg & (0x1 << DORD));
}

TEST_F(SpiTest, Bit_order_can_be_set_to_most_significant_bit_first)
{
	spi.setBitOrder(SpiBitOrder::MsbFirst);
	EXPECT_FALSE(controlReg & (0x1 << DORD));
}

TEST_F(SpiTest, Single_byte_can_be_transferred)
{
	u8 txByte = 123;
	spi.sendByte(txByte);
	EXPECT_EQ(txByte, dataReg);
}

TEST_F(SpiTest, Transfer_complete_false_if_corresponding_flag_not_set)
{
	statusReg &= ~(0x1 << SPIF);
	EXPECT_FALSE(spi.transferIsComplete());
}

TEST_F(SpiTest, Transfer_complete_true_if_corresponding_flag_is_set)
{
	statusReg |= (0x1 << SPIF);
	EXPECT_TRUE(spi.transferIsComplete());
}

TEST_F(SpiTest, Byte_buffer_can_be_transferred)
{
	r2k::vector<u8, 2> txBuffer = {222, 233};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();

	EXPECT_EQ(txBuffer[0], dataReg);
}

TEST_F(SpiTest, Buffer_contents_sent_byte_for_byte)
{
	r2k::vector<u8, 2> txBuffer = {11, 22};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();
	spi.sendNextByteInBuffer();

	EXPECT_EQ(txBuffer[1], dataReg);
}

TEST_F(SpiTest, Buffer_not_empty_when_bytes_remain_unsent)
{
	r2k::vector<u8, 2> txBuffer = {10, 20};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();

	EXPECT_FALSE(spi.txBufferIsEmpty());
}

TEST_F(SpiTest, Buffer_empty_after_all_bytes_sent)
{
	r2k::vector<u8, 2> txBuffer = {123, 234};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();
	spi.sendNextByteInBuffer();

	EXPECT_TRUE(spi.txBufferIsEmpty());
}

TEST_F(SpiTest, Sends_zero_if_trying_to_send_byte_in_empty_buffer)
{
	r2k::vector<u8, 2> txBuffer = {1, 2};
	spi.setTxBuffer(txBuffer);

	spi.sendNextByteInBuffer();
	spi.sendNextByteInBuffer();
	spi.sendNextByteInBuffer();

	EXPECT_EQ(0, dataReg);
}

TEST_F(SpiTest, Client_can_transfer_byte_buffer_by_combing_spi_driver_methods)
{
	u8 byteIndex = 0;
	r2k::vector<u8, 3> txBuffer = {12, 34, 56};
	spi.setTxBuffer(txBuffer);

	while(!spi.txBufferIsEmpty())
	{
		spi.sendNextByteInBuffer();
		ASSERT_EQ(txBuffer[byteIndex++], dataReg);
	}

	EXPECT_EQ(byteIndex, txBuffer.size());
}
