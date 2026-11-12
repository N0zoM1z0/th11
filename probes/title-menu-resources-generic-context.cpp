#include "TitleMenuLifecycle.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"

// Diagnostic only: independent generic priorities/slot prevent /GL from
// specializing the real native chain and document helpers to title constants.
extern "C" int __stdcall TitleResourcesGenericProbe(
    ChainElement *external, int priority, int slot)
{
    int result = InitializeTitleMenuResources();
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    return result;
}
