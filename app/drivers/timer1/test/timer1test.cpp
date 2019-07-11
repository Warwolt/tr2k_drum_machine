/*
****************************************************************************************************
* brief : Unit test for "timer1" 16-bit timer driver code
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "timer1.h"
#include "traceprint.h"

class Timer1Test : public ::testing::Test
{
public:
	Timer1 timer;
};

TEST_F(Timer1Test, Hello)
{
	FAIL() << ERROR_MSG_STR("Write some trial code in main before writing unit tests");
}