#pragma once
#include "FrameLoop.hpp"

// A typed lifetime view for the native 0x1C-byte ThreadInf storage. VC9
// emits the original constructor, normal destructor and deleting destructor
// through actual C++ object lifetime, not hand-written instruction bytes.
struct ThreadInfView
{
    ThreadInfView();
    virtual ~ThreadInfView()
    {
        StopRuntimeThread(reinterpret_cast<RuntimeThreadView *>(this));
    }
    HANDLE thread;
    unsigned int unknown08;
    int stopRequested;
    int pending;
    unsigned int unknown14;
    unsigned int unknown18;
};
typedef char ThreadInfStorageSize[(sizeof(ThreadInfView) == sizeof(RuntimeThreadView)) ? 1 : -1];
