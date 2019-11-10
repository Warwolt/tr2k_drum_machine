/*
****************************************************************************************************
* brief : Mock implementation of gpio pin interface
****************************************************************************************************
*/

#ifndef I_GPIO_PIN_MOCK_H
#define I_GPIO_PIN_MOCK_H

#include "gmock/gmock.h"
#include "linuxtypes.h"
#include "IGpioPin.h"

class GpioPinMock : public IGpioPin
{
public:
	MOCK_METHOD0(set, void());
	MOCK_METHOD0(clear, void());
	MOCK_METHOD0(toggle, void());
	MOCK_METHOD1(write, void(LogicState));
	MOCK_METHOD0(read, LogicState());
	MOCK_METHOD1(setDirection, void(DataDirection direction));
};

#endif /* I_GPIO_PIN_MOCK_H */
