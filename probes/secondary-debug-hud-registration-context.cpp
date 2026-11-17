#include "SecondaryDebugHudLifetime.hpp"

// Real factory provides an unspecialized owning C++ caller for the
// 339-byte registration owner, without an ASM or dummy callback proxy.
extern "C" int __stdcall SecondaryDebugHudRegistrationProbe(int create)
{
    if (create)
    {
        SecondaryDebugHudOwned *hud = CreateSecondaryDebugHud();
        return hud != 0;
    }
    return 0;
}
