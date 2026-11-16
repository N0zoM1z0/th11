#include "../src/StageScript.hpp"
void __stdcall AddStageVectorComponents(const CameraVector *, const CameraVector *, CameraVector *);
void __stdcall ScaleStageVectorByReciprocal(const CameraVector *, CameraVector *, float);
float __stdcall StageVectorSquaredLength(const CameraVector *);

extern "C" __declspec(dllexport) float __stdcall StageVectorMathLeavesProbe(
    CameraVector *output, const CameraVector *first, const CameraVector *second,
    float divisor, int mode)
{
    if (mode == 0)
    {
        AddStageVectorComponents(second, first, output);
        return output->x;
    }
    if (mode == 1)
    {
        ScaleStageVectorByReciprocal(first, output, divisor);
        return output->y;
    }
    return StageVectorSquaredLength(second);
}
