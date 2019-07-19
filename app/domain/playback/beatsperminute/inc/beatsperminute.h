/*
****************************************************************************************************
* brief : Value object class for Beats Per Minute unit
****************************************************************************************************
*/

#ifndef BEATS_PER_MINUTE_H
#define BEATS_PER_MINUTE_H

#include "linuxtypes.h"

class BeatsPerMinute
{
public:
	BeatsPerMinute() = default;
	BeatsPerMinute(u8 wholePart);
	BeatsPerMinute(u8 wholePart, u8 centiPart);
	BeatsPerMinute& operator=(const BeatsPerMinute &rhs);
	BeatsPerMinute& operator+=(const BeatsPerMinute &rhs);
	BeatsPerMinute& operator-=(const BeatsPerMinute &rhs);
	const BeatsPerMinute operator+(const BeatsPerMinute &other) const;
	const BeatsPerMinute operator-(const BeatsPerMinute &other) const;
	operator float() const { return value/100.0; };

private:
	u16 value; // bpm measured in 100ths
};

#endif /* BEATS_PER_MINUTE_H */
