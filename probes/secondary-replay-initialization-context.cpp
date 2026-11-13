#include "../src/SecondaryReplayState.hpp"
extern int __stdcall InitializeSecondaryReplay(ReplayInitStateView *state);
extern "C" int __stdcall SecondaryReplayInitializationProbe(
    ReplayInitStateView *state, int mode, void *source)
{
    int result;
    __asm {
        mov eax, mode
        mov ecx, source
        push state
        call InitializeSecondaryReplay
        mov result, eax
    }
    return result;
}
