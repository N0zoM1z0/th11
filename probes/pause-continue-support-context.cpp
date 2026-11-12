#include "PauseContinueMenu.hpp"

// Independently supplied menu owners and a VM ID prevent the /GL compiler
// from specializing this pair of native follow-up menu helper functions.
extern "C" void __stdcall PauseContinueSupportProbe(
    PauseMenuView *first, PauseMenuView *second, const AnimationVmId *id)
{
    ClosePauseContinueMenu(first);
    ShowPauseMenuAnimationTree(*id);
    ClosePauseContinueMenu(second);
}
