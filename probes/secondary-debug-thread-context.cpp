#include "SecondaryGameplay.hpp"

// A second independent dynamic callback producer prevents the native EDI
// callback convention from being accidentally folded into a fixed constant.
extern "C" void __stdcall SecondaryDebugThreadProbe(
    RuntimeThreadView *first, RuntimeThreadView *second,
    SecondaryDebugThreadCallback callback, void *context)
{
    StartSecondaryDebugThread(first, callback, context);
    StartSecondaryDebugThread(second, callback, context);
}
