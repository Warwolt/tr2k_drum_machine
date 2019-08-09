/*
****************************************************************************************************
* brief : Header file for general purpose input-output driver class
****************************************************************************************************
*/

#ifndef GPIO_PIN_H
#define GPIO_PIN_H

#include "igpiopin.h"
#include <stdio.h>
#include <avr/io.h>
#include "linuxtypes.h"

enum Port {PortB, PortC, PortD}; // note: PORT_A not supported in atmega328p
enum PinNumber {Pin0 = 0, Pin1, Pin2, Pin3, Pin4, Pin5, Pin6, Pin7, Pin8};

class GpioPin : public IGpioPin
{
public:
	GpioPin(PinNumber num, Port port);
	GpioPin(PinNumber num, Port port, DataDirection dir);

	void set();
	void clear();
	void toggle();
	void write(LogicState);
	LogicState read();

	void setDirection(DataDirection direction);
	void setInputRegister(u8 &regptr);
	void setOutputRegister(u8 &regptr);
	void setDataDirectionRegister(u8 &regptr);

private:
	volatile u8* getPortDataDirRegAddress(Port port);
	volatile u8* getInputRegAddress(Port port);
	volatile u8* getOutputRegAddress(Port port);

	DataDirection direction;
	unsigned int pinNum;
	volatile u8* inputRegPtr;
	volatile u8* outputRegPtr;
	volatile u8* dataDirRegPtr;
};

#endif /* GPIO_PIN_H */
