/*
****************************************************************************************************
* brief : Unit tests for BeatsPerMinute datatype
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "beatsperminute.h"

static const float precision = 0.01; // precise to 100ths place

class TestBeatsPerMinute : public ::testing::Test
{
public:

};

TEST_F(TestBeatsPerMinute, Bpm_value_can_be_accessed_as_floating_point_number)
{
	BeatsPerMinute bpm(128,50);
	EXPECT_NEAR(128.50, float(bpm), precision);
}

TEST_F(TestBeatsPerMinute, Bpm_values_can_be_summed)
{
	BeatsPerMinute sum = BeatsPerMinute(128) + BeatsPerMinute(0, 50);
	EXPECT_NEAR(128.5, float(sum), precision);
}

TEST_F(TestBeatsPerMinute, Bpm_values_can_be_subtracted)
{
	BeatsPerMinute sum = BeatsPerMinute(123, 50) - BeatsPerMinute(0, 50);
	EXPECT_NEAR(123.0, float(sum), precision);
}
