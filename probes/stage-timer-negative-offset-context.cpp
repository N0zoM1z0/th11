#include "../src/StageResource.hpp"

struct StageTimerNegativeOffsetView : StageTimerView
{
    int OffsetByNegativeFrames(int frames);
};

extern "C" __declspec(dllexport) int __stdcall StageTimerNegativeOffsetProbe(
    StageTimerView *timer, int frames)
{
    return reinterpret_cast<StageTimerNegativeOffsetView *>(timer)->OffsetByNegativeFrames(frames);
}
