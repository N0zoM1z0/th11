#include "SecondaryDebugSessionLifecycle.hpp"
#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryClearing.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryPlayerWrapItems.hpp"
#include "ReplayFileBrowserLifetime.hpp"
#include "StageAuxiliary.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <stdlib.h>

// Original independent component ownership boundaries. Only existing
// game-owned singleton pointers are passed; no substitute global storage,
// ASM shim, or invented destructor implementation is introduced here.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
extern SecondaryEclDebugOwner *g_SecondaryDebugSession;
void __stdcall DestructSecondaryPlayerStorage(SecondaryPlayerControllerView *self);
void __stdcall DestroySceneProjectilePool(SceneClearPoolView *self);
void __stdcall DestroySecondaryMaster(SecondaryMasterView *self);
void __stdcall DestroySecondaryAttractionItems(SecondaryAttractionManagerView *self);

// The real ThreadInf-derived member destructor returns its thread to the
// runtime, after the owner's C++ body has completed.
SecondaryDebugThreadOwner::~SecondaryDebugThreadOwner()
{
    StopRuntimeThread(reinterpret_cast<RuntimeThreadView *>(this));
}

namespace
{
__forceinline void UnregisterDebugChain(ChainElement *element)
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
    // The native rechecks this flag after the callback returns.
    if (g_MainRuntime.flags & 0x8000u)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

// Complete native 0x40D320..0x40D4ED debug session destruction.
// Component destructor and operator-delete calls are kept separate, so
// each original side effect and singleton invalidation remains observable.
SecondaryEclDebugOwner::~SecondaryEclDebugOwner()
{
    ReleaseSecondaryDebugFilenames(
        reinterpret_cast<SecondaryEclDebugView *>(this));
    UnregisterDebugChain(updateChain);
    UnregisterDebugChain(drawChain);

    SecondaryDebugHudOwned *hud =
        reinterpret_cast<SecondaryDebugHudOwned *>(g_SecondaryActorHud);
    if (hud)
        delete hud;

    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    if (player)
    {
        DestructSecondaryPlayerStorage(player);
        ::operator delete(player);
    }

    SceneClearPoolView *projectiles = g_SceneClearPool;
    if (projectiles)
    {
        DestroySceneProjectilePool(projectiles);
        ::operator delete(projectiles);
    }

    SecondaryMasterView *master = g_SecondaryMaster;
    if (master)
    {
        DestroySecondaryMaster(master);
        ::operator delete(master);
    }

    StageAuxiliaryView *auxiliary =
        reinterpret_cast<StageAuxiliaryView *>(g_MeshEffectEnemy);
    if (auxiliary)
        delete auxiliary;

    SecondaryAttractionManagerView *attraction = g_SecondaryAttractionItems;
    if (attraction)
    {
        DestroySecondaryAttractionItems(attraction);
        ::operator delete(attraction);
    }

    ReplayFileBrowserView *replay = g_ReplayFileBrowser;
    if (replay)
    {
        DestroyReplayFileBrowser(replay);
        ::operator delete(replay);
    }

    g_SecondaryDebugSession = 0;
    if (ownedData)
        free(ownedData);
    ownedData = 0;

    // As in original C++, the embedded thread's actual virtual destructor
    // is invoked automatically by the compiler after this body.
}
