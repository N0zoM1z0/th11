#pragma once
#include <stddef.h>

// Physical timer-worker state at 0x4A5790. The native nominal class and TU
// are not yet known; only measured fields are represented here.
// The observed 32-bit layout places the first double at +4.
#pragma pack(push,4)
struct FrameTickWorkerView
{
    unsigned int flags;          // +0, bit1 running / bit0 stop request
    double currentSeconds;       // +4
    double priorSeconds;         // +0xC
    double nextTickSeconds;      // +0x14
    unsigned char unknown01c[0x2c - 0x1c];
    unsigned int completedTicks; // +0x2c
};
#pragma pack(pop)
typedef char FrameTickCurrentOffset[(offsetof(FrameTickWorkerView,currentSeconds)==4)?1:-1];
typedef char FrameTickNextOffset[(offsetof(FrameTickWorkerView,nextTickSeconds)==0x14)?1:-1];
typedef char FrameTickCountOffset[(offsetof(FrameTickWorkerView,completedTicks)==0x2c)?1:-1];
extern FrameTickWorkerView g_FrameTickWorker;
void __cdecl RunFrameTickWorker();
