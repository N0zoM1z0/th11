#include "AnimationSurfaceCopy.hpp"
void __stdcall FlushScreenshotResources();
extern "C" void __cdecl AnimationScreenshotFlushOnlyProbe()
{
    FlushScreenshotResources();
}
