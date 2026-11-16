#include "../src/StageResource.hpp"
int __stdcall OffsetStageTimerByFrames(StageTimerView *time, int frames);
extern "C" __declspec(dllexport) int __stdcall StageTimerIntegerOffsetProbe(StageTimerView *time, int frames)
{
    return OffsetStageTimerByFrames(time, frames);
}
