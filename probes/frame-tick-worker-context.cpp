#include "FrameTickWorker.hpp"
#include "FrameClock.hpp"
// Actual dynamic clock call makes the original x87 producer visible while the
// complete timer thread remains its own primary translation-unit owner.
extern "C" double __cdecl FrameTickWorkerProbe()
{
    RunFrameTickWorker();
    return GetFrameClockSeconds();
}
