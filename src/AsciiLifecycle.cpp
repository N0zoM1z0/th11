#include "AsciiLifecycle.hpp"
#include "AnimationVm.hpp"
#include "AnimationSpriteBinding.hpp"
#include "ChainRegistration.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <string.h>

AsciiManagerObject::AsciiManagerObject()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    g_AsciiManager = this;
    color = 0xffffffff;
    unknown18490 = 0;
    drawMode = 9;
}

namespace
{
struct AsciiChainElement : ChainElement
{
    __forceinline AsciiChainElement()
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

__forceinline AsciiChainElement *MakeAsciiChain(ChainCallback callback, AsciiManagerObject *self)
{
    AsciiChainElement *element = new AsciiChainElement;
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = self;
    element->flags = (element->flags & ~2u) | 1;
    return element;
}

__forceinline void RemoveAsciiChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element)
        return;
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

__forceinline void ReleaseAsciiDocument(int index)
{
    AnimationDocumentView **slot =
        &reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch)->files[index];
    if (*slot)
    {
        DestroyAnimationDocument(*slot);
        operator delete(*slot);
        *slot = 0;
    }
}
}

int __stdcall InitializeAsciiManager(AsciiManagerObject *self)
{
    self->animation = AcquireAnimationDocument(2, g_AsciiAnimationFilename);
    if (!self->animation)
    {
        AppendErrorLog(&g_RuntimeLog, g_AsciiAnimationLoadError);
        return -1;
    }
    AsciiChainElement *element = MakeAsciiChain(
        reinterpret_cast<ChainCallback>(AsciiManagerOnUpdate), self);
    AddToUpdateChain(element, 4);
    self->updateChain = element;
    element = MakeAsciiChain(reinterpret_cast<ChainCallback>(AsciiManagerDrawCallback), self);
    AddToDrawChain(element, 0x42);
    self->drawChain = element;
    element = MakeAsciiChain(reinterpret_cast<ChainCallback>(AsciiManagerOverlayCallback), self);
    AddToDrawChain(element, 0x2b);
    self->overlayChain = element;
    AnimationDocumentView *file = self->animation;
    AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(&self->textVm);
    ResetAnimationVm(vm);
    vm->file = reinterpret_cast<AnimationFileView *>(file);
    BindAnimationSprite(file, 0, vm);
    file = self->animation;
    vm = reinterpret_cast<AnimationVmView *>(&self->overlayVm);
    ResetAnimationVm(vm);
    vm->file = reinterpret_cast<AnimationFileView *>(file);
    BindAnimationSprite(file, 0x62, vm);
    return 0;
}

AsciiManagerObject::~AsciiManagerObject()
{
    RemoveAsciiChain(updateChain);
    RemoveAsciiChain(drawChain);
    RemoveAsciiChain(overlayChain);
    ReleaseAsciiDocument(2);
    ReleaseAsciiDocument(0);
    g_AsciiManager = 0;
}

int __fastcall AsciiManagerDrawCallback(AsciiManagerObject *self)
{
    return DrawAsciiManager(self, 0);
}

int __fastcall AsciiManagerOverlayCallback(AsciiManagerObject *self)
{
    return DrawAsciiOverlay(self);
}
