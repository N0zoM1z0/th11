#include "SecondaryGameplay.hpp"

// A dynamic gameplay callsite keeps the actual pause behavior live.
extern "C" void __stdcall SecondaryDebugPlayerPauseProbe(int active)
{
    if (active)
        PauseSecondaryGameplayPlayer();
}
