#include "SecondaryDebugHudLifetime.hpp"

// A distinct scene cleanup caller preserves the real separate stdcall
// contract of the 468-byte precleanup owner under VC9 /GL. The main
// owner is still destroyed through its genuine C++ destructor.
extern "C" void __stdcall SecondaryDebugHudDestructorGenericProbe(
    SecondaryDebugHudOwned *owner, SecondaryDebugHudOwned *cleanupOwner, int releaseResources)
{
    owner->~SecondaryDebugHudOwned();
    if (releaseResources)
        PrepareSecondaryDebugHudDestruction(cleanupOwner);
}
