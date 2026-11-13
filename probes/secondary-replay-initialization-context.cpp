#include "../src/SecondaryReplayState.hpp"
extern "C" int __stdcall SecondaryReplayInitializationProbe(
    ReplayInitStateView *state, int mode, void *source)
{
    int result;
    __asm {
        mov eax, mode
        mov ecx, source
        push state
        call ReplayInitContext::InitializeSecondaryReplay
        mov result, eax
    }
    return result;
}
