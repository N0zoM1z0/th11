#include "SecondaryDebugOverlay.hpp"

// Genuine callers of both the stateful debug update and the paired ASCII/VM
// overlay. The compiler can retain its original private callback lifetimes.
extern "C" int __stdcall SecondaryDebugOverlayProbe(
    SecondaryEclDebugView *session, int invokeUpdate)
{
    if (invokeUpdate) UpdateSecondaryEclDebug(session);
    return RenderSecondaryDebugOverlayCallback(session);
}
