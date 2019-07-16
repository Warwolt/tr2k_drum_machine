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
	void setInputRegister(uint8_t &regptr);
	void setOutputRegister(uint8_t &regptr);
	void setDataDirectionRegister(uint8_t &regptr);

private:
	volatile uint8_t* getPortDataDirRegAddress(Port port);
	volatile uint8_t* getInputRegAddress(Port port);
	volatile uint8_t* getOutputRegAddress(Port port);

	DataDirection direction;
	unsigned int pinNum;
	volatile uint8_t* inputRegPtr;
	volatile uint8_t* outputRegPtr;
	volatile uint8_t* dataDirRegPtr;
};

#endif /* GPIO_PIN_H */