#include "../src/StageTimerComparison.hpp"

extern "C" __declspec(dllexport) int __stdcall StageTimerComparisonProbe(
    StageTimerComparisonView *timer, int frame)
{
    return timer->IsAtOrBefore(frame);
}
