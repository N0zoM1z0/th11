#include "../src/StageScript.hpp"
struct StageVectorMathView { float x, y, z; CameraVector __fastcall operator+(const StageVectorMathView &right) const; };
void __stdcall ScaleStageVectorByReciprocal(const CameraVector *, CameraVector *, float);
float __stdcall StageVectorSquaredLength(const CameraVector *);

extern "C" __declspec(dllexport) float __stdcall StageVectorMathLeavesProbe(
    CameraVector *output, const CameraVector *first, const CameraVector *second,
    float divisor, int mode)
{
    if (mode == 0)
    {
        *output = *reinterpret_cast<const StageVectorMathView *>(first) +
            *reinterpret_cast<const StageVectorMathView *>(second);
        return output->x;
    }
    if (mode == 1)
    {
        ScaleStageVectorByReciprocal(first, output, divisor);
        return output->y;
    }
    return StageVectorSquaredLength(second);
}
