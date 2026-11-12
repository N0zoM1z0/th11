#include "ReplayFileBrowserLifetime.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <stdlib.h>

namespace
{
__forceinline void RemoveReplayBrowserChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (element)
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
            ++g_MainRuntime.criticalDepth[0];
        }
        UnregisterChainElement(element, manager);
        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
            --g_MainRuntime.criticalDepth[0];
        }
    }
}
}

// Original 0x437E40..0x437F50, 273 bytes. The two chain removals use the
// original independently guarded runtime lock/unlock operations, and the
// optional owned replay data is freed and cleared after singleton teardown.
void __stdcall DestroyReplayFileBrowser(ReplayFileBrowserView *self)
{
    RemoveReplayBrowserChain(self->update);
    RemoveReplayBrowserChain(self->draw);
    g_ReplayFileBrowser = 0;
    if (self->ownedData)
    {
        free(self->ownedData);
        self->ownedData = 0;
    }
    else
        self->ownedData = 0;
}

// Complete native 0x438000..0x43801B, null-guarded singleton destruction
// and scalar delete. The dtor owns the global reset, not this leaf.
void __cdecl DeleteReplayFileBrowser()
{
    ReplayFileBrowserView *self = g_ReplayFileBrowser;
    if (self)
    {
        DestroyReplayFileBrowser(self);
        ::operator delete(self);
    }
}
