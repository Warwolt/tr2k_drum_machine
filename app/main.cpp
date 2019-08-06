#include "interrupts.h"
#include "spi.h"
#include "gpiopin.h"
#include "timer1.h"
#include "tempotimer16bit.h"
#include "tempotimingmanager.h"
#include <util/delay.h>

static Spi spi;
static Timer1 tim1;
static TempoTimer16Bit tempoTimer = TempoTimer16Bit(tim1);
static TempoTimingManager timingManager = TempoTimingManager(tempoTimer);
static GpioPin ledPin = GpioPin(Pin5, PortC, DigitalOutput);
static GpioPin dataLatchPin = GpioPin(Pin2, PortB, DigitalOutput);

static unsigned char segmentDataLut[] =
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};

void displayNumber(u16 number);
void displayDigit(u8 segmentData, u8 digitData);
void init();
void registerSpiInterrupt();
void registerTimerInterrupt();
void setupSpi();
void setupTempoTimer();
void setupTimingManager();

int main()
{
	init();

	while (1)
	{
		displayNumber(1337);
		_delay_us(2000);
	}
}

void displayNumber(u16 num)
{
	u16 digits[] = {num % 10, num/10 % 10, num/100 % 10, num/1000 % 10};
	for(int i = 0; i < 4; i++)
	{
		displayDigit(segmentDataLut[digits[i]], i);
		_delay_us(400);
	}
	displayDigit(0xFF, 5);
}

void displayDigit(u8 segmentData, u8 digitNum)
{
		r2k::vector<u8, 2> txBuffer = {segmentData, static_cast<u8>(0x1 << digitNum)};
		spi.setTxBuffer(txBuffer);
		spi.sendNextByteInBuffer();
}

void init()
{
	Interrupts::enableInterruptsGlobally();
	registerTimerInterrupt();
	setupTempoTimer();
	registerSpiInterrupt();
	setupSpi();
	setupTimingManager();
}

void registerSpiInterrupt()
{
	InterruptHandler spiTransferISR = []
	{
		if (!spi.txBufferIsEmpty())
		{
			spi.sendNextByteInBuffer();
		}
		else
		{
			dataLatchPin.set();
			dataLatchPin.clear();
		}
	};

	InterruptRequest spiIRQ = InterruptRequest::SpiTransferComplete;
	Interrupts::setHandlerForInterrupt(spiTransferISR, spiIRQ);
}

void registerTimerInterrupt()
{
	InterruptHandler timerISR = []
	{
		tempoTimer.countPulse();
	};
	InterruptRequest timerIRQ = InterruptRequest::Timer1CompareMatch;
	Interrupts::setHandlerForInterrupt(timerISR, timerIRQ);
}

void setupSpi()
{
	spi.setBitOrder(SpiBitOrder::MsbFirst);
	spi.setClockSpeed(SpiClockSpeed::SysFreq_over_128);
}

void setupTempoTimer()
{
	tim1.enablePeriodicInterrupts();
	tempoTimer.setTempo(BeatsPerMinute(120));
	// tempoTimer.start();
}

void setupTimingManager()
{
	static	u8 counter = 0;
	timingManager.addPlaybackStepHandler([]
	{
		if (counter == 2)
		{
			ledPin.toggle();
			counter = 0;
		}
		counter++;
	});
}
