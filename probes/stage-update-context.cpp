#include "StageUpdate.hpp"
#include "StageResource.hpp"

// Linker-only entry for the stage-update diagnostic context. It is not run.
extern "C" __declspec(noinline) int __stdcall StageUpdateProbe(StageResourceView *stage)
{
    return UpdateStage(stage);
}
