#pragma once
#include <Windows.h>
#include <stddef.h>

// Partial physical view rooted at TH11 0x004C3280. Unknown storage is retained
// between independently observed fields; the complete owner size is open.
struct MainRuntimeView
{
    HINSTANCE instance;
    void *d3dInterface;
    void *d3dDevice;
    unsigned char unknown00c[0x584];
    unsigned int flags;
    unsigned char unknown594[0x27c];
    CRITICAL_SECTION criticalSections[12];
    unsigned char criticalDepth[12];
};

typedef char MainRuntimeFlagsOffset[(offsetof(MainRuntimeView, flags) == 0x590) ? 1 : -1];
typedef char MainRuntimeCriticalOffset[(offsetof(MainRuntimeView, criticalSections) == 0x810) ? 1 : -1];
typedef char MainRuntimeDepthOffset[(offsetof(MainRuntimeView, criticalDepth) == 0x930) ? 1 : -1];
typedef char MainRuntimeViewSize[(sizeof(MainRuntimeView) == 0x93c) ? 1 : -1];

extern MainRuntimeView g_MainRuntime;
