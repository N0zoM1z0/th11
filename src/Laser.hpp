#pragma once
#include "Camera.hpp"
#include <stddef.h>

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
class LaserDataInf
{
public:
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
    // 0x14-byte timers at +0x14, +0x28, eighteen 0x34-byte pattern records
    // from +0x70 (timer first) and +0x424; the clear byte at +0x68 and the ID
    // at +0x6C are written by the manager.
    unsigned char unknown010[0x440 - 0x10];
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

typedef char LaserDataInfSize[(sizeof(LaserDataInf) == 0x440) ? 1 : -1];
typedef char LaserDataInfNext[(offsetof(LaserDataInf, next) == 8) ? 1 : -1];
typedef char LaserDataInfState[(offsetof(LaserDataInf, state) == 0xc) ? 1 : -1];
