/*
****************************************************************************************************
* brief : Interface class for 8-bit timer peripherals
****************************************************************************************************
*/

#ifndef TIMER_8_BIT_H
#define TIMER_8_BIT_H

#include "linuxtypes.h"

class Timer8Bit
{
public:
	enum class PrescaleOption {_1, _8, _64, _256, _1024};

	virtual void setPrescaler(PrescaleOption prescaler) = 0;
	virtual void setPeriod(u8 period) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void clear() = 0;
};

#endif /* TIMER_8_BIT_H */
