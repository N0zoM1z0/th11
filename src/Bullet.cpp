#include "Bullet.hpp"
#include "ChainRegistration.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include "MenuController.hpp"
#include <new>
#include <string.h>

extern const char g_BulletAnimationName[];
extern const char g_BulletAnimationError[];

namespace
{
struct BulletChainElement : ChainElement
{
    BulletChainElement()
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

__forceinline ChainElement *RegisterBulletChain(BulletInf *self, ChainCallback callback, int priority, bool draw)
{
    BulletChainElement *chain = new BulletChainElement;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = self;
    chain->flags = (chain->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(chain, priority);
    else AddToUpdateChain(chain, priority);
    return chain;
}

__forceinline void UnregisterBulletChain(ChainElement *element)
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

inline BulletInf::BulletInf()
{
    memset(this, 0, sizeof(BulletInf));
    g_BulletManager = this;
}

// 0x408450: load bullet.anm into slot 6, point the cursor at the first bullet,
// mark the sentinel, then register update priority 20 and draw priority 29.
int __stdcall InitializeBulletManager(BulletInf *self)
{
    self->document = AcquireAnimationDocument(6, g_BulletAnimationName);
    if (!self->document)
    {
        AppendLog(&g_RuntimeLog.buffer, g_BulletAnimationError);
        return -1;
    }
    self->bulletCursor = self->bullets;
    self->bullets[0x7d0].state = 5;
    self->updateElement = RegisterBulletChain(self,
        reinterpret_cast<ChainCallback>(BulletManagerUpdateCallback), 20, false);
    self->drawElement = RegisterBulletChain(self,
        reinterpret_cast<ChainCallback>(BulletManagerDrawCallback), 29, true);
    return 0;
}

// 0x408550: drop every bullet animation and clear all bullets.
void __cdecl ResetBulletManager()
{
    BulletInf *self = g_BulletManager;
    MarkMenuFileAnimations(g_RenderBatch, reinterpret_cast<AnimationFileView *>(self->document));
    memset(self->bullets, 0, sizeof(self->bullets));
    self->bulletCursor = self->bullets;
    self->bullets[0x7d0].state = 5;
}

// 0x4085F0
BulletInf::~BulletInf()
{
    UnregisterBulletChain(updateElement);
    UnregisterBulletChain(drawElement);
    MarkMenuFileAnimations(g_RenderBatch, reinterpret_cast<AnimationFileView *>(document));
    g_BulletManager = 0;
}

// 0x408710
BulletInf *__cdecl CreateBulletManager()
{
    BulletInf *self = new BulletInf;
    if (InitializeBulletManager(self))
    {
        delete self;
        return 0;
    }
    return self;
}

// 0x408800
void __cdecl DeleteBulletManager()
{
    BulletInf *self = g_BulletManager;
    if (self) delete self;
}
