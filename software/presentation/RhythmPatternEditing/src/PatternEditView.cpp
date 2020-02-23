/*
****************************************************************************************************
* brief : Interface view for editing rhythm patterns
****************************************************************************************************
*/

#include "PatternEditView.h"
#include "traceprint.h"

PatternEditView::PatternEditView(PatternEditController& editController, ButtonGroup& controlButtons,
    ButtonGroup& stepButtons, LedGroup& leds) : editController(editController),
    controlButtons(controlButtons), stepButtons(stepButtons), stepLeds(leds) {}

/**
 * @brief Iterate the view
 *
 * There are two modes for this view, editing the current pattern and selecting
 * which pattern to edit. The mode is selected with the Channel Select button.
 *
 * During edit mode, the current pattern is viewed on the LEDs and the step
 * states can be edited using the step buttons.
 *
 * During channel select mode, the LED corresponding to the active channel
 * number lights up, and a channel can be selected by pushing any step button.
 */
void PatternEditView::update()
{
    /* Decide view mode */
    bool selectButtonPressed = controlButtons.buttonIsDown(channelSelectButton);
    ViewMode mode = selectButtonPressed ? ViewMode::ChannelSelect : ViewMode::PatternEdit;

    /* Handle update */
    handleStateUpdate(mode);
    outputViewToLeds(mode);
}

/**
 * @brief If user has pushed any step button, handle update accordingly
 */
inline void PatternEditView::handleStateUpdate(ViewMode mode)
{
    if (mode == ViewMode::PatternEdit)
    {
        if (controlButtons.buttonIsDown(patternClearButton))
        {
            handlePatternClear();
        }
        else
        {
            handlePatternEdit();
        }
    }

    if (mode == ViewMode::ChannelSelect)
    {
        for (int i = 0; i < numStepButtons; i++)
        {
            if (stepButtons.buttonPressedNow(i))
            {
                editController.selectActivePattern(i);
            }
        }
    }
}

/**
 * @brief Clear pattern step if playing or whole pattern if stopped
 */
inline void PatternEditView::handlePatternClear()
{
    if (editController.playbackIsOngoing())
    {
        /* Clear current step */
        u8 playbackPosition = editController.getPlaybackPosition();
        editController.clearActivePatternStep(playbackPosition);
    }
    else
    {
        /* Clear whole pattern */
        editController.clearActivePattern();
    }
}

/**
 * @brief Toggle pattern states with step buttons
 */
inline void PatternEditView::handlePatternEdit()
{
    for (int i = 0; i < numStepButtons; i++)
    {
        if (stepButtons.buttonPressedNow(i))
        {
            editController.toggleActivePatternStep(i);
        }
    }
}

/**
 * @brief Draws the state of the selected view mode on the LEDs
 */
inline void PatternEditView::outputViewToLeds(ViewMode mode)
{
    /* Draw current view to buffer */
    u16 drawBuffer;
    if (mode == ViewMode::PatternEdit)   drawBuffer = calculateEditModeDrawBuffer();
    if (mode == ViewMode::ChannelSelect) drawBuffer = calculateSelectModeDrawBuffer();

    /* Output buffer to LEDs */
    for (int i = 0; i < numStepLeds; i++)
    {
        bool currentBitSet = (drawBuffer & 0x1 << i);
        currentBitSet ? stepLeds.setLed(i) : stepLeds.clearLed(i);
    }
}

/**
 * @brief Calculates what to display for Pattern Edit mode
 */
inline u16 PatternEditView::calculateEditModeDrawBuffer()
{
    u16 drawBuffer = 0;
    RhythmPattern activePattern = editController.getActivePattern();

    /* Add pattern state to draw buffer */
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

    /* Add playback position to draw buffer */
    if (editController.playbackIsOngoing())
    {
        drawBuffer ^= 0x1 << editController.getPlaybackPosition();
    }

    return drawBuffer;
}


/**
 * @brief Calculates what to display for Channel Select mode
 */
inline u16 PatternEditView::calculateSelectModeDrawBuffer()
{
    u16 drawBuffer = 0x1 << editController.getActivePatternNum();
    return drawBuffer;
}
