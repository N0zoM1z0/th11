// Independent object input; this diagnostic image is never executed.
#include "SecondaryActorState.hpp"
extern "C" float __stdcall SecondaryPairTweenProbe(SecondaryPairTween *self)
{
    SecondaryVector2 result;
    StepSecondaryPairTween(&result, self);
    return result.x + result.y;
}
