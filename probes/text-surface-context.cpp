// Non-runnable text resource initialization and release ordering.
#include "../src/TextSurface.hpp"

extern "C" bool __stdcall TextSurfaceProbe()
{
    InitializeTextResources();
    return ReleaseTextSurface();
}
