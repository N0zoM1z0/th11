#include "StageLayerDrawing.hpp"

// A dynamic source caller for the two game-specific owners. The resulting
// image is a non-runnable codegen diagnostic, not a complete TH11 build.
extern "C" int __stdcall StageLayerProbe(StageResourceView *stage, int layer)
{
    return DrawStageLayer(stage, layer);
}
