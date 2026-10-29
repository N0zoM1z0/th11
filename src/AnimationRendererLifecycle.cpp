#include "AnimationRendererLifecycle.hpp"
#include "ChainRegistration.hpp"
#include "MainRuntime.hpp"
#include <string.h>
#include <stdlib.h>
#include <new>

// Suffixes denote registration priority, not layer number. Bodies remain open.
int __fastcall AnimationRendererUpdatePriority26(void *context);
int __fastcall AnimationRendererUpdatePriority08(void *context);
int __fastcall AnimationRendererDrawPriority04(void *context);
int __fastcall AnimationRendererDrawPriority06(void *context);
int __fastcall AnimationRendererDrawPriority08(void *context);
int __fastcall AnimationRendererDrawPriority10(void *context);
int __fastcall AnimationRendererDrawPriority12(void *context);
int __fastcall AnimationRendererDrawPriority15(void *context);
int __fastcall AnimationRendererDrawPriority16(void *context);
int __fastcall AnimationRendererDrawPriority17(void *context);
int __fastcall AnimationRendererDrawPriority18(void *context);
int __fastcall AnimationRendererDrawPriority19(void *context);
int __fastcall AnimationRendererDrawPriority21(void *context);
int __fastcall AnimationRendererDrawPriority23(void *context);
int __fastcall AnimationRendererDrawPriority24(void *context);
int __fastcall AnimationRendererDrawPriority26(void *context);
int __fastcall AnimationRendererDrawPriority28(void *context);
int __fastcall AnimationRendererDrawPriority32(void *context);
int __fastcall AnimationRendererDrawPriority34(void *context);
int __fastcall AnimationRendererDrawPriority36(void *context);
int __fastcall AnimationRendererDrawPriority39(void *context);
int __fastcall AnimationRendererDrawPriority40(void *context);
int __fastcall AnimationRendererDrawPriority50(void *context);
int __fastcall AnimationRendererDrawPriority60(void *context);
int __fastcall AnimationRendererDrawPriority61(void *context);
int __fastcall AnimationRendererDrawPriority49(void *context);
int __fastcall AnimationRendererDrawPriority48(void *context);
int __fastcall AnimationRendererDrawPriority63(void *context);
int __fastcall AnimationRendererDrawPriority62(void *context);

namespace
{
struct RendererChainElement : ChainElement
{
    __forceinline RendererChainElement()
    {
        flags &= ~1u;
        callback = unknown0c = auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = link.previous = 0;
    }
};
__forceinline ChainElement *MakeRendererChain(ChainCallback callback, AnimationRenderer *self)
{
    RendererChainElement *element = new RendererChainElement;
    element->flags |= 3;
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = self;
    return element;
}
__forceinline IDirect3DDevice9 *Device()
{
    return static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice);
}
}

AnimationRenderer::AnimationRenderer()
{
    memset(this, 0, sizeof(*this));
    g_AnimationScratchQuad[0].w = g_AnimationScratchQuad[1].w =
        g_AnimationScratchQuad[2].w = g_AnimationScratchQuad[3].w = 1.0f;
    g_AnimationScratchQuad[0].u = g_AnimationScratchQuad[0].v =
        g_AnimationScratchQuad[1].v = g_AnimationScratchQuad[2].u = 0.0f;
    g_AnimationScratchQuad[1].u = g_AnimationScratchQuad[2].v =
        g_AnimationScratchQuad[3].u = g_AnimationScratchQuad[3].v = 1.0f;
    g_AnimationQuad[0].rhw = g_AnimationQuad[1].rhw =
        g_AnimationQuad[2].rhw = g_AnimationQuad[3].rhw = 1.0f;
    g_AnimationQuad[0].u = g_AnimationQuad[0].v =
        g_AnimationQuad[1].v = g_AnimationQuad[2].u = 0.0f;
    g_AnimationQuad[1].u = g_AnimationQuad[2].v =
        g_AnimationQuad[3].u = g_AnimationQuad[3].v = 1.0f;
    worldBuffer = 0;
    texture = 0;
    blend = cachedC1 = geometry = cachedC3 = 0;
    textureFactor = 1;
    cachedC4 = 0xff;
    prefix[0].marker = prefix[1].marker = prefix[2].marker = prefix[3].marker = -1;
    for (int i = 0; i < 0x1000; ++i)
        ResetAnimationVm(reinterpret_cast<AnimationVmView *>(&vms[i]));
    AddToUpdateChain(MakeRendererChain(AnimationRendererUpdatePriority26, this), 26);
    AddToUpdateChain(MakeRendererChain(AnimationRendererUpdatePriority08, this), 8);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority04, this), 4);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority06, this), 6);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority08, this), 8);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority10, this), 10);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority12, this), 12);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority15, this), 15);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority16, this), 16);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority17, this), 17);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority18, this), 18);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority19, this), 19);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority21, this), 21);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority23, this), 23);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority24, this), 24);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority26, this), 26);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority28, this), 28);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority32, this), 32);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority34, this), 34);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority36, this), 36);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority39, this), 39);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority40, this), 40);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority50, this), 50);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority60, this), 60);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority61, this), 61);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority49, this), 49);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority48, this), 48);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority63, this), 63);
    AddToDrawChain(MakeRendererChain(AnimationRendererDrawPriority62, this), 62);
    Device()->SetVertexShader(0);
}

