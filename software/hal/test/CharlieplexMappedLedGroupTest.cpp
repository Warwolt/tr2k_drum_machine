/*
****************************************************************************************************
* brief : Unit test for rotary encoder driver.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "CharlieplexMappedLedGroup.h"
#include "CharlieplexMatrix.h"
#include "IGpioPinMock.h"

using ::testing::Return;
using ::testing::AtLeast;

static constexpr u8 numLedsGroup1 = 3;
static constexpr u8 numLedsGroup2 = 2;
static constexpr u8 ledOffsetGroup1 = 0;
static constexpr u8 ledOffsetGroup2 = 3;

class CharlieplexMappedLedGroupTest : public ::testing::Test
{
public:
    /* Tests use at most 3 pins, if new tests changes this fact, change the
     * size of this array accordingly. */
    static constexpr u8 numPins = 3;
    GpioPinMock mockPins[numPins];

    /*
     *  State table with H = high, L = low and Z = tristate. The Z entries do
     *  not appear in the PinStatePair entries, but are implicitly Z since
     *  those are all pins that are neither H or L.
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

    /* Instantiation of driver using mock pins and the above look up table. The
     * led group leds {L0,L1,L2} is mapped onto the matrix leds {L0,L1,L2}. A
     * second led group has leds {L0, L1} mapped onto the matrix leds {L3, L4}.
     */
    CharlieplexMatrix<GpioPinMock> matrix =
        CharlieplexMatrix<GpioPinMock>(numPins, mockPins, threePinLut);

    CharlieplexMappedLedGroup<GpioPinMock> mappedLedGroup1 =
        CharlieplexMappedLedGroup<GpioPinMock>(numLedsGroup1, ledOffsetGroup1, matrix);

    CharlieplexMappedLedGroup<GpioPinMock> mappedLedGroup2 =
        CharlieplexMappedLedGroup<GpioPinMock>(numLedsGroup2, ledOffsetGroup2, matrix);

    LedGroup& ledGroup1 = mappedLedGroup1;
    LedGroup& ledGroup2 = mappedLedGroup2;

    void expectMatrixLedOn(u8 ledNum)
    {
        expectMatrixPinSetHigh(mockPins[threePinLut[ledNum].highPinNum]);
        expectMatrixPinSetLow(mockPins[threePinLut[ledNum].lowPinNum]);
    }

    void expectMatrixLedOff(u8 ledNum)
    {
        expectMatrixPinSetTristate(mockPins[threePinLut[ledNum].highPinNum]);
        expectMatrixPinSetTristate(mockPins[threePinLut[ledNum].lowPinNum]);
    }

    void expectMatrixLedOnAndOutputNextLedInRange(u8 ledNumLow, u8 ledNumHigh)
    {
        if(ledNumHigh < ledNumLow)
        {
            return;
        }

        for(int i = ledNumLow; i <= ledNumHigh; i++)
        {
            expectMatrixLedOn(i);
            matrix.outputNextLed();
        }
    }

    void expectMatrixLedOffAndOutputNextLedInRange(u8 ledNumLow, u8 ledNumHigh)
    {
        if(ledNumHigh < ledNumLow)
        {
            return;
        }

        for(int i = ledNumLow; i <= ledNumHigh; i++)
        {
            expectMatrixLedOff(i);
            matrix.outputNextLed();
        }
    }

    void expectMatrixPinSetHigh(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalOutput));
        EXPECT_CALL(pin, set());
    }

    void expectMatrixPinSetLow(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalOutput));
        EXPECT_CALL(pin, clear());
    }

    void expectMatrixPinSetTristate(GpioPinMock& pin)
    {
        EXPECT_CALL(pin, setDirection(DigitalInput)).Times(AtLeast(1));
    }
};

TEST_F(CharlieplexMappedLedGroupTest, Number_of_leds_is_calculated_correctly)
{
    EXPECT_EQ(numLedsGroup1, ledGroup1.getNumLeds());
}

TEST_F(CharlieplexMappedLedGroupTest, Setting_led_sets_corresponding_led_in_matrix)
{
    u8 ledNum = 2;
    ledGroup1.setLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup1 - 1);

    expectMatrixLedOn(ledNum + ledOffsetGroup1);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMappedLedGroupTest, Clearing_led_clears_corresponding_led_in_matrix)
{
    u8 ledNum = 1;
    ledGroup1.setLed(ledNum);
    ledGroup1.clearLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup1 - 1);

    expectMatrixLedOff(ledNum + ledOffsetGroup1);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMappedLedGroupTest, Toggling_led_toggles_corresponding_led_in_matrix)
{
    u8 ledNum = 2;
    ledGroup1.toggleLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup1 - 1);

    expectMatrixLedOn(ledNum + ledOffsetGroup1);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMappedLedGroupTest, Setting_led_sets_corresponding_led_in_matrix_group_2)
{
    u8 ledNum = 2;
    ledGroup2.setLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup2 - 1);

    expectMatrixLedOn(ledNum + ledOffsetGroup2);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMappedLedGroupTest, Clearing_led_clears_corresponding_led_in_matrix_group_2)
{
    u8 ledNum = 1;
    ledGroup2.setLed(ledNum);
    ledGroup2.clearLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup2 - 1);

    expectMatrixLedOff(ledNum + ledOffsetGroup2);
    matrix.outputNextLed();
}

TEST_F(CharlieplexMappedLedGroupTest, Toggling_led_toggles_corresponding_led_in_matrix_group_2)
{
    u8 ledNum = 2;
    ledGroup2.toggleLed(ledNum);
    expectMatrixLedOffAndOutputNextLedInRange(0, ledNum + ledOffsetGroup2 - 1);

    expectMatrixLedOn(ledNum + ledOffsetGroup2);
    matrix.outputNextLed();
}
