/*
****************************************************************************************************
* brief : Unit tests for TempoControlView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "TempoControlView.h"
#include "TempoKnobMock.h"
#include "FourDigitDisplayMock.h"
#include "TempoTimerMock.h"

using ::testing::Return;
using ::testing::NiceMock;

class TempoControlViewTest : public ::testing::Test
{
public:
	/* Dependencies */
	NiceMock<TempoTimerMock> timerMock;
	NiceMock<TempoKnobMock> knobMock;
	NiceMock<FourDigitDisplayMock> displayMock;
	RhythmPlaybackController controller = RhythmPlaybackController(timerMock);

	/* Code under test */
	TempoControlView tempoView = TempoControlView(controller, knobMock, displayMock);
};

TEST_F(TempoControlViewTest, Enables_dot_for_digit_1_on_display_in_constructor)
{
	EXPECT_CALL(displayMock, enableDecimalPoint(1));
	TempoControlView(controller, knobMock, displayMock);
}

TEST_F(TempoControlViewTest, Sets_display_tempo_equal_to_knob_setting)
{
	BeatsPerMinute bpm(120);
	EXPECT_CALL(knobMock, read()).WillOnce(Return(bpm));
	EXPECT_CALL(displayMock, setNumberToDisplay(int(bpm * 10))); // 120 is displayed as 120.0
	tempoView.handleTempoControl();
}

TEST_F(TempoControlViewTest, Sets_playback_tempo_via_controller_to_knob_setting)
{
	EXPECT_CALL(knobMock, read()).WillOnce(Return(BeatsPerMinute(123)));
	EXPECT_CALL(timerMock, setTempo(BeatsPerMinute(123)));
	tempoView.handleTempoControl();
}
