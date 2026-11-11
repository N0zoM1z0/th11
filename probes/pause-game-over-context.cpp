#include "PauseContinueMenu.hpp"

// Compile-only game-over dispatch context, never a replacement for a runnable
// TH11 executable. Calls the complete state20..26 function under VC9 /GL.
extern "C" void __stdcall PauseGameOverProbe(PauseContinueMenuView *owner)
{
    UpdatePauseGameOverMenu(owner);
}
