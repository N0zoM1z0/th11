#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"
#include "VmBindings.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>

// The separately bounded 0x41AD30 reset prepass has a real source owner.
// Keep its call intact and preserve its independent ownership contract.
void __stdcall PrepareSecondaryDebugHudDestruction(SecondaryDebugHudOwned *self);

namespace
{
__forceinline void UnregisterHudChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element)
        return;
    if (g_MainRuntime.flags & 0x8000u)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(manager, element);
    if (g_MainRuntime.flags & 0x8000u)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}

// The native inline lookup scans both real renderer VM-ID lists.
__forceinline void ReleaseTrackedHudVm(unsigned int &id)
{
    if (id)
    {
        BoundVmView *vm = FindBoundVm(id);
        if (vm)
        {
            vm->flags |= 0x04000000u;
            if (!vm->marker018)
                for (BoundVmNodeView *node = vm->related; node; node = node->next)
                    node->vm->flags |= 0x04000000u;
        }
    }
    id = 0;
}

__forceinline void ReleaseVmsByHudMarker(BoundVmNodeView *node, int marker)
{
    while (node)
    {
        // The native captures each next link before marking a VM. Keeping
        // the snapshot also preserves traversal if a mark mutates the node.
        BoundVmNodeView *next = node->next;
        BoundVmView *vm = node->vm;
        const int group = *reinterpret_cast<const int *>(
            reinterpret_cast<const unsigned char *>(vm) + 0x3b0);
        if (group == marker)
            vm->flags |= 0x04000000u;
        node = next;
    }
}
}

// Native 0x41AFC0..0x41B298. The 9/2/4 VM arrays are destructed by
// ordinary VC9 C++ member-array destruction at function exit.
SecondaryDebugHudOwned::~SecondaryDebugHudOwned()
{
    PrepareSecondaryDebugHudDestruction(this);

    UnregisterHudChain(updateChain);
    UnregisterHudChain(drawChain);
    UnregisterHudChain(overlayChain);

    updateChain = 0;
    ReleaseSecondaryAnimation(AnimationVmId(effectVmId));
    effectVmId = 0;
    ReleaseSecondaryAnimation(AnimationVmId(transitionVmId));
    transitionVmId = 0;

    // The native visits eight slots with a decrementing count, not a
    // pointer-to-end comparison (which changes this /GL owner's layout).
    unsigned int *current = trackedVmIds;
    for (int remaining = 8; remaining; --remaining, ++current)
        ReleaseTrackedHudVm(*current);

    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    // The original snapshots the owner marker once for both renderer lists.
    const int marker = ownerMarker;
    ReleaseVmsByHudMarker(lists->first, marker);
    ReleaseVmsByHudMarker(lists->second, marker);

    g_SecondaryActorHud = 0;

    // Ordinary C++ now destroys persistentVm followed by overlayVms[4],
    // auxiliaryVms[2] and mainVms[9], in that exact reverse member order.
}
