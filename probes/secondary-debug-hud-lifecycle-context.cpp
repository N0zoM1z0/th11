#include "SecondaryDebugHudLifetime.hpp"

// Dynamic destructor caller retains all three physical StageEmbeddedVm
// member arrays and their C++ unwinding semantics in this local /GL producer.
extern "C" void __stdcall SecondaryDebugHudLifecycleProbe(
    SecondaryDebugHudOwned *owner)
{
    owner->~SecondaryDebugHudOwned();
}
