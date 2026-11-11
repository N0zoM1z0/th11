#include "PauseOverlayRectangle.hpp"

// TH11 0042F400: scan four forty-byte rectangle records and populate the
// first whose texture field is negative. There is no overflow fallback.
int __stdcall QueuePauseOverlayRectangle(RenderBatchView *renderer,
    int texture, int textureIndex, int x, int y, int width, int height)
{
    PauseOverlayRectangleSlot *slots =
        reinterpret_cast<PauseOverlayRectangleSlot *>(renderer);
    unsigned int index = 0;
    PauseOverlayRectangleSlot *cursor = slots;
    for (; index < 4; ++index, ++cursor)
        if (cursor->texture < 0) goto insert;
    return 0;
insert:
    PauseOverlayRectangleSlot *slot = slots + index;
    slot->texture = texture;
    slot->textureIndex = textureIndex;
    slot->minX = 0x20;
    slot->minY = 0x10;
    slot->maxX = 0x180;
    slot->maxY = 0x1c0;
    slot->x = x;
    slot->y = y;
    slot->width = width;
    slot->height = height;
    return 0;
}

namespace
{
struct PauseOverlayVmView
{
    unsigned char unknown000[0x3ac];
    AnimationSpriteView *sprite;
    AnimationDocumentTextureView *texture;
};
typedef char PauseSpriteHeightOffset[(offsetof(AnimationSpriteView, height) == 0x34) ? 1 : -1];
typedef char PauseSpriteWidthOffset[(offsetof(AnimationSpriteView, width) == 0x38) ? 1 : -1];
typedef char PauseSpriteXOffset[(offsetof(AnimationSpriteView, left) == 0xc) ? 1 : -1];
typedef char PauseOverlayVmSpriteOffset[(offsetof(PauseOverlayVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char PauseOverlayVmTextureOffset[(offsetof(PauseOverlayVmView, texture) == 0x3b0) ? 1 : -1];
}

// TH11 0042F480: convert the sprite's four floating rectangle coordinates
// to integers and append a draw slot. The native dereferences VM/sprite
// without adding a new null check.
void __stdcall CapturePauseOverlayFromVm(AnimationVmId id)
{
    PauseOverlayVmView *vm = reinterpret_cast<PauseOverlayVmView *>(
        FindAnimationVmById(g_RenderBatch, id));
    if (!vm) id.value = 0;
    AnimationSpriteView *sprite = vm->sprite;
    int height = static_cast<int>(sprite->height);
    int width = static_cast<int>(sprite->width);
    int y = static_cast<int>(sprite->top);
    int x = static_cast<int>(sprite->left);
    QueuePauseOverlayRectangle(g_RenderBatch,
        reinterpret_cast<int>(vm->texture->texture), sprite->textureIndex,
        x, y, width, height);
}
