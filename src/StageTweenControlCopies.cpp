#include "StageScript.hpp"

// Two short control-point setters in the stage interpolation neighborhood.
// Their original class identity is unknown; both copy all three vector words.
void __stdcall SetStageTweenControl2(StageVectorTween *tween, const CameraVector *source)
{
    tween->control2 = *source;
}

void __stdcall SetStageTweenControl1(StageVectorTween *tween, const CameraVector *source)
{
    tween->control1 = *source;
}
