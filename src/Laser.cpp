#include "Laser.hpp"
#include "ChainRegistration.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <string.h>

extern const char g_LaserAnimationName[];
extern const char g_LaserAnimationError[];

namespace
{
struct LaserChainElement : ChainElement
{
    LaserChainElement()
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

__forceinline ChainElement *RegisterLaserChain(LaserInf *self, ChainCallback callback, int priority, bool draw)
{
    LaserChainElement *chain = new LaserChainElement;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = self;
    chain->flags = (chain->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(chain, priority);
    else AddToUpdateChain(chain, priority);
    return chain;
}

__forceinline void UnregisterLaserChain(ChainElement *element)
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

// 0x424830. The vtable store and member timer constructors run before the
// body, whose memset clears the whole object including the vtable pointer.
LaserDataInf::LaserDataInf()
{
    memset(this, 0, sizeof(LaserDataInf));
    timer.SetFrame(-1);
}

// LaserDataInf default slots, 0x4246E0..0x424820. Native code order is
// slots 0-4, 7, 6, 8, 5, 9, 10-19.
void LaserDataInf::Unknown00() {}
int LaserDataInf::Initialize(void *) { return 0; }
int LaserDataInf::Update() { return 0; }
int LaserDataInf::Draw() { return 0; }
int LaserDataInf::Unknown10()
{
    Unlink();
    return 0;
}
int LaserDataInf::ClearInRadius(const CameraVector *, float, int, int) { return 0; }
int LaserDataInf::Unknown18(int, int, int, int) { return 0; }
int LaserDataInf::Unknown20(int, int, int, int) { return 0; }
int LaserDataInf::Clear(int, int) { return 0; }
int LaserDataInf::Unknown24(int, int) { return 0; }
void LaserDataInf::Unknown28() {}
void LaserDataInf::Unknown2c() {}
void LaserDataInf::Unknown30() {}
void LaserDataInf::Unknown34() {}
void LaserDataInf::Unknown38() {}
void LaserDataInf::Unknown3c() {}
void LaserDataInf::Unknown40() {}
void LaserDataInf::Unknown44() {}
void LaserDataInf::Unknown48() {}
void LaserDataInf::Unknown4c() {}

// Both derived lasers replace slot 4 with an empty result (0x426220, 0x427720).
int LaserLineInf::Unknown10() { return 0; }
int LaserInfiniteInf::Unknown10() { return 0; }

// 0x424FB0: flag every live laser for clearing; the manager update deletes it
// one frame later.
int __stdcall MarkSecondaryHostEffects()
{
    for (LaserDataInf *laser = g_LaserManager->head.next; laser; )
    {
        LaserDataInf *next = laser->next;
        if (laser->state != 1 && !laser->clearing) laser->clearing = 1;
        laser = next;
    }
    return 0;
}

inline LaserInf::LaserInf()
{
    memset(this, 0, sizeof(LaserInf));
    nextId = 0x10000;
    g_LaserManager = this;
}

// 0x424970: load bullet.anm into slot 6, register update priority 19 and draw
// priority 27; the list tail starts at the embedded head.
int __stdcall InitializeLaserManager(LaserInf *self)
{
    self->document = AcquireAnimationDocument(6, g_LaserAnimationName);
    if (!self->document)
    {
        AppendLog(&g_RuntimeLog.buffer, g_LaserAnimationError);
        return -1;
    }
    self->updateElement = RegisterLaserChain(self,
        reinterpret_cast<ChainCallback>(LaserManagerUpdateCallback), 19, false);
    self->drawElement = RegisterLaserChain(self,
        reinterpret_cast<ChainCallback>(LaserManagerDrawCallback), 27, true);
    self->tail = &self->head;
    return 0;
}

// 0x424A70: unregister both chain elements and delete every laser.
LaserInf::~LaserInf()
{
    UnregisterLaserChain(updateElement);
    UnregisterLaserChain(drawElement);
    LaserDataInf *next;
    for (LaserDataInf *laser = head.next; laser; laser = next)
    {
        next = laser->next;
        laser->Unknown10();
        laser->Unlink();
        delete laser;
    }
    g_LaserManager = 0;
}

// 0x424B60
LaserInf *__cdecl CreateLaserManager()
{
    LaserInf *self = new LaserInf;
    if (InitializeLaserManager(self))
    {
        delete self;
        return 0;
    }
    return self;
}
