// Independent input; this diagnostic image is never executed.
#include "SecondaryActorRuntime.hpp"
extern "C" int __stdcall SecondaryActorMotionProbe(SecondaryMotionView *motion, StageTimerView *timer, float amount)
{
    AdvanceSecondaryMotion(motion);
    ComposeSecondaryMotion(motion);
    return OffsetSecondaryTimer(timer, amount);
}
