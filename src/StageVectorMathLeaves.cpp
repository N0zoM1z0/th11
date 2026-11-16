#include "StageScript.hpp"

// Native vector helpers adjacent to the stage tween implementation. The
// original owner/class identity is still unknown; keep normal C++ operations.
struct StageVectorMathView
{
    float x, y, z;
    CameraVector __fastcall operator+(const StageVectorMathView &right) const;
};

CameraVector __fastcall StageVectorMathView::operator+(const StageVectorMathView &right) const
{
    CameraVector result = { x + right.x, y + right.y, z + right.z };
    return result;
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
