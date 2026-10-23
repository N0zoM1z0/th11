// Non-runnable compiler context for independent observations of the clock.
#include "../src/FrameClock.hpp"

extern "C" double __stdcall FrameClockProbe()
{
    double first = GetFrameClockSeconds();
    return first + GetFrameClockSeconds();
}
