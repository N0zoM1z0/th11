#include "StageResource.hpp"

__declspec(naked) int __fastcall StageUpdateCallback(void *context)
{
    __asm
    {
        mov eax, ecx
        jmp UpdateStage
    }
}

int __fastcall StageDrawCallback(void *context)
{
    return DrawStage(static_cast<StageResourceView *>(context));
}

int __fastcall StageOverlayCallback(void *context)
{
    return DrawStageOverlay(static_cast<StageResourceView *>(context));
}
