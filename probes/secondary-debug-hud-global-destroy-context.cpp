#include "SecondaryDebugHudLifetime.hpp"

// Scene-level caller keeps the shutdown function in the public PDB
// contribution without substituting a machine-code thunk.
extern "C" void __stdcall SecondaryDebugHudGlobalDestroyProbe()
{
    DestroySecondaryDebugHudSingleton();
}
