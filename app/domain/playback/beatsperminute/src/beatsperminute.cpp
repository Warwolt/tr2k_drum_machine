/*
****************************************************************************************************
* brief : Value object class for Beats Per Minute unit
****************************************************************************************************
*/

#include "beatsperminute.h"

BeatsPerMinute::BeatsPerMinute(u8 wholePart, u8 centiPart)
{
	value = wholePart * 100 + centiPart;
}

BeatsPerMinute& BeatsPerMinute::operator=(const BeatsPerMinute &rhs)
{
	value = rhs.value;
	return *this;
}

BeatsPerMinute& BeatsPerMinute::operator+=(const BeatsPerMinute &rhs)
{
	value += rhs.value;
	return *this;
}

BeatsPerMinute& BeatsPerMinute::operator-=(const BeatsPerMinute &rhs)
{
	value -= rhs.value;
	return *this;
}

const BeatsPerMinute BeatsPerMinute::operator-(const BeatsPerMinute &other) const
{
	BeatsPerMinute ret = *this;
	ret -= other;
	return ret;
}

const BeatsPerMinute BeatsPerMinute::operator+(const BeatsPerMinute &other) const
{
	BeatsPerMinute ret = *this;
	ret += other;
	return ret;
}

Fix16 BeatsPerMinute::getAsFix16()
{
	return Fix16(value) / Fix16(100);
}
