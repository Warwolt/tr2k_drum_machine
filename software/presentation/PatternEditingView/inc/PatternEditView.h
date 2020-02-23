/*
****************************************************************************************************
* brief : Interface view for editing rhythm patterns
****************************************************************************************************
*/

#ifndef PATTERN_EDIT_VIEW
#define PATTERN_EDIT_VIEW

#include "ButtonGroup.h"
#include "LedGroup.h"
#include "PatternEditController.h"

class PatternEditView
{
public:
    PatternEditView(PatternEditController& editController, ButtonGroup& controlButtons,
        ButtonGroup& stepButtons, LedGroup& stepLeds);
    void update();

private:
    enum class ViewMode {PatternEdit, ChannelSelect};

    static constexpr u8 numStepLeds = 16;
    static constexpr u8 numStepButtons = 16;
    static constexpr u8 channelSelectButton = 0;
    static constexpr u8 patternClearButton = 1;

    PatternEditController& editController;
    ButtonGroup& controlButtons;
    ButtonGroup& stepButtons;
    LedGroup& stepLeds;

    inline void handleStateUpdate(ViewMode mode);
    inline void handlePatternEdit();
    inline void handlePatternClear();
    inline void outputViewToLeds(ViewMode mode);
    inline u16 calculateEditModeDrawBuffer();
    inline u16 calculateSelectModeDrawBuffer();
};

#endif /* PATTERN_EDIT_VIEW */

