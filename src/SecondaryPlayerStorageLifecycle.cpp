#include "SecondaryPlayerController.hpp"
#include "StageResource.hpp"
#include "Chain.hpp"
#include "MainRuntime.hpp"
#include "MenuController.hpp"
#include <stdlib.h>
#include <string.h>
#include <new>
#include "SecondaryPlayerEmitterPairConstructors.hpp"
#include "SecondaryPlayerElementIterator.hpp"

// Original nominal C++ owner unknown. Both globals are physically observed.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
extern SecondaryPatternConfigurationView *g_SecondaryPlayerPreparedConfiguration;

namespace
{
// A physical C++ view of two embedded VM members at their native offsets.
// The inline aggregate constructor preserves two distinct member calls
// while keeping the observed 0x434-byte VM layouts and EH lifetimes.
struct SecondaryPlayerEmbeddedVms
{
    unsigned char unknown000[0x14];
    StageEmbeddedVm primary;
    StageEmbeddedVm secondary;
    __forceinline SecondaryPlayerEmbeddedVms() : primary(), secondary() {}
};
typedef char PlayerEmbeddedVmPrimaryOffset[
    (offsetof(SecondaryPlayerEmbeddedVms, primary) == 0x14) ? 1 : -1];
typedef char PlayerEmbeddedVmSecondaryOffset[
    (offsetof(SecondaryPlayerEmbeddedVms, secondary) == 0x448) ? 1 : -1];
typedef char PlayerFirstVmStorageOffset[
    (offsetof(SecondaryPlayerEmbeddedVms, primary) +
     offsetof(StageEmbeddedVm, storage400) == 0x414) ? 1 : -1];
typedef char PlayerSecondVmStorageOffset[
    (offsetof(SecondaryPlayerEmbeddedVms, secondary) +
     offsetof(StageEmbeddedVm, storage400) == 0x848) ? 1 : -1];

// The native storage constructor's two seven-element arrays use real
// identity-only default constructors. Keep the callback behavior expressed
// through the C++ type, rather than supplying a fake register shim.
static void *__fastcall ConstructPolarPair(void *memory)
{
    return new (memory) PlayerEmitterPolarPair;
}
static void *__fastcall ConstructOffsetPair(void *memory)
{
    return new (memory) PlayerEmitterOffsetPair;
}

// The native destructor conditionally locks separately around each chain
// unregister call. C++ scoped cleanup also preserves the EH/unwind lifetime.
struct ScopedPlayerChainLock
{
    ScopedPlayerChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
            ++g_MainRuntime.criticalDepth[0];
        }
    }
    ~ScopedPlayerChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
            --g_MainRuntime.criticalDepth[0];
        }
    }
};

static __forceinline void RemovePlayerChain(ChainElement *element)
{
    if (!element)
        return;
    ChainManagerView *manager = g_ChainManager;
    ScopedPlayerChainLock lock;
    UnregisterChainElement(element, manager);
}
}

// Native 00430010..004301AE: linked chain removal, optional ANM and pattern
// retention, and cleanup of two VM-attached allocations.
void __stdcall DestructSecondaryPlayerStorage(SecondaryPlayerControllerView *owner)
{
    unsigned char *base = reinterpret_cast<unsigned char *>(owner);
    RemovePlayerChain(*reinterpret_cast<ChainElement **>(base + 8));
    RemovePlayerChain(*reinterpret_cast<ChainElement **>(base + 12));
    g_SecondaryPlayerControllerInstance = 0;

    if (!(g_StageResourceFlags & 1))
    {
        AnimationDocumentSlotsView *slots =
            reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
        AnimationDocumentView *document = slots->files[7];
        if (document)
        {
            DestroyAnimationDocument(document);
            ::operator delete(document);
            slots->files[7] = 0;
        }
        if (owner->configuration)
        {
            free(owner->configuration);
            owner->configuration = 0;
        }
        g_SecondaryPlayerPreparedConfiguration = 0;
    }
    else
    {
        AnimationFileView *file = *reinterpret_cast<AnimationFileView **>(
            base + 0x10);
        MarkMenuFileAnimations(g_RenderBatch, file);
        g_SecondaryPlayerPreparedConfiguration = owner->configuration;
    }

    // The original frees storage400 of the SECOND embedded VM before
    // storage400 of the first (+0x848, then +0x414). A real composite
    // C++ member destructor preserves the reverse lifetime ordering.
    reinterpret_cast<SecondaryPlayerEmbeddedVms *>(owner)->
        ~SecondaryPlayerEmbeddedVms();
}

// Original 0042F760..0042F86F, 272 bytes. Two StageEmbeddedVm objects
// live back-to-back at offsets 0x14 and 0x448; their constructor itself
// has a separate native exact owner (00402270).
SecondaryPlayerControllerView *__stdcall ConstructSecondaryPlayerStorage(
    SecondaryPlayerControllerView *owner)
{
    unsigned char *base = reinterpret_cast<unsigned char *>(owner);
    new (base) SecondaryPlayerEmbeddedVms;

    owner->patternTime.flags &= ~1u;
    owner->stateTime.flags &= ~1u;
    owner->auxiliaryTime.flags &= ~1u;

    // The original contains two vector-constructor calls per emitter,
    // both with count 7 and stride 8. Their linked identity callbacks
    // have no member writes, but the calls themselves are real work.
    for (int i = 0; i < 256; ++i)
    {
        unsigned int *flag = reinterpret_cast<unsigned int *>(
            base + 0x97c + i * 0x6c);
        *flag &= ~1u;
    }
    for (int i = 0; i < 8; ++i)
    {
        unsigned char *emitter = base + 0x7570 + i * 0xe4;
        ConstructSecondaryPlayerElementArray(emitter + 0x1c, 8, 7,
            ConstructPolarPair);
        ConstructSecondaryPlayerElementArray(emitter + 0x64, 8, 7,
            ConstructOffsetPair);
        unsigned int *flag = reinterpret_cast<unsigned int *>(emitter + 0xc8);
        *flag &= ~1u;
    }
    for (int i = 0; i < 33; ++i)
    {
        unsigned int *flag = reinterpret_cast<unsigned int *>(
            base + 0x7cf8 + i * 0x74);
        *flag &= ~1u;
    }
    owner->statusTime.flags &= ~1u;
    memset(owner, 0, sizeof(*owner));
    g_SecondaryPlayerControllerInstance = owner;
    return owner;
}
