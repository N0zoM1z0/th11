#include "StageDrawing.hpp"

extern "C" int __stdcall StageDrawingProbe(StageResourceView *first, StageResourceView *second)
{
    int result = DrawStage(first) + DrawStage(second);
    result += DrawStageOverlay(first) + DrawStageOverlay(second);
    return result;
}
