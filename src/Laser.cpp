#include "Laser.hpp"
#include "ChainRegistration.hpp"
#include "GameInf.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <math.h>
#include <new>
#include <string.h>

int __stdcall AdvanceStageTimer(StageTimerView *self);
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

// 0x424EA0, 0x424EE0
LaserLineInf::LaserLineInf() {}
LaserInfiniteInf::LaserInfiniteInf() {}

// 0x425BD0, slot 11: pattern 1 accelerates the line laser. While its timer
// runs, word 0 adds to the speed and words 2-4 to the velocity, both scaled by
// the animation time scale, and the angle follows the velocity unless both
// components are tiny. When it expires, pattern flag bit 2 is cleared.
void LaserLineInf::Unknown2c()
{
    LaserPatternRecord &pattern = patterns[1];
    if (pattern.timer.current >= pattern.words[5].i)
    {
        patternFlags &= ~4u;
        AdvanceStageTimer(&pattern.timer);
        return;
    }
    speed += pattern.words[0].f * g_AnimationTimeScale;
    CameraVector step = { pattern.words[2].f * g_AnimationTimeScale, pattern.words[3].f * g_AnimationTimeScale,
        pattern.words[4].f * g_AnimationTimeScale };
    velocity.x += step.x;
    velocity.y += step.y;
    velocity.z += step.z;
    if (fabs(velocity.x) > static_cast<double>(0.0001f) || fabs(velocity.y) > static_cast<double>(0.0001f))
        angle = atan2(velocity.y, velocity.x);
    AdvanceStageTimer(&pattern.timer);
}

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

// 0x424C10. A laser marked for clearing survives one more frame.
int LaserInf::Update()
{
    LaserDataInf *next;
    for (LaserDataInf *laser = head.next; laser; laser = next)
    {
        next = laser->next;
        if (laser->clearing)
        {
            laser->clearing++;
            if (laser->clearing >= 2)
            {
                laser->Unknown10();
                Remove(laser);
                delete laser;
                continue;
            }
        }
        if (laser->state == 1)
        {
            laser->Unknown10();
            Remove(laser);
        }
        else if (laser->Update())
        {
            laser->Unknown10();
            Remove(laser);
        }
        else
        {
            laser->timer.Tick();
            laser->started = 1;
            continue;
        }
        delete laser;
    }
    return 1;
}

// 0x424D40
int LaserInf::Draw()
{
    LaserDataInf *next;
    for (LaserDataInf *laser = head.next; laser; laser = next)
    {
        next = laser->next;
        if (laser->state != 1) laser->Draw();
    }
    return 1;
}

// 0x424D70: no update while game flag bits 0, 2 or 10 are set; bit 1 updates
// with the animation time scale held at zero.
int __fastcall LaserManagerUpdateCallback(LaserInf *self)
{
    if (g_Game->flag0 | g_Game->flag2) return 1;
    if (g_Game->flag10) return 1;
    if (g_Game->freezeTime)
    {
        float scale = g_AnimationTimeScale;
        g_AnimationTimeScale = 0.0f;
        int result = self->Update();
        g_AnimationTimeScale = scale;
        return result;
    }
    return self->Update();
}

// 0x424DD0
int __fastcall LaserManagerDrawCallback(LaserInf *self)
{
    if (g_Game->flag2) return 1;
    return self->Draw();
}

int __stdcall CreateSecondaryHostObject(const void *packet, int kind)
{
    LaserInf *manager = g_LaserManager;
    if (manager->count >= 0x100) return 0;
    manager->nextId++;
    if (manager->nextId < 0x10000) manager->nextId = 0x10000;
    LaserDataInf *laser;
    switch (kind)
    {
    case 0:
        laser = new LaserLineInf;
        break;
    case 1:
        laser = new LaserInfiniteInf;
        break;
    default:
        return manager->nextId;
    }
    laser->id = manager->nextId;
    manager->Append(laser);
    laser->Initialize(const_cast<void *>(packet));
    return manager->nextId;
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
