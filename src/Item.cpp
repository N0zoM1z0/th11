#include "Item.hpp"
#include "ChainRegistration.hpp"
#include "GameInf.hpp"
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
    delete g_ItemManager;
}
