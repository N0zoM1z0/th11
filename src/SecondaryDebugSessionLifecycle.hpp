#pragma once

#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"
#include <stddef.h>

// Native ThreadInf member at secondary debug root+0x10. The virtual
// destructor is genuine C++ ownership, not a raw vtable-address store.
// Its physical layout matches the runtime thread view (0x1c bytes).
struct SecondaryDebugThreadOwner
{
    virtual ~SecondaryDebugThreadOwner();

    HANDLE thread;
    unsigned int unknown008;
    int stopRequested;
    int pending;
    unsigned int unknown014;
    unsigned int unknown018;
};
typedef char SecondaryDebugThreadOwnerSize[
    (sizeof(SecondaryDebugThreadOwner) == sizeof(RuntimeThreadView)) ? 1 : -1];

// Owning view of the existing debug-session object. The gameplay-side
// SecondaryEclDebugView is a non-owning physical projection of the same
// storage and retains all of its previously reviewed field offsets.
struct SecondaryEclDebugOwner
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    SecondaryDebugThreadOwner worker;
    unsigned int unknown02c;
    int state;
    char **filenames;
    int fileCount;
    unsigned char unknown03c[0x2c8 - 0x3c];
    // Embedded script VM. Its inline destructor frees storage at +0x6C8
    // after the body; the EH unwind map calls the out-of-line 0x402170.
    StageEmbeddedVm vm;
    unsigned char unknown6fc[0x710 - 0x6fc];

    ~SecondaryEclDebugOwner();
};

typedef char SecondaryDebugSessionOwnerSize[
    (sizeof(SecondaryEclDebugOwner) == 0x710) ? 1 : -1];
typedef char SecondaryDebugOwnerWorkerOffset[
    (offsetof(SecondaryEclDebugOwner, worker) == 0x10) ? 1 : -1];
typedef char SecondaryDebugOwnerFilenamesOffset[
    (offsetof(SecondaryEclDebugOwner, filenames) == 0x34) ? 1 : -1];
typedef char SecondaryDebugOwnerCountOffset[
    (offsetof(SecondaryEclDebugOwner, fileCount) == 0x38) ? 1 : -1];
typedef char SecondaryDebugOwnerVmOffset[
    (offsetof(SecondaryEclDebugOwner, vm) == 0x2c8) ? 1 : -1];

void __stdcall ReleaseSecondaryDebugFilenames(SecondaryEclDebugView *self);
