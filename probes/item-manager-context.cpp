#include "Item.hpp"
#include "ChainRegistration.hpp"
#include "AnimationImmediateDrawViews.hpp"
#include "SecondaryPlayerController.hpp"

// Diagnostic only: create and delete the manager the way the game does and
// call the chain helpers with generic values.
extern "C" int __stdcall ItemManagerProbe(ChainElement *external, int priority, RenderBatchView *renderer, AnimationVmView *vm, ItemInf *items,
    const CameraVector *position, int kind, unsigned int color, float angle, AnimationFileView *file)
{
    int result = CreateItemManager() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (g_ItemManager)
        result += ItemManagerDrawCallback(g_ItemManager) + ItemManagerUpdateCallback(g_ItemManager);
    DeleteItemManager();
    result += ConvertItemsToDrops(items);
    // Generic callers of the drop spawn and positioned VM creation, as in the game.
    result += SpawnSecondaryPlayerDrop(reinterpret_cast<SecondaryControllerDropManager *>(items), position, kind, color, angle, angle);
    result += file->CreatePositionedVmId(position, kind, priority).value;
    // Other callers pass other renderers; keep DrawByMode's renderer argument.
    result += reinterpret_cast<ImmediateVmView *>(vm)->DrawByMode(renderer);
    return result;
}
