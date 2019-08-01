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
	Spi spi;
	u8 controlReg = 0;
	u8 pinDirectionReg = 0;
	u8 statusReg = 0;

	void SetUp()
	{
		spi.setControlRegister(&controlReg);
		spi.setPinDirectionRegister(&pinDirectionReg);
		spi.setStatusRegister(&statusReg);
	}

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

TEST_F(SpiTest, Spi_enabled_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlReg & (0x1 << SPE));
}

TEST_F(SpiTest, Peripheral_selected_as_master_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlReg & (0x1 << MSTR));
}

TEST_F(SpiTest, Transfer_complete_interrupts_enabled_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlReg & (0x1 << SPIE));
}

TEST_F(SpiTest, Master_Out_Slave_In_set_as_output_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(pinDirectionReg & (0x1 << MOSI_PIN_NUMBER));
}

TEST_F(SpiTest, Serial_Clock_set_as_output_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(pinDirectionReg & (0x1 << SCK_PIN_NUMBER));
}

TEST_F(SpiTest, Slave_Select_set_as_output_during_initialization)
{
	spi.initialize();
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