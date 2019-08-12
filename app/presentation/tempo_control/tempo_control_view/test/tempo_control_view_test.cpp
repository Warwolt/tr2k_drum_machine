/*
****************************************************************************************************
* brief : Unit tests for TempoControlView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "tempo_knob_mock.h"
#include "fourdigitdisplaymock.h"

using ::testing::Return;

class TempoControlViewTest : public ::testing::Test
{
public:
	TempoKnobMock mockKnob;
	FourDigitDisplayMock displayMock;

};

TEST_F(TempoControlViewTest, DISABLED_Sets_display_tempo_equal_to_knob_setting)
{
	FAIL() << ERROR_MSG_STR("Write a Rhythm Playback Controller before writing this test");
}
