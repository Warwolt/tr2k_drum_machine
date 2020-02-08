/*
****************************************************************************************************
* brief : Unit tests for r2k::function<> container template.
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "r2k/function.h"

static bool functionWasCalled;

class r2kFunctionTest : public ::testing::Test
{
public:
	void SetUp()
	{
		functionWasCalled = false;
	}

	void memberFunction()
	{
		functionWasCalled = true;
	}
};

static void freeFunction()
{
	functionWasCalled = true;
}

TEST_F(r2kFunctionTest, Free_function_can_be_wrapped_then_called)
{
	r2k::function<void()> func = freeFunction;
	func();
	EXPECT_TRUE(functionWasCalled);
}

TEST_F(r2kFunctionTest, Lambda_function_can_be_wrapped_then_called)
{
	r2k::function<void()> func = [](){ functionWasCalled = true; };
	func();
	EXPECT_TRUE(functionWasCalled);
}

TEST_F(r2kFunctionTest, Closure_function_can_be_wrapped_then_called)
{
	bool t = true;
	r2k::function<void()> func = [=](){ functionWasCalled = t; };
	func();
	EXPECT_TRUE(functionWasCalled);
}
