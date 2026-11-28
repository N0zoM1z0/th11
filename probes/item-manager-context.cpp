#include "Item.hpp"
#include "ChainRegistration.hpp"
#include "AnimationImmediateDrawViews.hpp"

// Diagnostic only: create and delete the manager the way the game does and
// call the chain helpers with generic values.
extern "C" int __stdcall ItemManagerProbe(ChainElement *external, int priority, RenderBatchView *renderer, AnimationVmView *vm)
{
    int result = CreateItemManager() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (g_ItemManager)
        result += ItemManagerDrawCallback(g_ItemManager) + ItemManagerUpdateCallback(g_ItemManager);
    DeleteItemManager();
    // Other callers pass other renderers; keep DrawByMode's renderer argument.
    result += reinterpret_cast<ImmediateVmView *>(vm)->DrawByMode(renderer);
    return result;
}
