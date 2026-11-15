#include "StageScript.hpp"

extern "C" int __stdcall StageScriptProbe(StageResourceView *stage,
    StageResourceView *unusedStage, StageFogView *unusedFog)
{
    (void)unusedStage;
    (void)unusedFog;
    return ExecuteStageScript(stage);
}
