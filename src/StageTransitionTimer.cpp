#include "StageResource.hpp"

// Starts the primary stage's 60-frame transition and publishes mode bit 2.
// The timer's first use also installs the shared animation time scale.
void __cdecl BeginPrimaryStageTransition()
{
    StageResourceView *stage = g_StagePrimary;
    StageTimerView &timer = stage->timer2ff4;
    if (!(timer.flags & 1))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.current = 60;
    timer.fraction = 60.0f;
    timer.previous = 59;
    stage->flags2ff0 |= 4;
}
