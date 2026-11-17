#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"
#include "VmBindings.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>

// 0x41AD30 is the separately bounded source-open HUD reset prepass.
// Do not replace it with a fictitious loop or a machine-code proxy.
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
    for (; node; node = node->next)
    {
        BoundVmView *vm = node->vm;
        const int group = *reinterpret_cast<const int *>(
            reinterpret_cast<const unsigned char *>(vm) + 0x3b0);
        if (group == marker)
            vm->flags |= 0x04000000u;
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

    for (int i = 0; i < 8; ++i)
        ReleaseTrackedHudVm(trackedVmIds[i]);

    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    ReleaseVmsByHudMarker(lists->first, ownerMarker);
    ReleaseVmsByHudMarker(lists->second, ownerMarker);

    g_SecondaryActorHud = 0;
    if (ownedData)
        free(ownedData);
    ownedData = 0;

    // The compiler now invokes overlayVms[4], auxiliaryVms[2] and
    // mainVms[9] destructors in the original reverse member order.
}
