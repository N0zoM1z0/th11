#include "MenuJoystick.hpp"
#include "AnimationSpriteBinding.hpp"

namespace
{
__forceinline AnimationVmId ChildId(AnimationVmId *parentId,
    RenderBatchView *renderer, short script)
{
    AnimationVmView *parent = FindAnimationVmById(renderer, *parentId);
    if (!parent) *parentId = AnimationVmId(0);
    for (AnimationVmNode *node = &parent->drawNode; node; node = node->next)
        if (static_cast<short>(node->vm->scriptId) == script)
            return AnimationVmId(node->vm->unknown000);
    return AnimationVmId(0);
}
__forceinline void Digit(MenuControllerView *self, RenderBatchView *renderer,
    short script, int binding, bool tens)
{
    AnimationVmId id = ChildId(&self->joystickRootVmId, renderer, script);
    AnimationVmView *vm = FindAnimationVmById(renderer, id);
    if (vm)
    {
        int value = tens ? self->joystickBindings[binding] / 10 :
            self->joystickBindings[binding] % 10;
        BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file),
            value + 0x2a, vm);
    }
}
}

void __stdcall ApplyMenuJoystickBindings(MenuControllerView *self)
{
    RenderBatchView *renderer = g_RenderBatch;
    Digit(self, renderer, 0x3b, 0, true);
    Digit(self, renderer, 0x3c, 0, false);
    Digit(self, renderer, 0x45, 0, true);
    Digit(self, renderer, 0x46, 0, false);
    Digit(self, renderer, 0x3d, 1, true);
    Digit(self, renderer, 0x3e, 1, false);
    Digit(self, renderer, 0x47, 1, true);
    Digit(self, renderer, 0x48, 1, false);
    Digit(self, renderer, 0x3f, 2, true);
    Digit(self, renderer, 0x40, 2, false);
    Digit(self, renderer, 0x49, 2, true);
    Digit(self, renderer, 0x4a, 2, false);
    Digit(self, renderer, 0x41, 3, true);
    Digit(self, renderer, 0x42, 3, false);
    Digit(self, renderer, 0x4b, 3, true);
    Digit(self, renderer, 0x4c, 3, false);
    Digit(self, renderer, 0x43, 4, true);
    Digit(self, renderer, 0x44, 4, false);
    Digit(self, renderer, 0x4d, 4, true);
    Digit(self, renderer, 0x4e, 4, false);
}
