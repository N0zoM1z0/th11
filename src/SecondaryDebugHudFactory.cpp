#include "SecondaryDebugHudLifetime.hpp"
#include <new>

// Native TH11 0x41B2A0: the owning HUD is a genuine 0x4458-byte
// C++ object with sixteen embedded StageEmbeddedVm members.
// Do not turn its constructor into memset-only raw heap storage.
SecondaryDebugHudOwned *CreateSecondaryDebugHud()
{
    SecondaryDebugHudOwned *hud = new SecondaryDebugHudOwned;
    if (RegisterSecondaryHudChains(hud))
    {
        delete hud;
        return 0;
    }
    return hud;
}
