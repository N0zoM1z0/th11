#include "StageResource.hpp"

int __fastcall StageUpdateCallback(void *context)
{
    return UpdateStage(static_cast<StageResourceView *>(context));
}

int __fastcall StageDrawCallback(void *context)
{
    return DrawStage(static_cast<StageResourceView *>(context));
}

int __fastcall StageOverlayCallback(void *context)
{
    return DrawStageOverlay(static_cast<StageResourceView *>(context));
}
