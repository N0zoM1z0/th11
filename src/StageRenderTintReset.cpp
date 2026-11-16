#include "StageDrawing.hpp"

// The stage renderer's two-word tint tail is restored to its neutral state.
// This physical view is shared with the stage drawing code.
void __cdecl ResetStageRenderTint()
{
    StageRenderTintView &tint = *reinterpret_cast<StageRenderTintView *>(g_RenderBatch);
    tint.enabled = 0;
    tint.tint = 0x80808080;
}
