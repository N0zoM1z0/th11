#include "SecondaryAuxiliaryOwner.hpp"
#include "ChainRegistration.hpp"

// Diagnostic only: an independent caller with generic chain priorities and a
// generic document slot keeps /GL from specializing the shared helpers to the
// owner's constants; it also destroys the published owner.
extern "C" int __stdcall SecondaryAuxiliaryOwnerProbe(ChainElement *external, int priority, int slot)
{
    int result = CreateSecondaryAuxiliaryOwner() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    if (g_SecondaryAuxiliaryOwner)
        DestroySecondaryAuxiliaryOwner(g_SecondaryAuxiliaryOwner);
    return result;
}
