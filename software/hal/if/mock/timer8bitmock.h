/*
****************************************************************************************************
* brief : Mock implementation of Timer8Bit interface
****************************************************************************************************
*/

#ifndef TIMER_8_BIT_MOCK_H
#define TIMER_8_BIT_MOCK_H

#include "gmock/gmock.h"
#include "timer8bit.h"
#include "linuxtypes.h"

class Timer8BitMock : public Timer8Bit
{
public:
	MOCK_METHOD1(setPrescaler, void(Timer8Bit::PrescaleOption prescaler));
	MOCK_METHOD1(setPeriod, void(u8 period));
	MOCK_METHOD0(start, void());
	MOCK_METHOD0(stop,  void());
	MOCK_METHOD0(clear, void());
};

#endif /* TIMER_8_BIT_MOCK_H */
