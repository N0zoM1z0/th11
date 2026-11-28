// Diagnostic only: calls the two game.cpp music helpers and the queue with
// generic arguments, so /GL keeps the queue's general private convention.
#include "SoundQueue.hpp"
void __cdecl StartSecondaryGameplayExtraAudio();
void __cdecl AdvanceSecondaryGameplayExtraAudio();

extern "C" int __stdcall SecondaryGameplayAudioProbe(int kind, SoundManagerQueueView *manager,
    int opcode, int value, const char *text)
{
    if (kind == 0) StartSecondaryGameplayExtraAudio();
    else if (kind == 1) AdvanceSecondaryGameplayExtraAudio();
    else EnqueueSoundCommand(manager, opcode, value, text);
    return ProcessSoundQueue();
}
