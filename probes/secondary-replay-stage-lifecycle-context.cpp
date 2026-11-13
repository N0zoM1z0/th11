#include "../src/SecondaryReplayStageLifecycle.hpp"
extern "C" void __stdcall SecondaryReplayStageLifecycleProbe()
{
    UpdateSecondaryReplayStage();
}
