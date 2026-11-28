#include "Laser.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"

// Diagnostic only: an independent caller with generic chain priorities and a
// generic document slot keeps /GL from specializing the shared helpers to the
// laser constants, and deletes the manager the way the game teardown does.
extern "C" int __stdcall LaserManagerProbe(ChainElement *external, int priority, int slot)
{
    int result = CreateLaserManager() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    result += MarkSecondaryHostEffects();
    // The ECL callers pass a local request block and a constant kind.
    unsigned int request[0x80];
    request[0] = slot;
    result += CreateSecondaryHostObject(request, 0);
    request[1] = priority;
    result += CreateSecondaryHostObject(request, 1);
    delete g_LaserManager;
    return result;
}
