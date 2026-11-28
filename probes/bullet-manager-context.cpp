#include "Bullet.hpp"
#include "ChainRegistration.hpp"
#include "MenuController.hpp"

// Diagnostic only: create, reset and delete the manager as the game does, and
// call the shared helpers with generic arguments.
extern "C" int __stdcall BulletManagerProbe(ChainElement *external, int priority, int slot,
    RenderBatchView *renderer, AnimationFileView *file)
{
    int result = CreateBulletManager() != 0;
    ResetBulletManager();
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    MarkMenuFileAnimations(renderer, file);
    DeleteBulletManager();
    return result;
}
