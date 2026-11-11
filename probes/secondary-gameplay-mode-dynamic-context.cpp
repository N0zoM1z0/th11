#include "SecondaryGameplay.hpp"

// Independently supplied modes and different runtime roots retain the true
// variable-argument calling convention without fake ABI/register shims.
extern "C" void __stdcall SecondaryGameplayModeGenericProbe(
    MainRuntimeView *first, MainRuntimeView *second, int mode, int other)
{
    SetSecondaryGameplayRuntimeMode(first, mode);
    SetSecondaryGameplayRuntimeMode(second, other);
}
