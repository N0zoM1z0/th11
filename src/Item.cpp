#include "Item.hpp"
#include "ChainRegistration.hpp"
#include "GameInf.hpp"
#include "AnimationImmediateDrawViews.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <string.h>

namespace
{
struct ItemChainElement : ChainElement
{
    ItemChainElement()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline ChainElement *RegisterItemChain(ItemInf *self, ChainCallback callback, int priority, bool draw)
{
    ItemChainElement *chain = new ItemChainElement;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = self;
    chain->flags = (chain->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(chain, priority);
    else AddToUpdateChain(chain, priority);
    return chain;
}

__forceinline void UnregisterItemChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element) return;
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(manager, element);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

inline ItemInf::ItemInf()
{
    memset(this, 0, sizeof(ItemInf));
    flags |= 2u;
    g_ItemManager = this;
}

// 0x423270: update priority 21, draw priority 25.
int __stdcall InitializeItemManager(ItemInf *self)
{
    self->updateElement = RegisterItemChain(self,
        reinterpret_cast<ChainCallback>(ItemManagerUpdateCallback), 21, false);
    self->drawElement = RegisterItemChain(self,
        reinterpret_cast<ChainCallback>(ItemManagerDrawCallback), 25, true);
    return 0;
}

// 0x424090: like the laser update callback, but tolerating a missing GameInf.
int __fastcall ItemManagerUpdateCallback(ItemInf *self)
{
    GameInf *game = g_Game;
    if (game)
    {
        if (game->flag0 | game->flag2) return 1;
        if (game->flag10) return 1;
    }
    return UpdateItemManager(self);
}

// 0x423F40: draw every live item. Items above the playfield top are drawn as
// an arrow at y 24 whose alpha grows with the distance over 32 units.
int __stdcall DrawItemManager(ItemInf *self)
{
    ItemData *item = self->items;
    for (int i = 0; i < 0x896; ++i, ++item)
    {
        if (!item->state) continue;
        AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(&item->vm);
        unsigned char *alpha = reinterpret_cast<unsigned char *>(&vm->color) + 3;
        CameraVector &position = vm->position;
        position.x = static_cast<float>(item->position.x + 32.0 + 192.0);
        position.y = static_cast<float>(item->position.y + 16.0);
        position.z = item->position.z;
        if (position.y < 8.0f)
        {
            float distance = static_cast<float>(position.y - 8.0);
            position.y = 24.0f;
            if (distance >= 32.0f)
                *alpha = 0xff;
            else
                *alpha = static_cast<unsigned char>(static_cast<int>(distance / 32.0 * 255.0));
            if (vm->marker != item->type + 0x162)
                vm->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(g_SecondaryActorFiles->file),
                    item->type + 0x161);
        }
        else if (vm->marker != item->type + 0x159)
        {
            vm->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(g_SecondaryActorFiles->file),
                item->type + 0x158);
            *alpha = 0xff;
        }
        reinterpret_cast<ImmediateVmView *>(vm)->DrawByMode(g_RenderBatch);
    }
    return 1;
}

// 0x4240F0, called from the update loop: every live item of kind 1, 4, 10 or
// 11 among the first 150 becomes a kind-9 drop moving up (angle -pi/2, speed
// 2.2) and leaves effect script 0x73.
int __stdcall ConvertItemsToDrops(ItemInf *self)
{
    SecondaryControllerDropManager *drops = reinterpret_cast<SecondaryControllerDropManager *>(self);
    ItemData *item = self->items;
    for (int i = 0; i < 0x96; ++i, ++item)
    {
        if (!item->state) continue;
        if (item->kind != 1 && item->kind != 4 && item->kind != 10 && item->kind != 11) continue;
        item->state = 0;
        if (item->kind == 1)
        {
            SpawnSecondaryPlayerDrop(drops, &item->position, 9, 0xffffffff, -1.5707964f, 2.2f);
            g_SecondaryActorFiles->file->CreatePositionedVmId(&item->position, 0x73, 0x16);
        }
        else if (item->kind == 4)
        {
            SpawnSecondaryPlayerDrop(drops, &item->position, 9, 0xffffffff, -1.5707964f, 2.2f);
            g_SecondaryActorFiles->file->CreatePositionedVmId(&item->position, 0x73, 0x16);
        }
        else if (item->kind == 10)
        {
            SpawnSecondaryPlayerDrop(drops, &item->position, 9, 0xffffffff, -1.5707964f, 2.2f);
            g_SecondaryActorFiles->file->CreatePositionedVmId(&item->position, 0x73, 0x16);
        }
        else if (item->kind == 11)
        {
            SpawnSecondaryPlayerDrop(drops, &item->position, 9, 0xffffffff, -1.5707964f, 2.2f);
            g_SecondaryActorFiles->file->CreatePositionedVmId(&item->position, 0x73, 0x16);
        }
    }
    return 0;
}

// 0x4240D0
int __fastcall ItemManagerDrawCallback(ItemInf *self)
{
    GameInf *game = g_Game;
    if (game && game->flag2) return 1;
    return DrawItemManager(self);
}

// 0x423330
ItemInf::~ItemInf()
{
    UnregisterItemChain(updateElement);
    UnregisterItemChain(drawElement);
    g_ItemManager = 0;
}

// 0x423440
ItemInf *__cdecl CreateItemManager()
{
    ItemInf *self = new ItemInf;
    if (InitializeItemManager(self))
    {
        delete self;
        return 0;
    }
    return self;
}

// 0x423530
void __cdecl DeleteItemManager()
{
    ItemInf *self = g_ItemManager;
    if (self) delete self;
}
