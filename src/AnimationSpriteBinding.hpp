#pragma once
#include "AnimationDocument.hpp"
#include "AnimationVm.hpp"

int __stdcall BindAnimationSprite(AnimationDocumentView *file, int sprite, AnimationVmView *vm);

// Four UV corners plus the selected-sprite pointer; no dimensions or reset.
void __stdcall SetAnimationSpriteUv(AnimationVmView *vm, AnimationSpriteView *sprite);
