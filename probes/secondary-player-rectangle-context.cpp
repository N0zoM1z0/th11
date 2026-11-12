#include "../src/SecondaryPlayerController.hpp"
extern int __stdcall CheckSecondaryPlayerRectangle(const float *, const CameraVector *);
extern "C" int __stdcall SecondaryPlayerRectangleProbe(const float *extent,
    const CameraVector *position)
{
    return CheckSecondaryPlayerRectangle(extent, position);
}
