/*
****************************************************************************************************
* brief : Unit tests for TempoControlView
****************************************************************************************************
*/

#include "gtest/gtest.h"
#include "traceprint.h"
#include "linuxtypes.h"
#include "tempo_control_view.h"
#include "tempo_knob_mock.h"
#include "fourdigitdisplaymock.h"
#include "tempotimermock.h"

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

TEST_F(TempoControlViewTest, Sets_display_tempo_equal_to_knob_setting)
{
	BeatsPerMinute bpm(120);
	EXPECT_CALL(knobMock, read()).WillOnce(Return(bpm));
	EXPECT_CALL(displayMock, setNumberToDisplay(int(bpm)));
	tempoView.handleTempoControl();
}

TEST_F(TempoControlViewTest, Sets_playback_tempo_via_controller_to_knob_setting)
{
	EXPECT_CALL(knobMock, read()).WillOnce(Return(BeatsPerMinute(123)));
	EXPECT_CALL(timerMock, setTempo(BeatsPerMinute(123)));
	tempoView.handleTempoControl();
}