AnimationRenderer::~AnimationRenderer()
{
    AnimationVmNode *node = activeHead;
    while (node)
    {
        AnimationVmNode *next = node->next;
        DestroyRendererAnimationVm(this, node->vm);
        node = next;
    }
    node = otherHead;
    while (node)
    {
        AnimationVmNode *next = node->next;
        DestroyRendererAnimationVm(this, node->vm);
        node = next;
    }
}

void __cdecl InitializeAnimationWorldBuffer()
{
    AnimationRenderer *self = reinterpret_cast<AnimationRenderer *>(g_RenderBatch);
    AnimationWorldVertex *vertices = self->corners;
    self->corners[0].position.x = self->corners[2].position.x = -128.0f;
    self->corners[1].position.x = self->corners[3].position.x = 128.0f;
    self->corners[0].position.y = self->corners[1].position.y = -128.0f;
    self->corners[2].position.y = self->corners[3].position.y = 128.0f;
    self->corners[0].position.z = self->corners[1].position.z =
        self->corners[2].position.z = self->corners[3].position.z = 0.0f;
    self->corners[0].u = self->corners[2].u = 0.0f;
    self->corners[1].u = self->corners[3].u = 1.0f;
    self->corners[0].v = self->corners[1].v = 0.0f;
    self->corners[2].v = self->corners[3].v = 1.0f;
    g_AnimationProjectedScratchQuad[0].position = self->corners[0].position;
    g_AnimationProjectedScratchQuad[1].position = self->corners[1].position;
    g_AnimationProjectedScratchQuad[2].position = self->corners[2].position;
    g_AnimationProjectedScratchQuad[3].position = self->corners[3].position;
    g_AnimationProjectedScratchQuad[0].u = self->corners[0].u;
    g_AnimationProjectedScratchQuad[0].v = self->corners[0].v;
    g_AnimationProjectedScratchQuad[1].u = self->corners[1].u;
    g_AnimationProjectedScratchQuad[1].v = self->corners[1].v;
    g_AnimationProjectedScratchQuad[2].u = self->corners[2].u;
    g_AnimationProjectedScratchQuad[2].v = self->corners[2].v;
    g_AnimationProjectedScratchQuad[3].u = self->corners[3].u;
    g_AnimationProjectedScratchQuad[3].v = self->corners[3].v;
    Device()->CreateVertexBuffer(80, 0, D3DFVF_XYZ | D3DFVF_TEX1,
        D3DPOOL_MANAGED, &self->worldBuffer, 0);
    void *destination;
    self->worldBuffer->Lock(0, 0, &destination, 0);
    memcpy(destination, vertices, 80);
    self->worldBuffer->Unlock();
    Device()->SetStreamSource(0, self->worldBuffer, 0, sizeof(AnimationWorldVertex));
}

int __stdcall DestroyRendererAnimationVm(AnimationRenderer *renderer, AnimationVmView *vm)
{
    AnimationVmNode *node = &vm->updateNode;
    if (node == renderer->activeTail) renderer->activeTail = vm->updateNode.previous;
    if (node == renderer->activeHead) renderer->activeHead = vm->updateNode.next;
    if (node == renderer->otherTail) renderer->otherTail = vm->updateNode.previous;
    if (node == renderer->otherHead) renderer->otherHead = vm->updateNode.next;
    if (node->next) node->next->previous = node->previous;
    if (node->previous) node->previous->next = node->next;
    node->next = 0;
    node->previous = 0;
    node = &vm->drawNode;
    if (node->next) node->next->previous = node->previous;
    if (node->previous) node->previous->next = node->next;
    node->next = 0;
    node->previous = 0;
    unsigned int address = reinterpret_cast<unsigned int>(vm);
    unsigned int first = reinterpret_cast<unsigned int>(&renderer->vms[0]);
    unsigned int end = reinterpret_cast<unsigned int>(&renderer->vms[0x1000]);
    StageEmbeddedVm *storage = reinterpret_cast<StageEmbeddedVm *>(vm);
    if (address >= first && address < end)
    {
        int index = static_cast<int>(address - reinterpret_cast<unsigned int>(renderer) -
            offsetof(AnimationRenderer, vms)) / static_cast<int>(sizeof(StageEmbeddedVm));
        renderer->used[index] = 0;
        if (storage->storage400) free(storage->storage400);
        storage->storage400 = 0;
        ResetAnimationVm(vm);
    }
    else if (vm)
    {
        if (storage->storage400) free(storage->storage400);
        storage->storage400 = 0;
        operator delete(vm);
    }
    return 0;
}
