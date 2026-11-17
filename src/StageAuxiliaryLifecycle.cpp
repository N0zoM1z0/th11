#include "StageAuxiliary.hpp"
#include "ChainRegistration.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <stdlib.h>
#include <string.h>

// These two game helpers are recorded at 0x00420A00 and 0x004562E0.
// Their ordinary declarations do not assert the private LTCG register ABI.
void __stdcall ReleaseSecondaryAnimation(AnimationVmId id);
void __stdcall ResetSecondaryStageEffect(int type);

namespace
{
struct StageAuxiliaryChainElement : ChainElement
{
    StageAuxiliaryChainElement()
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

__forceinline StageAuxiliaryChainElement *CreateAuxiliaryChain(
    ChainCallback callback, StageAuxiliaryView *stage)
{
    StageAuxiliaryChainElement *element = new StageAuxiliaryChainElement;
    element->flags |= 3;
    element->SetCallback(callback);
    element->context = stage;
    return element;
}

__forceinline void RemoveAuxiliaryChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element)
        return;

    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(manager, element);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

StageAuxiliaryView::StageAuxiliaryView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    g_StageAuxiliary = this;
}

int __stdcall RegisterStageAuxiliaryChains(StageAuxiliaryView *self)
{
    StageAuxiliaryChainElement *update =
        CreateAuxiliaryChain(reinterpret_cast<ChainCallback>(StageAuxiliaryUpdateCallback), self);
    AddToUpdateChain(update, 17);
    self->updateChain = update;

    StageAuxiliaryChainElement *draw =
        CreateAuxiliaryChain(StageAuxiliaryDrawCallback, self);
    AddToDrawChain(draw, 33);
    self->drawChain = draw;
    return 0;
}

StageAuxiliaryView::~StageAuxiliaryView()
{
    if (effectActive && g_StagePlayerCharacter + 3 * g_StagePlayerPartner == 5)
    {
        ReleaseSecondaryAnimation(AnimationVmId(animationId040));
        animationId040 = 0;
        ResetSecondaryStageEffect(10);
    }

    ReleaseSecondaryAnimation(AnimationVmId(animationId040));
    animationId040 = 0;
    ReleaseSecondaryAnimation(AnimationVmId(animationId044));
    animationId044 = 0;

    RemoveAuxiliaryChain(updateChain);
    RemoveAuxiliaryChain(drawChain);
    RemoveAuxiliaryChain(overlayChain);

    g_StageAuxiliary = 0;
    if (bindings)
    {
        ReleaseVmBindingBlock(bindings);
        operator delete(bindings);
    }
    bindings = 0;
    if (cachedData)
    {
        free(cachedData);
        cachedData = 0;
    }
    // The embedded VM's real C++ destructor subsequently frees vm.storage400.
}

StageAuxiliaryView *CreateStageAuxiliary()
{
    StageAuxiliaryView *self = new StageAuxiliaryView;
    if (RegisterStageAuxiliaryChains(self))
    {
        delete self;
        return 0;
    }
    return self;
}

void DestroyStageAuxiliary()
{
    StageAuxiliaryView *self = g_StageAuxiliary;
    if (self)
        delete self;
}

int __fastcall StageAuxiliaryUpdateCallback(StageAuxiliaryView *self)
{
    return UpdateStageAuxiliary(self);
}

int __fastcall StageAuxiliaryDrawCallback(void *)
{
    return 1;
}
