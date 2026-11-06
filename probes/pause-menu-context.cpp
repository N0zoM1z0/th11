#include "PauseMenu.hpp"

// Compile-only independent object input. No runtime harness is implied.
extern "C" void __stdcall PauseMenuProbe(PauseMenuView *self)
{
    UpdatePauseMenu(self);
}
