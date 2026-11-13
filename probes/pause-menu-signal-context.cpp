#include "PauseMenu.hpp"

// Independent dynamic IDs keep the event helper's reference argument live
// instead of specializing its incoming VM identifier.
extern "C" void __stdcall PauseMenuSignalProbe(
    const AnimationVmId *first, const AnimationVmId *second)
{
    SignalPauseMenuChild(*first);
    SignalPauseMenuChild(*second);
}
