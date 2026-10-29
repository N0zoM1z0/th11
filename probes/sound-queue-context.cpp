// Independent manager/controller/command inputs; diagnostic image is not runnable.
#include "SoundQueue.hpp"
extern "C" int __stdcall SoundQueueProbe(int kind, SoundManagerQueueView *manager,
    MusicControllerView *music, int opcode, int value, const char *text, void *context)
{
    switch (kind)
    {
    case 0: return ProcessSoundQueue();
    case 1: return MusicStreamingThread(context);
    case 2: EnqueueSoundCommand(manager, opcode, value, text); break;
    case 3: return reinterpret_cast<int>(FirstMusicBuffer(music));
    case 4: SetMusicBufferVolume(music, value); break;
    case 5:
    {
        DWORD id;
        return reinterpret_cast<int>(CreateThread(0, 0, MusicStreamingThread, context, 0, &id));
    }
    }
    return 0;
}
