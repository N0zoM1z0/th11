// Non-runnable reset/recreate ordering used by the presentation failure path.
#include "../src/RendererTextures.hpp"

extern "C" void __stdcall RendererTexturesProbe()
{
    PrepareRendererForReset();
    RestoreRendererAfterReset();
}
