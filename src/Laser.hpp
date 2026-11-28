#pragma once
#include "StageResource.hpp"
#include <stddef.h>

struct AnimationDocumentView;

// laser.cpp. Class names come from the target's RTTI: LaserDataInf (vtable
// 0x494A0C, 20 slots) with derived LaserLineInf (0x494A64) and
// LaserInfiniteInf (0x494ABC). The manager allocation tag is "LaserInf".
// SecondaryClearObjectView and SecondaryHostObjectView are older partial
// views of the same objects.
//
// Slot names follow observed use: Initialize receives the spawn request right
// after allocation (0x424DF0), Update returning nonzero deletes the laser,
// Draw renders its two embedded VMs, Clear/ClearInRadius are the clearing
// entry points. The other slots keep their vtable offsets.
// Laser timers clear their initialized bit on construction, like the other
// embedded game timers (see SecondaryHud.hpp).
struct LaserTimer : StageTimerView
{
    LaserTimer() { flags &= ~1u; }
};

struct LaserPatternRecord
{
    LaserTimer timer;
    unsigned char unknown014[0x34 - 0x14];
};

class LaserDataInf
{
public:
    LaserDataInf();
    void Unlink()
    {
        previous->next = next;
        if (next) next->previous = previous;
    }

    virtual void Unknown00();
    virtual int Initialize(void *request);
    virtual int Update();
    virtual int Draw();
    virtual int Unknown10();
    virtual int Clear(int mode, int flags);
    virtual int Unknown18(int, int, int, int);
    virtual int ClearInRadius(const CameraVector *position, float radius, int mode, int flags);
    virtual int Unknown20(int, int, int, int);
    virtual int Unknown24(int, int);
    virtual void Unknown28();
    virtual void Unknown2c();
    virtual void Unknown30();
    virtual void Unknown34();
    virtual void Unknown38();
    virtual void Unknown3c();
    virtual void Unknown40();
    virtual void Unknown44();
    virtual void Unknown48();
    virtual void Unknown4c();

    LaserDataInf *previous;
    LaserDataInf *next;
    int state;
    unsigned int unknown010;
    LaserTimer timer;
    LaserTimer timer028;
    CameraVector position;
    unsigned char unknown048[0x68 - 0x48];
    unsigned char clearing;
    unsigned char unknown069[0x6c - 0x69];
    int id;
    LaserPatternRecord patterns[18];
    unsigned char unknown418[0x424 - 0x418];
    LaserTimer timer424;
    unsigned char unknown438[0x440 - 0x438];
};

class LaserLineInf : public LaserDataInf
{
public:
    virtual int Unknown10();
};

class LaserInfiniteInf : public LaserDataInf
{
public:
    virtual int Unknown10();
};

// The manager ("laser.cpp:115 LaserInf"). Its embedded LaserDataInf is the
// list head, so the first laser is head.next (+0x18).
class LaserInf
{
public:
    LaserInf();
    ~LaserInf();

    unsigned char unknown000[0x08];
    ChainElement *updateElement;
    ChainElement *drawElement;
    LaserDataInf head;
    LaserDataInf *tail;
    int count;
    int nextId;
    CameraVector clearPosition;
    unsigned char unknown468[0x474 - 0x468];
    AnimationDocumentView *document;
    unsigned char unknown478[0x47c - 0x478];
};

// Same address as g_SecondaryClearManager (0x4A8E94), an older partial view.
extern LaserInf *g_LaserManager;

int __stdcall InitializeLaserManager(LaserInf *self);
LaserInf *__cdecl CreateLaserManager();
int __fastcall LaserManagerUpdateCallback(LaserInf *self);
int __fastcall LaserManagerDrawCallback(LaserInf *self);
// Called by ECL opcode 0x16D under this older name.
int __stdcall MarkSecondaryHostEffects();

typedef char LaserDataInfSize[(sizeof(LaserDataInf) == 0x440) ? 1 : -1];
typedef char LaserDataInfNext[(offsetof(LaserDataInf, next) == 8) ? 1 : -1];
typedef char LaserDataInfState[(offsetof(LaserDataInf, state) == 0xc) ? 1 : -1];
typedef char LaserDataInfClearing[(offsetof(LaserDataInf, clearing) == 0x68) ? 1 : -1];
typedef char LaserDataInfId[(offsetof(LaserDataInf, id) == 0x6c) ? 1 : -1];
typedef char LaserDataInfTimer[(offsetof(LaserDataInf, timer) == 0x14) ? 1 : -1];
typedef char LaserDataInfPosition[(offsetof(LaserDataInf, position) == 0x3c) ? 1 : -1];
typedef char LaserDataInfPatterns[(offsetof(LaserDataInf, patterns) == 0x70) ? 1 : -1];
typedef char LaserDataInfTimer424[(offsetof(LaserDataInf, timer424) == 0x424) ? 1 : -1];
typedef char LaserInfSize[(sizeof(LaserInf) == 0x47c) ? 1 : -1];
typedef char LaserInfHead[(offsetof(LaserInf, head) == 0x10) ? 1 : -1];
typedef char LaserInfTail[(offsetof(LaserInf, tail) == 0x450) ? 1 : -1];
typedef char LaserInfClearPosition[(offsetof(LaserInf, clearPosition) == 0x45c) ? 1 : -1];
typedef char LaserInfDocument[(offsetof(LaserInf, document) == 0x474) ? 1 : -1];
