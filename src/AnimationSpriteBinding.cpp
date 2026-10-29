#include "AnimationSpriteBinding.hpp"

namespace
{
// This read-only binding path observes the ready flag as an ordinary int.
struct SpriteBindingFileView
{
    unsigned char unknown000[0x108];
    AnimationChunkHeaderView *data;
    int counts[3];
    AnimationSpriteView *sprites;
    unsigned char unknown11c[8];
    int busy;
};

struct SpriteUv
{
    float u, v;
};

struct SpriteBindingVmView
{
    unsigned char unknown000[0x4c];
    float width, height;
    unsigned char unknown054[0x70 - 0x54];
    SpriteUv corners[4];
    unsigned char unknown090[0x2b4 - 0x90];
    D3DMATRIX transform, savedTransform, textureTransform;
    unsigned char unknown374[0x39c - 0x374];
    short spriteId;
    unsigned char unknown39e[0x3ac - 0x39e];
    AnimationSpriteView *sprite;
    AnimationDocumentView *file;
};

typedef char SpriteBindingReadyOffset[(offsetof(SpriteBindingFileView, busy) == 0x124) ? 1 : -1];
typedef char SpriteBindingUvOffset[(offsetof(SpriteBindingVmView, corners) == 0x70) ? 1 : -1];
typedef char SpriteBindingMarkerOffset[(offsetof(SpriteBindingVmView, spriteId) == 0x39c) ? 1 : -1];
typedef char SpriteBindingTransformOffset[(offsetof(SpriteBindingVmView, transform) == 0x2b4) ? 1 : -1];
typedef char SpriteBindingFileOffset[(offsetof(SpriteBindingVmView, file) == 0x3b0) ? 1 : -1];

__forceinline void Identity(D3DMATRIX &m)
{
    m._12 = m._13 = m._14 = m._21 = m._23 = m._24 =
    m._31 = m._32 = m._34 = m._41 = m._42 = m._43 = 0.0f;
    m._11 = m._22 = m._33 = m._44 = 1.0f;
}
}

int __stdcall BindAnimationSprite(AnimationDocumentView *file, int sprite, AnimationVmView *vm)
{
    const SpriteBindingFileView *source = reinterpret_cast<const SpriteBindingFileView *>(file);
    if (!source->data || source->busy)
        return -1;
    SpriteBindingVmView *self = reinterpret_cast<SpriteBindingVmView *>(vm);
    self->spriteId = static_cast<short>(sprite);
    self->file = file;
    self->sprite = &source->sprites[sprite];
    self->corners[0].u = self->corners[2].u = self->sprite->u0;
    self->corners[1].u = self->corners[3].u = self->sprite->u1;
    self->corners[0].v = self->corners[1].v = self->sprite->v0;
    self->corners[2].v = self->corners[3].v = self->sprite->v1;
    self->width = self->sprite->width;
    self->height = self->sprite->height;
    Identity(self->transform);
    Identity(self->textureTransform);
    self->transform._11 = self->width * 0.5f;
    self->transform._22 = self->height * 0.5f;
    self->textureTransform._11 = self->width / self->sprite->textureWidth * self->sprite->scaleX;
    self->textureTransform._22 = self->height / self->sprite->textureHeight * self->sprite->scaleY;
    self->savedTransform = self->transform;
    return 0;
}

void __stdcall SetAnimationSpriteUv(AnimationVmView *vm, AnimationSpriteView *sprite)
{
    SpriteBindingVmView *self = reinterpret_cast<SpriteBindingVmView *>(vm);
    self->sprite = sprite;
    self->corners[0].u = self->corners[2].u = sprite->u0;
    self->corners[1].u = self->corners[3].u = sprite->u1;
    self->corners[0].v = self->corners[1].v = sprite->v0;
    self->corners[2].v = self->corners[3].v = sprite->v1;
}
