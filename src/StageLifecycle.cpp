#include "StageResource.hpp"
#include "ChainRegistration.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>
#include <string.h>

StageEmbeddedVm::StageEmbeddedVm()
{
    memset(this, 0, sizeof(*this));
    marker39c = -1;
}

StageEmbeddedVm::~StageEmbeddedVm()
{
    if (storage400)
        free(storage400);
    storage400 = 0;
}

StageResourceView::StageResourceView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
}

namespace
{
struct StageChainElement : ChainElement
{
    __forceinline StageChainElement()
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

__forceinline StageChainElement *MakeStageChain(ChainCallback callback, StageResourceView *stage)
{
    StageChainElement *element = new StageChainElement;
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = stage;
    element->flags = (element->flags & ~2u) | 1;
    return element;
}

__forceinline CameraVector MakeStageVector(float x, float y, float z)
{
    CameraVector result = { x, y, z };
    return result;
}

__forceinline void RemoveStageChain(ChainElement *element)
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
}

int __stdcall InitializeStageResource(StageResourceView *self, const char *filename)
{
    self->variant = g_StageVariant;
    g_StagePrimary = self;
    if (LoadStageResource(self, filename))
    {
        AppendErrorLog(&g_RuntimeLog, g_StageLoadError);
        return -1;
    }
    self->camera = *reinterpret_cast<const StageCameraView *>(g_MainRuntime.unknown00c + 0x428);
    self->camera.view.position = MakeStageVector(0.0f, 0.0f, -600.0f);
    self->camera.view.targetOffset = MakeStageVector(0.0f, 300.0f, 600.0f);
    self->camera.view.up = MakeStageVector(0.0f, 1.0f, 0.0f);
    self->camera.view.eyeOffset = MakeStageVector(0.0f, 0.0f, 0.0f);
    self->distanceSquared = 9610000.0f;

    StageChainElement *element = MakeStageChain(StageUpdateCallback, self);
    AddToUpdateChain(element, 12);
    self->updateChain = element;
    element = MakeStageChain(StageDrawCallback, self);
    AddToDrawChain(element, 2);
    self->drawChain = element;
    element = MakeStageChain(StageOverlayCallback, self);
    AddToDrawChain(element, 5);
    self->overlayChain = element;
    self->reset300c = 0;
    self->time.SetFrame(-1);
    self->flags2ff0 |= 1;
    self->reset094 = 0;
    self->reset0e0 = 0;
    return 0;
}

StageResourceView::~StageResourceView()
{
    RemoveStageChain(updateChain);
    RemoveStageChain(drawChain);
    RemoveStageChain(overlayChain);
    if (data)
    {
        free(data);
        data = 0;
    }
    if (source)
    {
        free(source);
        source = 0;
    }
    source = 0;
    if (vms)
    {
        free(vms);
        vms = 0;
    }
    VmBindingBlockView *block = bindings;
    if (block)
    {
        ReleaseVmBindingBlock(block);
        operator delete(block);
        bindings = 0;
    }
    if (!(g_StageResourceFlags & 1))
    {
        int index = (variant & 1) + 3;
        if (index >= 0 && index < 32)
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
    if (g_StagePrimary == this)
        g_StagePrimary = 0;
    if (g_StageSecondary == this)
        g_StageSecondary = 0;
}

StageResourceView *__stdcall CreateStageResource(const char *filename)
{
    StageResourceView *self = new StageResourceView;
    if (InitializeStageResource(self, filename))
    {
        delete self;
        return 0;
    }
    return self;
}
