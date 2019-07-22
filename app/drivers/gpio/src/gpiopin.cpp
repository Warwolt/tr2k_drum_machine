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
	// no PortA supported in atmega328
	if (port == PortB) return &DDRB;
	if (port == PortC) return &DDRC;
	if (port == PortD) return &DDRD;
	else return 0;
}

volatile uint8_t* GpioPin::getInputRegAddress(Port port)
{
	// no PortA supported in atmega328
	if (port == PortB) return &PINB;
	if (port == PortC) return &PINC;
	if (port == PortD) return &PIND;
	else return 0;
}

volatile uint8_t* GpioPin::getOutputRegAddress(Port port)
{
	// no PortA supported in atmega328
	if (port == PortB) return &PORTB;
	if (port == PortC) return &PORTC;
	if (port == PortD) return &PORTD;
	else return 0;
}

/* Input / Output methods ------------------------------------------------------------------------*/
void GpioPin::set()
{
	if (direction == DigitalOutput)
		(*outputRegPtr) |= 0x1 << pinNum;
}

void GpioPin::clear()
{
	(*outputRegPtr) &= ~(0x1 << pinNum);
}

void GpioPin::toggle()
{
	if (direction == DigitalOutput)
		(*outputRegPtr) ^= 0x1 << pinNum;
}

void GpioPin::write(LogicState state)
{
	(state == LogicHigh) ? set() : clear();
}

LogicState GpioPin::read()
{
	bool bitHigh = *(inputRegPtr) & (0x1 << pinNum);
	return bitHigh ? LogicHigh : LogicLow;
}

/* Setter methods --------------------------------------------------------------------------------*/
void GpioPin::setDirection(DataDirection dir)
{
	direction = dir;

	if (direction == DigitalInput)
	{
		(*dataDirRegPtr) &= ~(0x1 << pinNum); // set data direction bit to input
		(*outputRegPtr) &= ~(0x1 << pinNum);  // clear pullup bit
	}

	if (direction == DigitalOutput)
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