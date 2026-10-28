#include "StageScript.hpp"

extern "C" int __stdcall StageScriptProbe(StageResourceView *first, StageResourceView *second,
    StageFogView *fog)
{
    int result = ExecuteStageScript(first);
    result += ExecuteStageScript(second);
    fog->Pack();
    return result;
}
