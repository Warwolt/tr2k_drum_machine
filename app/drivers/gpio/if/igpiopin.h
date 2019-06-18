/*
****************************************************************************************************
* brief : Interface for gpio pin purely for mocking, use templating in production code.
****************************************************************************************************
*/

#ifndef I_GPIO_PIN_H
#define I_GPIO_PIN_H

#include <avr/io.h>

enum LogicState {LOGIC_LOW = false, LOGIC_HIGH = true};
enum DataDirection {DIGITAL_INPUT, DIGITAL_OUTPUT};

class IGpioPin
{
public:
	virtual ~IGpioPin() {}

	virtual void set() = 0;
	virtual void clear() = 0;
	virtual void toggle() = 0;
	virtual void write(LogicState) = 0;
	virtual LogicState read() = 0;
	virtual void setDirection(DataDirection direction) = 0;
};

#endif /* I_GPIO_PIN_H */