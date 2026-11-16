#include "SecondaryHud.hpp"
#include "StageResource.hpp"
#include <new>

// Stage handoff creates the thirty-frame screen transition and resets the
// secondary stage timer, regardless of whether effect allocation succeeds.
void __cdecl BeginSecondaryStageTransition()
{
    StageResourceView *stage = g_StageSecondary;
    SecondaryHudScreenEffect *effect = new SecondaryHudScreenEffect;
    if (effect)
        InitializeSecondaryHudScreenEffect(effect, 2, 30, 0, 0, 10);

    StageTimerView &timer = stage->timer2ff4;
    if (!(timer.flags & 1))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.current = 30;
    timer.fraction = 30.0f;
    timer.previous = 29;
    stage->flags2ff0 |= 2;
}
