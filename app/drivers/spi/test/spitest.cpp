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
	u8 controlRegister = 0;
	u8 pinDirectionRegister = 0;

	void SetUp()
	{
		spi.setControlRegister(&controlRegister);
		spi.setPinDirectionRegister(&pinDirectionRegister);
	}
};

TEST_F(SpiTest, Spi_enabled_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlRegister & (0x1 << SPE));
}

TEST_F(SpiTest, Peripheral_selected_as_master_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlRegister & (0x1 << MSTR));
}

TEST_F(SpiTest, Transfer_complete_interrupts_enabled_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(controlRegister & (0x1 << SPIE));
}

TEST_F(SpiTest, Master_Out_Slave_In_set_as_output_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(pinDirectionRegister & (0x1 << MOSI_PIN_NUMBER));
}

TEST_F(SpiTest, Serial_Clock_set_as_output_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(pinDirectionRegister & (0x1 << SCK_PIN_NUMBER));
}

TEST_F(SpiTest, Slave_Select_set_as_output_during_initialization)
{
	spi.initialize();
	EXPECT_TRUE(pinDirectionRegister & (0x1 << SS_PIN_NUMBER));
}
