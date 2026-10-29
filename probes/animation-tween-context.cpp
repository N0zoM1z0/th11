// Independent physical tween inputs; diagnostic entry is never executed.
#include "AnimationTween.hpp"
extern "C" float __stdcall AnimationTweenProbe(int kind, AnimationIntVectorTween *vector,
    AnimationIntScalarTween *integer, AnimationFloatPairTween *pair,
    AnimationFloatScalarTween *scalar, AnimationIntVector *vectorOut,
    AnimationFloatPair *pairOut, AnimationScalarTweenTimeView *timer,
    const float *value, int setting)
{
    switch (kind)
    {
    case 0: *vectorOut = StepAnimationIntVectorTween(vector); break;
    case 1: return static_cast<float>(StepAnimationIntScalarTween(integer));
    case 2: *pairOut = StepAnimationFloatPairTween(pair); break;
    case 3: return StepAnimationFloatScalarTween(scalar);
    case 4: return AnimationIntVectorTweenFactor(vector);
    case 5: return AnimationIntScalarTweenFactor(integer);
    case 6: return AnimationFloatPairTweenFactor(pair);
    case 7: return AnimationFloatScalarTweenFactor(scalar);
    case 8: ResetAnimationScalarTweenTime(timer); break;
    case 9: return static_cast<float>(AnimationIntVectorTweenDuration(vector));
    case 10: return static_cast<float>(AnimationFloatPairTweenDuration(pair));
    case 11: return static_cast<float>(AnimationFloatScalarTweenDuration(scalar));
    case 12: SetAnimationFloatScalarTweenMode(scalar, setting); break;
    case 13: SetAnimationFloatScalarTweenDuration(scalar, setting); break;
    case 14: SetAnimationFloatScalarTweenInitial(scalar, value); break;
    case 15: SetAnimationFloatScalarTweenTarget(scalar, value); break;
    case 16: SetAnimationFloatScalarTweenControl1(scalar, value); break;
    case 17: SetAnimationFloatScalarTweenControl2(scalar, value); break;
    }
    return 0;
}
