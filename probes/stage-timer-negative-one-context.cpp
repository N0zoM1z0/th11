#include "../src/StageResource.hpp"

struct StageTimerNegativeOneView : StageTimerView
{
    int OffsetByOneFrame();
};

extern "C" __declspec(dllexport) int __stdcall StageTimerNegativeOneProbe(
    StageTimerView *timer)
{
    return reinterpret_cast<StageTimerNegativeOneView *>(timer)->OffsetByOneFrame();
}
