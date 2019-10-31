/*
****************************************************************************************************
* brief : Unit test for charlieplex driver.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "igpiopin_mock.h"
#include "millisecond_timer_mock.h"
#include "gpio_matrix.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::AtLeast;
using milliseconds = MillisecondTimer::milliseconds;

static constexpr u8 numColumns = 4;
static constexpr u8 numRows = 3;
static constexpr milliseconds debounceTime = 50; // mlliseconds

struct DebouncableElement
{
    u8 num;
    u8 column;
    u8 row;
    milliseconds startTime;
};

class GpioMatrixTest : public ::testing::Test
{
public:
    /* This test uses a 3 by 4 matrix. Below is a depiction with c=column,
     * r=row, e=element.
     *
     *      [c0] [c1] [c2] [c3]
     * [r0]--e0---e1---e2---e3
     * [r1]--e4---e5---e6---e7
     * [r2]--e8---e9--e10--e11
     *
     */
    NiceMock<GpioPinMock> columnPins[numColumns];
    NiceMock<GpioPinMock> rowPins[numRows];
    NiceMock<MillisecondTimerMock> timer;
    GpioMatrix<GpioPinMock> matrix = GpioMatrix<GpioPinMock>(columnPins, numColumns, rowPins, numRows, timer, debounceTime);

    /*
     * First element is e1
     *      c0   c1   c2   c3
     *  r0--e0---e1---e2---e3
     *  r1--e4---e5--(E6)--e7
     *  r2--e8---e9--e10--e11
     */
    const DebouncableElement firstElement = {num: 1, column:1, row:0, startTime: 0};

    /*
     * Second element is e6
     *      c0   c1   c 2  c3
     *  r0--e0--(E1)--e2---e3
     *  r1--e4---e5---e6---e7
     *  r2--e8---e9--e10--e11
     */
    const DebouncableElement secondElement = {num: 6, column:2, row:1, startTime: firstElement.startTime + 20};

    u8 previousColumnNum = 0;
    void expectColumnSet(u8 columnNum)
    {
        EXPECT_CALL(columnPins[previousColumnNum], setDirection(DigitalInput)); // tristate mode
        EXPECT_CALL(columnPins[columnNum], setDirection(DigitalOutput));
        EXPECT_CALL(columnPins[columnNum], set());
        previousColumnNum = columnNum;
    }

    void expectRowRead(u8 columnNum)
    {
        EXPECT_CALL(rowPins[columnNum], read());
    }

    void expectRowReadAndReturn(u8 columnNum, LogicState state)
    {
        EXPECT_CALL(rowPins[columnNum], read()).WillOnce(Return(state));
    }

    void expectAllPinsSetToInputs()
    {
        for(int i = 0; i < numRows; i++)
            EXPECT_CALL(rowPins[i], setDirection(DigitalInput));

        for(int i = 0; i < numColumns; i++)
            EXPECT_CALL(columnPins[i], setDirection(DigitalInput));
    }

    void expectTimerReadAndReturn(u16 milliseconds)
    {
        EXPECT_CALL(timer, getCurrentTime()).WillOnce(Return(milliseconds));
    }
};

TEST_F(GpioMatrixTest, Matrix_can_return_number_of_input_elements)
{
    EXPECT_EQ(numColumns * numRows, matrix.getNumElements());
}

TEST_F(GpioMatrixTest, Constructor_sets_all_pins_as_inputs)
{
    expectAllPinsSetToInputs();
    GpioMatrix<GpioPinMock> matrix(columnPins, numColumns, rowPins, numRows, timer, debounceTime);
}

TEST_F(GpioMatrixTest, Reading_zeroth_element_sets_and_reads_corresponding_column_and_row)
{
    expectColumnSet(0);
    expectRowRead(0);
    matrix.readElement(0);
}

TEST_F(GpioMatrixTest, Reading_middle_element_sets_and_reads_corresponding_column_and_row)
{
    expectColumnSet(2);
    expectRowRead(1);
    matrix.readElement(6);
}

TEST_F(GpioMatrixTest, Reading_laste_element_sets_and_reads_corresponding_column_and_row)
{
    expectColumnSet(3);
    expectRowRead(2);
    matrix.readElement(11);
}

TEST_F(GpioMatrixTest, Element_state_initially_low_before_debounced)
{
    expectColumnSet(2);
    expectRowReadAndReturn(1, LogicState::High); // e.g. button pressed connect column to row
    expectTimerReadAndReturn(0); // no milliseconds will have elapsed
    EXPECT_EQ(LogicState::Low, matrix.readElement(6)); // but not debounced yet so expect low state
}

