#include "SecondaryDebugSessionLifecycle.hpp"

// A live, unspecialized C++ destructor caller for the original
// 462-byte debug-session owner; no machine-code proxy is used.
extern "C" void __stdcall SecondaryDebugSessionLifecycleProbe(
    SecondaryEclDebugOwner *self)
{
    self->~SecondaryEclDebugOwner();
}
