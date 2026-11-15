#include "StageUpdate.hpp"
#include "StageResource.hpp"

// Export both private-register entry points into one receipt-bound VC9 context.
// The probe is never executed; EBX/EAX are supplied by the original callers.
extern "C" __declspec(naked) void __stdcall StageUpdateProbe(void)
{
    __asm
    {
        call UpdateStageObjects
        call UpdateStage
        ret
    }
}
