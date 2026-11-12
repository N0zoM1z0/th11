#pragma once
#include "FrameLoop.hpp"

// A typed lifetime view for the 0x1C-byte native ThreadInf storage. The
// compiler-generated virtual destructor points at the observed vtable root
// 0x4978D0; the original translation unit and source spelling are unknown.
struct ThreadInfView
{
    virtual ~ThreadInfView();
    HANDLE thread;
    unsigned int unknown08;
    int stopRequested;
    int pending;
    unsigned int unknown14;
    unsigned int unknown18;
};
typedef char ThreadInfStorageSize[(sizeof(ThreadInfView) == sizeof(RuntimeThreadView)) ? 1 : -1];

void __stdcall DestroyThreadInfForUnwind(ThreadInfView *self);
