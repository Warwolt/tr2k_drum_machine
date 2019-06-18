/*
****************************************************************************************************
* brief : Implementation of general purpose input-output driver
****************************************************************************************************
*/

#include "gpiopin.h"

/* Constructor -----------------------------------------------------------------------------------*/
GpioPin::GpioPin(PinNumber num, Port port)
{
	pinNum = static_cast<int>(num);
	dataDirRegPtr = getPortDataDirRegAddress(port);
	inputRegPtr = getInputRegAddress(port);
	outputRegPtr = getOutputRegAddress(port);
}

GpioPin::GpioPin(PinNumber num, Port port, DataDirection direction)
{
	pinNum = static_cast<int>(num);
	dataDirRegPtr = getPortDataDirRegAddress(port);
	inputRegPtr = getInputRegAddress(port);
	outputRegPtr = getOutputRegAddress(port);
	setDirection(direction);
}

volatile uint8_t* GpioPin::getPortDataDirRegAddress(Port port)
{
	// no PORT_A supported in atmega328
	if(port == PORT_B) return &DDRB;
	if(port == PORT_C) return &DDRC;
	if(port == PORT_D) return &DDRD;
	else return 0;
}

volatile uint8_t* GpioPin::getInputRegAddress(Port port)
{
	// no PORT_A supported in atmega328
	if(port == PORT_B) return &PINB;
	if(port == PORT_C) return &PINC;
	if(port == PORT_D) return &PIND;
	else return 0;
}

volatile uint8_t* GpioPin::getOutputRegAddress(Port port)
{
	// no PORT_A supported in atmega328
	if(port == PORT_B) return &PORTB;
	if(port == PORT_C) return &PORTC;
	if(port == PORT_D) return &PORTD;
	else return 0;
}

/* Input / Output methods ------------------------------------------------------------------------*/
void GpioPin::set()
{
	if(direction == DIGITAL_OUTPUT)
		(*outputRegPtr) |= 0x1 << pinNum;
}

void GpioPin::clear()
{
	(*outputRegPtr) &= ~(0x1 << pinNum);
}

void GpioPin::toggle()
{
	if(direction == DIGITAL_OUTPUT)
		(*outputRegPtr) ^= 0x1 << pinNum;
}

void GpioPin::write(LogicState state)
{
	(state == LOGIC_HIGH) ? set() : clear();
}

LogicState GpioPin::read()
{
	bool bitHigh = *(inputRegPtr) & (0x1 << pinNum);
	return bitHigh ? LOGIC_HIGH : LOGIC_LOW;
}

/* Setter methods --------------------------------------------------------------------------------*/
void GpioPin::setDirection(DataDirection dir)
{
	direction = dir;

	if(direction == DIGITAL_INPUT)
	{
		(*dataDirRegPtr) &= ~(0x1 << pinNum); // set data direction bit to input
		(*outputRegPtr) &= ~(0x1 << pinNum);  // clear pullup bit
	}

	if(direction == DIGITAL_OUTPUT)
	{
		(*dataDirRegPtr) |= 0x1 << pinNum;
	}
}

void GpioPin::setInputRegister(uint8_t &regptr)
{
	inputRegPtr = &regptr;
}

void GpioPin::setOutputRegister(uint8_t &regptr)
{
	outputRegPtr = &regptr;
}

void GpioPin::setDataDirectionRegister(uint8_t &regptr)
{
	dataDirRegPtr = &regptr;
}