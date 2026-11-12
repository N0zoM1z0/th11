#include "TitleMenuLifecycle.hpp"
#include "MenuController.hpp"
#include "VmBindings.hpp"
#include "SecondaryActorRuntime.hpp"
#include "ChainRegistration.hpp"
#include "ErrorLog.hpp"
#include <new>
#include <process.h>
#include <stdlib.h>
#include <string.h>

extern const unsigned int g_TitleMenuVtable[];
extern const unsigned int g_SecondaryBrowserWorkerVtable[];
extern const char g_TitleMenuAnimation18[], g_TitleMenuAnimation19[];
extern const char g_TitleMenuAnimationError[];
extern int g_TitleMenuThreadState;
extern int g_TitleMenuInitializationState;
int __fastcall MenuControllerUpdateCallback(MenuControllerView *self);
int __fastcall MenuControllerDrawCallback(MenuControllerView *self);

namespace
{
__forceinline void InitializeTitleSelection(SecondaryDebugSelectionView &selection)
{
    *reinterpret_cast<unsigned int *>(&selection.unknown00c[0x80]) = 0;
    selection.current = 0;
    selection.excludedCount = 0;
    selection.wrapEnabled = 1;
    selection.count = 999;
}

struct TitleMenuChain : ChainElement
{
    TitleMenuChain()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline TitleMenuChain *CreateTitleMenuChain(
    TitleMenuView *menu, ChainCallback callback)
{
    TitleMenuChain *chain = new TitleMenuChain;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = menu;
    chain->flags = (chain->flags & ~2u) | 1u;
    return chain;
}

struct ScopedTitleChainLock
{
    ScopedTitleChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
            ++g_MainRuntime.criticalDepth[0];
        }
    }
    ~ScopedTitleChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
            --g_MainRuntime.criticalDepth[0];
        }
    }
};

__forceinline void RemoveTitleMenuChain(ChainElement *chain)
{
    if (chain)
    {
        ScopedTitleChainLock lock;
        UnregisterChainElement(chain, g_ChainManager);
    }
}

__forceinline void ReleaseTitleMenuAnimation(int index)
{
    AnimationDocumentSlotsView *slots =
        reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
    AnimationDocumentView *file = slots->files[index];
    if (file)
    {
        DestroyAnimationDocument(file);
        ::operator delete(file);
        slots->files[index] = 0;
    }
}

struct ScopedTitleWorker
{
    RuntimeThreadView *worker;
    ~ScopedTitleWorker()
    {
        worker->unknown00 = reinterpret_cast<unsigned int>(g_SecondaryBrowserWorkerVtable);
        StopRuntimeThread(worker);
    }
};

__forceinline void FlagTrackedMenuAnimation(unsigned int id)
{
    if (!id) return;
    BoundVmView *vm = FindBoundVm(id);
    if (!vm) return;
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
    if (!vm->marker018)
        for (BoundVmNodeView *node = vm->related; node; node = node->next)
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
}
}

// 0x439420..0x4394E8. The native member initializers, including the worker,
// precede a whole-object clear; the final flag and singleton write follow it.
TitleMenuView::TitleMenuView()
{
    vtable = g_TitleMenuVtable;
    InitializeTitleSelection(selections[0]);
    InitializeTitleSelection(selections[1]);
    InitializeTitleSelection(selections[2]);
    flags2c0 &= ~1u;
    InitializeTitleSelection(replaySelection);
    worker.unknown00 = reinterpret_cast<unsigned int>(g_SecondaryBrowserWorkerVtable);
    worker.thread = 0;
    worker.unknown08 = 0;
    worker.stopRequested = 0;
    worker.pending = 0;
    memset(this, 0, sizeof(*this));
    flags004 |= 2;
    g_TitleMenu = this;
}

// 0x4395A0..0x4396B2. The title thread acquires two animation documents and
// registers an update/draw pair. A failed first or second load logs and exits
// without unrolling the already registered chain, as in the target.
int __cdecl InitializeTitleMenuResources()
{
    TitleMenuView *menu = g_TitleMenu;
    TitleMenuChain *update = CreateTitleMenuChain(menu,
        reinterpret_cast<ChainCallback>(MenuControllerUpdateCallback));
    AddToUpdateChain(update, 6);
    menu->updateChain = update;

    TitleMenuChain *draw = CreateTitleMenuChain(menu,
        reinterpret_cast<ChainCallback>(MenuControllerDrawCallback));
    AddToDrawChain(draw, 54);
    menu->drawChain = draw;

    menu->file18 = AcquireAnimationDocument(0x18, g_TitleMenuAnimation18);
    if (!menu->file18)
    {
        AppendLog(&g_RuntimeLog.buffer, g_TitleMenuAnimationError);
        return -1;
    }
    menu->file19 = AcquireAnimationDocument(0x19, g_TitleMenuAnimation19);
    if (!menu->file19)
    {
        AppendLog(&g_RuntimeLog.buffer, g_TitleMenuAnimationError);
        return -1;
    }
    menu->selections[0].wrapEnabled = 1;
    g_TitleMenuInitializationState = 0;
    return 0;
}

// 0x4396C0..0x4398F0: the title owner releases the worker, two chain nodes,
// two renderer document slots, all 100 stored records, and a tracked VM tree.
// Keeping the worker guard as an actual local lifetime also models unwind.
TitleMenuView::~TitleMenuView()
{
    ScopedTitleWorker workerGuard = { &worker };
    vtable = g_TitleMenuVtable;
    StopRuntimeThread(&worker);
    RemoveTitleMenuChain(updateChain);
    RemoveTitleMenuChain(drawChain);
    ReleaseTitleMenuAnimation(0x18);
    ReleaseTitleMenuAnimation(0x19);
    for (int i = 0; i < 100; ++i)
    {
        TitleMenuRecordView *record = records[i];
        if (record)
        {
            DestroyTitleMenuRecord(record);
            ::operator delete(record);
        }
    }
    FlagTrackedMenuAnimation(trackedAnimationId);
    if (loadedBlob)
    {
        free(loadedBlob);
        loadedBlob = 0;
    }
    g_TitleMenu = 0;
}

// 0x439900..0x439969: create the 0x5B94 owner, reset the owned worker and
// launch the independent title thread at 0x4394F0.
TitleMenuView *__cdecl StartTitleMenu()
{
    TitleMenuView *menu = new TitleMenuView;
    RuntimeThreadView *thread = &menu->worker;
    g_TitleMenuThreadState = 0;
    StopRuntimeThread(thread);
    thread->unknown18 = reinterpret_cast<unsigned int>(TitleMenuThread);
    thread->pending = 1;
    thread->stopRequested = 0;
    thread->thread = reinterpret_cast<HANDLE>(
        _beginthreadex(0, 0, reinterpret_cast<unsigned int (__stdcall *)(void *)>(TitleMenuThread),
            menu, 0, &thread->unknown08));
    return menu;
}

// 0x439970..0x43998B: singleton deletion is null-guarded, without publishing
// a second reset (the destructor itself clears the global pointer).
void __cdecl DeleteTitleMenu()
{
    TitleMenuView *menu = g_TitleMenu;
    if (menu) delete menu;
}
