#include "SecondaryProgramBrowser.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"
#include "FrameLoop.hpp"
#include <stdlib.h>
#include <new>

// The original ThreadInf virtual table is observed at 0x4978D0. Its nominal
// class remains unresolved; the corresponding native data root stays external.
extern const unsigned int g_SecondaryBrowserWorkerVtable[];

namespace
{
// The native browser has an owned ThreadInf member, not a plain worker
// pointer. Its destructor must also run during exception unwinding; keeping
// this C++ local lifetime restores the original SEH frame naturally.
struct BrowserWorkerGuard
{
    RuntimeThreadView *worker;
    ~BrowserWorkerGuard()
    {
        worker->unknown00 = reinterpret_cast<unsigned int>(g_SecondaryBrowserWorkerVtable);
        StopRuntimeThread(worker);
    }
};

struct ScopedBrowserChainLock
{
    ScopedBrowserChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
            ++g_MainRuntime.criticalDepth[0];
        }
    }
    ~ScopedBrowserChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
            --g_MainRuntime.criticalDepth[0];
        }
    }
};

__forceinline void RemoveBrowserChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (element)
    {
        ScopedBrowserChainLock lock;
        UnregisterChainElement(element, manager);
    }
}

__forceinline void ReleaseBrowserAnimation(int index)
{
    AnimationDocumentSlotsView *slots =
        reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
    AnimationDocumentView **slot = &slots->files[index];
    AnimationDocumentView *file = *slot;
    if (file)
    {
        DestroyAnimationDocument(file);
        ::operator delete(file);
        *slot = 0;
    }
}
}

// Native TH11 00438710..004388C6 (439 bytes). Complete explicit owner of
// the browser file-list, chain nodes, loaded ECL, script manager, two ANM
// slots and worker. The primary-TU EH/private register match remains open.
void __stdcall DestroySecondaryProgramBrowser(SecondaryProgramBrowserView *self)
{
    BrowserWorkerGuard worker = { &self->worker };
    ClearSecondaryProgramBrowserFiles(self);
    RemoveBrowserChain(self->updateChain);
    RemoveBrowserChain(self->drawChain);
    if (self->loadedData)
    {
        free(self->loadedData);
        self->loadedData = 0;
    }
    if (self->program)
    {
        delete self->program;
        self->program = 0;
    }
    if (self->manager)
    {
        delete self->manager;
        self->manager = 0;
    }
    ReleaseBrowserAnimation(6);
    ReleaseBrowserAnimation(5);
    g_SecondaryProgramBrowser = 0;

}
