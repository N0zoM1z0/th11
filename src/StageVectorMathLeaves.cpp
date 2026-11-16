#include "StageScript.hpp"

// Native vector helpers adjacent to the stage tween implementation. The
// original owner/class identity is still unknown; keep normal C++ operations.
void __stdcall AddStageVectorComponents(const CameraVector *right,
    const CameraVector *left, CameraVector *out)
{
    out->x = right->x + left->x;
    out->y = right->y + left->y;
    out->z = right->z + left->z;
}

void __stdcall ScaleStageVectorByReciprocal(const CameraVector *source,
    CameraVector *out, float divisor)
{
    const float reciprocal = 1.0f / divisor;
    out->x = source->x * reciprocal;
    out->y = source->y * reciprocal;
    out->z = source->z * reciprocal;
}

float __stdcall StageVectorSquaredLength(const CameraVector *source)
{
    return source->x * source->x + source->y * source->y +
        source->z * source->z;
}
