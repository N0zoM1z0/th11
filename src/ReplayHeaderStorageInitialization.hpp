#pragma once
#include <stddef.h>

// An 18-byte default block populated by the native replay-settings loader;
// native copies it as four dwords and a word. Field meanings remain opaque.
#pragma pack(push, 2)
struct ReplayHeaderStorageDefaultsView
{
    unsigned int word000;
    unsigned int word004;
    unsigned int word008;
    unsigned int word00c;
    unsigned short word010;
};
#pragma pack(pop)
extern ReplayHeaderStorageDefaultsView g_ReplayHeaderStorageDefaults;

// The 0x3C-byte settings block embedded at +0x24 of its owners.
struct ReplayHeaderStorageView
{
    unsigned int version;
    ReplayHeaderStorageDefaultsView defaults;
    unsigned short value016, value018;
    unsigned char byte01a, byte01b, byte01c, byte01d, byte01e;
    unsigned char byte01f, byte020, byte021, byte022, byte023;
    unsigned char unknown024[4];
    unsigned int value028, value02c;
    unsigned char unknown030[8];
    unsigned int flags;
};
typedef char ReplayHeaderStorageSize[(sizeof(ReplayHeaderStorageView) == 0x3c) ? 1 : -1];
typedef char ReplayHeaderStorageFlagsOffset[(offsetof(ReplayHeaderStorageView, flags) == 0x38) ? 1 : -1];
typedef char ReplayHeaderStorageValueOffset[(offsetof(ReplayHeaderStorageView, value016) == 0x16) ? 1 : -1];

void __stdcall InitializeReplayHeaderStorage(ReplayHeaderStorageView *storage);
