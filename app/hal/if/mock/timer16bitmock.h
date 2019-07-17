/*
****************************************************************************************************
* brief : Mock implementation of Timer16Bit interface
****************************************************************************************************
*/

#ifndef TIMER_16_BIT_MOCK_H
#define TIMER_16_BIT_MOCK_H

#include "gmock/gmock.h"
#include "timer16bit.h"
#include "linuxtypes.h"

class Timer16BitMock : public Timer16Bit
{
public:
	MOCK_METHOD1(setPrescaler, void(Timer16Bit::PrescaleOption prescaler));
	MOCK_METHOD1(setPeriod, void(u16 period));
	MOCK_METHOD0(start, void());
	MOCK_METHOD0(stop,  void());
	MOCK_METHOD0(clear, void());
};

#endif /* TIMER_16_BIT_MOCK_H */
