/*
****************************************************************************************************
* brief : Interface view for editing rhythm patterns
****************************************************************************************************
*/

#ifndef PATTERN_EDIT_VIEW
#define PATTERN_EDIT_VIEW

#include "RhythmPatternManager.h" // TODO: remove this !!!
#include "ButtonGroup.h"
#include "LedGroup.h"
#include "PatternEditController.h"

class PatternEditView
{
public:
    PatternEditView(PatternEditController& editController, ButtonGroup& controlButtons,
        LedGroup& stepLeds);
    void update();

private:
    static constexpr u8 numStepLeds = 16;
    static constexpr u8 channelSelectButton = 0;
    enum class ViewMode {PatternEdit, ChannelSelect};

    PatternEditController& editController;
    ButtonGroup& controlButtons;
    LedGroup& stepLeds;

    inline u8 calculateEditModeDrawBuffer();
    inline u8 calculateSelectModeDrawBuffer();
};

#endif /* PATTERN_EDIT_VIEW */

