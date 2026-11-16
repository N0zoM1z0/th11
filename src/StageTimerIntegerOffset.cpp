#include "StageResource.hpp"

// A signed-frame convenience wrapper for a float-valued timer adjustment.
// Keep the actual timer implementation as a normal callable dependency.
int __stdcall OffsetSecondaryTimer(StageTimerView *time, float amount);

int __stdcall OffsetStageTimerByFrames(StageTimerView *time, int frames)
{
    return OffsetSecondaryTimer(time, static_cast<float>(frames));
}
