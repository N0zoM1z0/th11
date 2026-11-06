#include "PauseContinueMenu.hpp"

// Compile-only independent menu input; no runtime harness is implied.
extern "C" void __stdcall PauseContinueMenuProbe(PauseContinueMenuView *self)
{
    UpdatePauseContinueMenu(self);
}
