#include "AnimationDocument.hpp"

extern "C" int __stdcall AnimationChunkProbe(AnimationDocumentView *file,
    AnimationDocumentView *other, int index, int spriteIndex, int scriptIndex,
    AnimationChunkHeaderView *chunk, const AnimationSpriteView *sprite)
{
    int result = LoadAnimationChunkSource(file, index, chunk);
    result += DecodeAnimationChunk(file, index, spriteIndex, scriptIndex, chunk);
    result += AdvanceAnimationDocument(file) != 0;
    result += AdvanceAnimationDocument(other) != 0;
    StoreAnimationSprite(file, spriteIndex, sprite);
    StoreAnimationSprite(other, index, sprite);
    return result;
}
