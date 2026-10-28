#include "AnimationTexture.hpp"

extern "C" int __stdcall AnimationTextureProbe(AnimationDocumentView *file,
    AnimationDocumentView *other, int index, AnimationChunkHeaderView *chunk,
    const AnimationSpriteView *sprite)
{
    int result = LoadAnimationChunkSource(file, index, chunk);
    result += LoadAnimationChunkSource(other, index, chunk);
    result += AdvanceAnimationDocument(file) != 0;
    result += AdvanceAnimationDocument(other) != 0;
    StoreAnimationSprite(file, index, sprite);
    StoreAnimationSprite(other, index, sprite);
    return result;
}
