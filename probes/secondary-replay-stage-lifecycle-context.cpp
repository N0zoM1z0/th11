#include "../src/SecondaryReplayStageLifecycle.hpp"
extern void __stdcall InitializeSecondaryReplay(void);
extern void __stdcall RecordSecondaryReplayFrame(void);
extern void __stdcall WritePauseMenuReplay(void);
extern "C" void __stdcall SecondaryReplayStageLifecycleProbe(void *context)
{
    // Keep all four native AppendReplayWriteSegment callers reachable in the
    // same LTCG image so the compiler can recover its EBX/EDI private ABI.
    InitializeSecondaryReplay();
    RecordSecondaryReplayFrame();
    WritePauseMenuReplay();
    reinterpret_cast<SecondaryReplayStageLifecycle *>(context)->ApplySecondaryReplayStageState();
}
