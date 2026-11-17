#include "SecondaryDebugHudLifetime.hpp"

// The owner at 41FE10 calls this complete independent HUD resource
// release entry, not the destructor's 468-byte prepass.
extern "C" void __stdcall SecondaryDebugHudFinalizationProbe()
{
    ClearSecondaryDebugHudResources();
}
