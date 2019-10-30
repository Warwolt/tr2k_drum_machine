/*
****************************************************************************************************
* brief : Unit test for charlieplex driver.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "charlieplex_matrix.h"
#include "igpiopin_mock.h"

using ::testing::Return;
using ::testing::AtLeast;

class CharlieplexMatrixTest : public ::testing::Test
{
public:
    /* Tests use at most 4 pins, if new tests changes this fact, change the
     * size of this array accordingly. */
    GpioPinMock mockPins[4];

    /*
     *  The table below is encoded as an array of pairs, where the pair
     *  consists of the index of the high pin and the index of the low pin.
     *
     *   LedNum   Pin0    Pin1
     *   ------  ------  ------
     *     0       H       L
     *     1       L       H
     */
    PinStatePair twoPinLut[2] = {{0,1},{1,0}};

    /*
     *  Variation of the first table, but with tristate introduced (Z). The Z
     *  entries do not appear in the LUT pairs, but are implicitly Z since
     *  those pins are neither H or L.
     *
     *   LedNum   Pin0    Pin1    Pin2
     *   ------  ------  ------  ------
     *     0       H       L       Z
     *     1       L       H       Z
     *     2       Z       H       L
     *     3       Z       L       H
     *     4       H       Z       L
     *     5       L       Z       H
     */
    PinStatePair threePinLut[6] = {{0,1},{1,0},{1,2},{2,1},{0,2},{2,0}};
    /*
     *  Four pin variation of the above table.
     *
     *   LedNum   Pin0    Pin1    Pin2    Pin3
     *   ------  ------  ------  ------  ------
     *     0       H       L       Z       Z
     *     1       L       H       Z       Z
     *     2       Z       H       L       Z
     *     3       Z       L       H       Z
     *     4       H       Z       L       Z
     *     5       L       Z       H       Z
     *     6       Z       Z       H       L
     *     7       Z       Z       L       H
     *     8       Z       H       Z       L
     *     9       Z       L       Z       H
     *     10      H       Z       Z       L
     *     11      L       Z       Z       H
     */
    PinStatePair fourPinLut[12] = {
        {0,1},{1,0},{1,2},{2,1},{0,2},{2,0},{2,3},{3,2},{1,3},{3,1},{0,3},{3,0}};


    void expectLedOn(u8 ledNum, PinStatePair* lookUpTable)
    {
        expectSetHigh(mockPins[lookUpTable[ledNum].highPinNum]);
        expectSetLow(mockPins[lookUpTable[ledNum].lowPinNum]);
    }

    void expectLedOff(u8 ledNum, PinStatePair* lookUpTable)
    {
        expectSetTristate(mockPins[lookUpTable[ledNum].highPinNum]);
        expectSetTristate(mockPins[lookUpTable[ledNum].lowPinNum]);
    }

    void expectSetHigh(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalOutput));
        EXPECT_CALL(pin, set());
    }

    void expectSetLow(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalOutput));
        EXPECT_CALL(pin, clear());
    }

    void expectSetTristate(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalInput)).Times(AtLeast(1));
    }
};

TEST_F(CharlieplexMatrixTest, Number_of_leds_is_calculated_correctly)
{
    u8 numPins = 2;
    CharlieplexMatrix<GpioPinMock> matrix(numPins, mockPins, twoPinLut);
    EXPECT_EQ(numPins * (numPins-1), matrix.getNumLeds());
}

TEST_F(CharlieplexMatrixTest, All_leds_are_initially_off)
{
    CharlieplexMatrix<GpioPinMock> matrix(3, mockPins, threePinLut);
    for(int ledNum = 0; ledNum < matrix.getNumLeds(); ledNum++)
    {
        /* Expect both pins to be set to tristate */
        expectLedOff(ledNum, threePinLut);

        /* Outputing disabled led */
        matrix.outputNextLed();
    }
}

TEST_F(CharlieplexMatrixTest, Led_can_be_turned_on)
{
    CharlieplexMatrix<GpioPinMock> matrix(2, mockPins, twoPinLut);
    matrix.setLed(0);

    /* Led 1 should be turned on */
    expectLedOn(0, twoPinLut);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMatrixTest, Led_can_be_turned_off)
{
    CharlieplexMatrix<GpioPinMock> matrix(2, mockPins, twoPinLut);
    matrix.setLed(0);
    matrix.clearLed(0);

    expectLedOff(0, twoPinLut);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMatrixTest, Led_state_can_be_toggled)
{
    CharlieplexMatrix<GpioPinMock> matrix(2, mockPins, twoPinLut);
    matrix.toggleLed(0);

    expectLedOn(0, twoPinLut);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMatrixTest, Outputting_led_turns_off_previous_led_if_it_was_on)
{
    CharlieplexMatrix<GpioPinMock> matrix(2, mockPins, twoPinLut);
    matrix.setLed(0);
    matrix.setLed(1);

    /* Led 0 should be turned on, and previous led (led 1) turned off  */
    expectLedOff(1, twoPinLut);
    expectLedOn(0, twoPinLut);
    matrix.outputNextLed();

    /* Led 1 should be turned on, and previous led (led 0) turned off */
    expectLedOff(0, twoPinLut);
    expectLedOn(1, twoPinLut);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMatrixTest, Outputting_leds_loops_around_after_last_led)
{
    CharlieplexMatrix<GpioPinMock> matrix(2, mockPins, twoPinLut);
    matrix.setLed(0);
    matrix.setLed(1);

    for(int i = 0; i < 2; i++)
    {
        /* Led 0 should be turned on, and previous led (led 1) turned off  */
        expectLedOff(1, twoPinLut);
        expectLedOn(0, twoPinLut);
        matrix.outputNextLed();

        /* Led 1 should be turned on, and previous led (led 0) turned off */
        expectLedOff(0, twoPinLut);
        expectLedOn(1, twoPinLut);
        matrix.outputNextLed();
    }
}
