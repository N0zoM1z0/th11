#include "SecondaryDebugHudLifetime.hpp"

// A real C++ caller retains the full initialization entry as a
// public /GL contribution without a synthetic instruction thunk.
extern "C" void __stdcall SecondaryDebugHudAnimationInitProbe()
{
    InitializeSecondaryDebugHudAnimations();
}
