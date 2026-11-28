// Diagnostic only: calls the two game.cpp music helpers and the queue with
// generic arguments, so /GL keeps the queue's general private convention.
#include "SoundQueue.hpp"
void __cdecl StartSecondaryGameplayExtraAudio();
void __cdecl AdvanceSecondaryGameplayExtraAudio();
int __cdecl ResetSecondaryGameplayAudio();
int __stdcall StartSecondaryGameplayAudio(int value, int selection);

extern "C" int __stdcall SecondaryGameplayAudioProbe(int kind, SoundManagerQueueView *manager,
    int opcode, int value, const char *text)
{
    if (kind == 0) StartSecondaryGameplayExtraAudio();
    else if (kind == 1) AdvanceSecondaryGameplayExtraAudio();
    else if (kind == 2) return ResetSecondaryGameplayAudio();
    else if (kind == 3) return StartSecondaryGameplayAudio(opcode, value);
    else EnqueueSoundCommand(manager, opcode, value, text);
    return ProcessSoundQueue();
}
