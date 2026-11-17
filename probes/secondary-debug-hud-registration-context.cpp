#include "SecondaryDebugHudLifetime.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"

// The real HUD factory exercises registration. Independent general-priority
// and general-file-slot calls retain the native private helper conventions
// in a representative whole-program optimization context.
extern "C" int __stdcall SecondaryDebugHudRegistrationProbe(
    int create, ChainElement *external, int priority, int slot)
{
    int result = 0;
    if (create)
    {
        SecondaryDebugHudOwned *hud = CreateSecondaryDebugHud();
        result = hud != 0;
    }
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    return result;
}
