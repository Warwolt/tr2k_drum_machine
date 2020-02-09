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
};

static void freeFunction()
{
	functionWasCalled = true;
}

static void functionClient(r2k::function<void()> func)
{
	func();
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

TEST_F(r2kFunctionTest, Wrapped_free_function_can_be_used_as_callback)
{
	r2k::function<void()> func = freeFunction;
	functionClient(func);
	EXPECT_TRUE(functionWasCalled);
}

TEST_F(r2kFunctionTest, Wrapped_closure_can_be_used_as_callback)
{
	bool t = true;
	r2k::function<void()> func = [=](){ functionWasCalled = t; };
	functionClient(func);
	EXPECT_TRUE(functionWasCalled);
}
