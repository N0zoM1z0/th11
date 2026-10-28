#include "AnimationDocument.hpp"

void __stdcall StoreAnimationSprite(AnimationDocumentView *file, int index,
    const AnimationSpriteView *sprite)
{
    file->sprites[index] = *sprite;
    file->sprites[index].u0 = file->sprites[index].left / file->sprites[index].textureWidth;
    file->sprites[index].u1 = file->sprites[index].right / file->sprites[index].textureWidth;
    file->sprites[index].v0 = file->sprites[index].top / file->sprites[index].textureHeight;
    file->sprites[index].v1 = file->sprites[index].bottom / file->sprites[index].textureHeight;
    file->sprites[index].width = (file->sprites[index].right - file->sprites[index].left) / sprite->scaleX;
    file->sprites[index].height = (file->sprites[index].bottom - file->sprites[index].top) / sprite->scaleY;
}