TEST_F(GpioMatrixTest, Element_state_can_change_to_high_after_debounce_time)
{
    /* First read will trigger debounce timer */
    expectColumnSet(2);
    expectRowReadAndReturn(1, LogicState::High);
    expectTimerReadAndReturn(0);
    EXPECT_EQ(LogicState::Low, matrix.readElement(6));

    /* Second read after debounce time elapsed */
    expectColumnSet(2);
    expectRowReadAndReturn(1, LogicState::High);
    expectTimerReadAndReturn(debounceTime);
    EXPECT_EQ(LogicState::High, matrix.readElement(6));
}

TEST_F(GpioMatrixTest, Element_must_remain_stable_for_the_whole_debounce_period)
{
    /* E.g. Button is pressed but not yet debounced */
    expectColumnSet(3);
    expectRowReadAndReturn(2, LogicState::High);
    expectTimerReadAndReturn(0);
    EXPECT_EQ(LogicState::Low, matrix.readElement(11));

    /* State is not stable so timer is reset */
    expectColumnSet(3);
    expectRowReadAndReturn(2, LogicState::Low);
    expectTimerReadAndReturn(25);
    EXPECT_EQ(LogicState::Low, matrix.readElement(11));

    /* First read with stabilizing element state */
    expectColumnSet(3);
    expectRowReadAndReturn(2, LogicState::High);
    expectTimerReadAndReturn(25 + 10);
    EXPECT_EQ(LogicState::Low, matrix.readElement(11));

    /* second read, state now stable, expect to read debounced value */
    expectColumnSet(3);
    expectRowReadAndReturn(2, LogicState::High);
    expectTimerReadAndReturn(25 + 10 + debounceTime);
    EXPECT_EQ(LogicState::High, matrix.readElement(11));
}

TEST_F(GpioMatrixTest, One_element_can_stabilize_while_a_second_is_unstable)
{
    /* First read of first element will trigger debounce timer for that first element */
    expectColumnSet(firstElement.column);
    expectRowReadAndReturn(firstElement.row, LogicState::High);
    expectTimerReadAndReturn(firstElement.startTime);
    EXPECT_EQ(LogicState::Low, matrix.readElement(firstElement.num));

    /* First read of second element will trigger debounce timer for that second element */
    expectColumnSet(secondElement.column);
    expectRowReadAndReturn(secondElement.row, LogicState::High);
    expectTimerReadAndReturn(secondElement.startTime);
    EXPECT_EQ(LogicState::Low, matrix.readElement(secondElement.num));

    /* Second read of first element is low and should retrigger its debounce timer */
    expectColumnSet(firstElement.column);
    expectRowReadAndReturn(firstElement.row, LogicState::Low);
    expectTimerReadAndReturn(firstElement.startTime + debounceTime);
    EXPECT_EQ(LogicState::Low, matrix.readElement(firstElement.num));

    /* Second read of second element after debounce time elapsed */
    expectColumnSet(secondElement.column);
    expectRowReadAndReturn(secondElement.row, LogicState::High);
    expectTimerReadAndReturn(secondElement.startTime + debounceTime);
    EXPECT_EQ(LogicState::High, matrix.readElement(secondElement.num));
}

TEST_F(GpioMatrixTest, Two_elements_can_be_debounced_to_high_state_at_the_same_time)
{
    /* First read of first element will trigger debounce timer for that first element */
    expectColumnSet(firstElement.column);
    expectRowReadAndReturn(firstElement.row, LogicState::High);
    expectTimerReadAndReturn(firstElement.startTime);
    EXPECT_EQ(LogicState::Low, matrix.readElement(firstElement.num));

    /* First read of second element will trigger debounce timer for that second element */
    expectColumnSet(secondElement.column);
    expectRowReadAndReturn(secondElement.row, LogicState::High);
    expectTimerReadAndReturn(secondElement.startTime);
    EXPECT_EQ(LogicState::Low, matrix.readElement(secondElement.num));

    /* Second read of first element after debounce time elapsed */
    expectColumnSet(firstElement.column);
    expectRowReadAndReturn(firstElement.row, LogicState::High);
    expectTimerReadAndReturn(firstElement.startTime + debounceTime);
    EXPECT_EQ(LogicState::High, matrix.readElement(firstElement.num));

    /* Second read of second element after debounce time elapsed */
    expectColumnSet(secondElement.column);
    expectRowReadAndReturn(secondElement.row, LogicState::High);
    expectTimerReadAndReturn(secondElement.startTime + debounceTime);
    EXPECT_EQ(LogicState::High, matrix.readElement(secondElement.num));
}
