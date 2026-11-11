#include "Screenshot.hpp"
#include "AnimationSurfaceCopy.hpp"
extern "C" void __stdcall AnimationSurfaceCopyProbe()
{
    FlushScreenshotResources();
}
