#include "StageTween.hpp"

extern "C" int __stdcall StageTweenProbe(StageVectorTween *vector,
    StageFogTween *fog, CameraVector *vectorOutput, StageFogView *fogOutput,
    const StageFogView *operand, float scale)
{
    *vectorOutput = StepStageVectorTween(vector);
    *fogOutput = StepStageFogTween(fog);
    *fogOutput = *fogOutput + *operand;
    *fogOutput = *fogOutput - *operand;
    *fogOutput = *fogOutput * scale;
    int result = AdvanceStageTimer(&vector->time);
    SetStageTimer(&vector->time, vector->duration);
    return result;
}

// Stable, exported test boundary around the compiler's private value-return ABIs.
extern "C" __declspec(dllexport) int __stdcall StageTweenTestEntry(int kind,
    StageVectorTween *vector, StageFogTween *fog, void *output,
    const StageFogView *operand, StageTimerView *timer, int frame, float scale)
{
    switch (kind)
    {
    case 0: *static_cast<CameraVector *>(output) = StepStageVectorTween(vector); break;
    case 1: *static_cast<StageFogView *>(output) = StepStageFogTween(fog); break;
    case 2: *static_cast<StageFogView *>(output) = fog->initial + *operand; break;
    case 3: *static_cast<StageFogView *>(output) = fog->initial - *operand; break;
    case 4: *static_cast<StageFogView *>(output) = fog->initial * scale; break;
    case 5: *static_cast<float *>(output) = StageVectorTweenFactor(vector); break;
    case 6: *static_cast<float *>(output) = StageFogTweenFactor(fog); break;
    case 7: return AdvanceStageTimer(timer);
    case 8: SetStageTimer(timer, frame); break;
    }
    return 0;
}
