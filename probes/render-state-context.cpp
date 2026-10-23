// Non-runnable compiler context; device/cache pointers may change between calls.
#include "../src/RenderState.hpp"

extern "C" void __stdcall RenderStateProbe()
{
    ResetDeviceRenderStates();
    ResetDeviceRenderStates();
}
