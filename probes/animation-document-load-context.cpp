#include "AnimationDocument.hpp"
// A compile-only ordinary producer context for the original document load
// operation; no target register assignments, no executable replacement.
extern "C" AnimationDocumentView *__stdcall AnimationDocumentLoadProbe(
    RenderBatchView *renderer, const char *filename, int slot)
{
    return BeginLoadAnimationDocument(renderer, filename, slot);
}
