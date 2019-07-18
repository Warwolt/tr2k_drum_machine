/*
****************************************************************************************************
* brief : Unit tests for BeatsPerMinute datatype
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "beatsperminute.h"
#include "fixmath/fixmath.h"

static const float precision = 0.01; // precise to 100ths place

class TestBeatsPerMinute : public ::testing::Test
{
public:

};

TEST_F(TestBeatsPerMinute, Bpm_value_can_be_accessed_as_fixed_point_number)
{
	BeatsPerMinute bpm(128,50);
	EXPECT_NEAR(128.50, bpm.getAsFix16(), precision);
}

TEST_F(TestBeatsPerMinute, Bpm_values_can_be_summed)
{
	BeatsPerMinute sum = BeatsPerMinute(128, 50) + BeatsPerMinute(0, 50);
	EXPECT_NEAR(129.0, sum.getAsFix16(), precision);
}

TEST_F(TestBeatsPerMinute, Bpm_values_can_be_subtracted)
{
	BeatsPerMinute sum = BeatsPerMinute(123, 50) - BeatsPerMinute(0, 50);
	EXPECT_NEAR(123.0, sum.getAsFix16(), precision);
}
