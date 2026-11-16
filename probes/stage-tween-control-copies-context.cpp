#include "../src/StageScript.hpp"

void __stdcall SetStageTweenControl2(StageVectorTween *tween, const CameraVector *source);
void __stdcall SetStageTweenControl1(StageVectorTween *tween, const CameraVector *source);

extern "C" __declspec(dllexport) void __stdcall StageTweenControlCopiesProbe(
    StageVectorTween *tween, const CameraVector *source, int mode)
{
    if (mode)
        SetStageTweenControl2(tween, source);
    else
        SetStageTweenControl1(tween, source);
}
