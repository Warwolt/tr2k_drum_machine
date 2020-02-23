/*
****************************************************************************************************
* brief : Interface view for editing rhythm patterns
****************************************************************************************************
*/

#include "PatternEditView.h"
#include "traceprint.h"

PatternEditView::PatternEditView(PatternEditController& editController, ButtonGroup& buttons,
    LedGroup& leds) : editController(editController), controlButtons(buttons), stepLeds(leds) {}

void PatternEditView::update()
{
    bool selectButtonPressed = controlButtons.buttonIsDown(channelSelectButton);
    ViewMode mode = selectButtonPressed ? ViewMode::ChannelSelect : ViewMode::PatternEdit;

    /* Draw to buffer */
    u8 drawBuffer;
    if (mode == ViewMode::PatternEdit)
    {
        drawBuffer = calculateEditModeDrawBuffer();
    }
    if (mode == ViewMode::ChannelSelect)
    {
        drawBuffer = calculateSelectModeDrawBuffer();
    }

    /* Draw buffer to LEDs */
    for (int i = 0; i < numStepLeds; i++)
    {
        bool currentBitSet = (drawBuffer & 0x1 << i);
        currentBitSet ? stepLeds.setLed(i) : stepLeds.clearLed(i);
    }
}

inline u8 PatternEditView::calculateEditModeDrawBuffer()
{
    u16 drawBuffer = 0;
    RhythmPattern activePattern = editController.getActivePattern();
    for (int i = 0; i < activePattern.length; i++)
    {
        if (stepActiveInPattern(i, activePattern))
        {
            drawBuffer |= 0x1 << i;
        }
        else
        {
            drawBuffer &= ~(0x1 << i);
        }

    }

    if (editController.playbackIsOngoing())
    {
        drawBuffer ^= 0x1 << editController.getPlaybackPosition();
    }

    return drawBuffer;
}

inline u8 PatternEditView::calculateSelectModeDrawBuffer()
{
    u8 drawBuffer = 0;
    for (int i = 0; i < numStepLeds; i++)
    {
        RhythmPattern currentPattern = editController.getPattern(i);
        if (currentPattern.state != 0)
        {
            drawBuffer |= 0x1 << i;
        }
    }
    return drawBuffer;
}
