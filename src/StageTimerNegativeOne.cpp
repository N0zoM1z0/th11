#include "StageResource.hpp"

int __stdcall OffsetSecondaryTimer(StageTimerView *time, float amount);

// The constant-step timer operation is expressed as a member call so VC9
// naturally supplies the timer receiver in ECX and keeps the -1.0f literal.
struct StageTimerNegativeOneView : StageTimerView
{
    int OffsetByOneFrame();
};

int StageTimerNegativeOneView::OffsetByOneFrame()
{
    return OffsetSecondaryTimer(this, -1.0f);
}
