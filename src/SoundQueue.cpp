#include "SoundQueue.hpp"
#include "MainRuntime.hpp"
#include <string.h>

// Actual reviewed helper boundaries; their remaining bodies are source-open.
void __cdecl TraceSoundQueue(const char *format, ...);
void __stdcall ClosePreparedMusic(SoundManagerQueueView *self);
int __stdcall LoadPreparedMusic(SoundManagerQueueView *self, int index, const char *name);
int __stdcall StartPreparedMusic(SoundManagerQueueView *self, int index);
int __stdcall FindSoundTrack(const char *name, SoundManagerQueueView *self);
HRESULT __stdcall OpenMusicDecoder(MusicControllerView *self);
HRESULT __stdcall ConfigureMusicDecoder(MusicDecoderView *self, SoundTrackRecord *record, int offset);
HRESULT __stdcall RefillMusicBuffer(MusicControllerView *self, SoundBufferView *buffer, int loop);
HRESULT __stdcall ResetMusicPosition(MusicControllerView *self);
HRESULT __stdcall PlayMusicStream(MusicControllerView *self);
HRESULT __stdcall StopMusicStream(MusicControllerView *self, int closeFile);
HRESULT __stdcall PauseMusicStream(MusicControllerView *self);
HRESULT __stdcall ResumeMusicStream(MusicControllerView *self);
HRESULT __stdcall UpdateMusicStream(MusicControllerView *self);

namespace
{
__forceinline unsigned char RuntimeByte(unsigned int offset)
{
    return reinterpret_cast<const unsigned char *>(&g_MainRuntime)[offset];
}
__forceinline void EnterSoundSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[11]);
        ++g_MainRuntime.criticalDepth[11];
    }
}
__forceinline void LeaveSoundSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[11]);
        --g_MainRuntime.criticalDepth[11];
    }
}
}

int __cdecl ProcessSoundQueue()
{
    EnterSoundSection();
    if (!g_SoundManagerQueue.enabled)
    {
        LeaveSoundSection();
        return 0;
    }
    SoundCommand *command = &g_SoundManagerQueue.commands[0];
    for (;;)
    {
        int repeat = 0;
        switch (command->opcode)
        {
        case 1:
            if (RuntimeByte(0x200) & 0x10)
            {
                if (command->step) goto AdvanceCommand;
                ClosePreparedMusic(&g_SoundManagerQueue);
            }
            LoadPreparedMusic(&g_SoundManagerQueue, command->value, command->text);
            repeat = 1;
            goto RemoveCommand;
        case 2:
            if ((RuntimeByte(0x200) & 0x10) && command->value >= 0)
            {
                if (!command->step)
                {
                    if (StartPreparedMusic(&g_SoundManagerQueue, command->value))
                        goto RemoveCommand;
                }
                else if (command->step == 2)
                {
                    if (g_MusicController && FAILED(ResetMusicPosition(g_MusicController)))
                        goto RemoveCommand;
                }
                else if (command->step == 5)
                {
                    SoundBufferView *buffer = FirstMusicBuffer(g_MusicController);
                    command->value = g_MusicController->decoder->record->dataSize != 0;
                    if (FAILED(RefillMusicBuffer(g_MusicController, buffer, command->value)))
                        goto RemoveCommand;
                }
                else if (command->step == 7)
                    PlayMusicStream(g_MusicController);
                else if (command->step >= 20)
                    goto RemoveCommand;
                goto AdvanceCommand;
            }
            if (!g_MusicController) goto RemoveCommand;
            if (!command->step)
                StopMusicStream(g_MusicController, 0);
            else if (command->step == 1)
            {
                if (g_MusicController->busy) goto FinishedCommands;
                OpenMusicDecoder(g_MusicController);
            }
            else if (command->step == 2)
            {
                const char *name = command->value < 0 ? command->text :
                    g_SoundManagerQueue.filenames[command->value];
                int track = FindSoundTrack(name, &g_SoundManagerQueue);
                ConfigureMusicDecoder(g_MusicController->decoder,
                    g_SoundManagerQueue.records + track, 0);
            }
            else if (command->step == 3)
            {
                SoundBufferView *buffer = FirstMusicBuffer(g_MusicController);
                ResetMusicPosition(g_MusicController);
                command->value = g_MusicController->decoder->record->dataSize != 0;
                if (FAILED(RefillMusicBuffer(g_MusicController, buffer, command->value)))
                    goto RemoveCommand;
            }
            else if (command->step == 4)
                PlayMusicStream(g_MusicController);
            else if (command->step >= 7)
                goto RemoveCommand;
            goto AdvanceCommand;
        case 3:
            if (!g_MusicController) goto RemoveCommand;
            if (!command->step) StopMusicStream(g_MusicController, 1);
            else if (command->step == 1) goto RemoveCommand;
            goto AdvanceCommand;
        case 4:
            if (!g_MusicController) goto RemoveCommand;
            if (!command->step) StopMusicStream(g_MusicController, 1);
            else if (command->step == 1)
            {
                if (!g_SoundManagerQueue.thread) goto RemoveCommand;
                PostThreadMessageA(g_SoundManagerQueue.threadId, WM_QUIT, 0, 0);
            }
            else if (command->step == 2)
            {
                if (WaitForSingleObject(g_SoundManagerQueue.thread, 256))
                {
                    PostThreadMessageA(g_SoundManagerQueue.threadId, WM_QUIT, 0, 0);
                    --command->step;
                }
                else g_SoundManagerQueue.thread = 0;
            }
            else if (command->step == 3)
            {
                CloseHandle(g_SoundManagerQueue.thread);
                CloseHandle(g_MusicEvent);
                g_SoundManagerQueue.thread = 0;
                delete g_MusicController;
                g_MusicController = 0;
            }
            else if (command->step == 10) goto RemoveCommand;
            goto AdvanceCommand;
        case 5:
        {
            TraceSoundQueue(g_SoundFadeTrace, command->value);
            float frames = static_cast<float>(command->value);
            MusicControllerView *controller = g_MusicController;
            if (controller)
            {
                controller->fadeEnabled = 1;
                controller->fadeRemaining = controller->fadeInitial = static_cast<int>(frames * 60.0);
            }
            goto RemoveCommand;
        }
        case 6:
            if (RuntimeByte(0x1e3) == 1)
            {
                if (g_MusicController->busy) goto FinishedCommands;
                PauseMusicStream(g_MusicController);
            }
            goto RemoveCommand;
        case 7:
            if (RuntimeByte(0x1e3) == 1)
            {
                if (g_MusicController->busy) goto FinishedCommands;
                ResumeMusicStream(g_MusicController);
            }
            goto RemoveCommand;
        case 8:
            if (g_MusicController) SetMusicBufferVolume(g_MusicController, g_MusicVolume);
            goto RemoveCommand;
        default:
            goto FinishedCommands;
        }
RemoveCommand:
        // The native cursor advances along with the shift; repeat does not reset it.
        for (int moved = 0; moved < 31 && command->opcode; ++moved, ++command)
            *command = command[1];
        if (repeat) continue;
        break;
AdvanceCommand:
        ++command->step;
        break;
    }
FinishedCommands:
    if (RuntimeByte(0x1e4))
    {
        for (int i = 0; i < 12; ++i)
        {
            int sound = g_SoundManagerQueue.effects.ids[i];
            if (sound < 0) break;
            g_SoundManagerQueue.effects.ids[i] = -1;
            int count = g_SoundManagerQueue.effects.counts[i];
            if (count < 0)
            {
                if (g_SoundEffectBuffers[sound]) g_SoundEffectBuffers[sound]->Stop();
                g_SoundManagerQueue.effects.counts[i] = 0;
                continue;
            }
            int pan = 0;
            if (count > 0)
            {
                for (int sample = 0; sample < count; ++sample)
                    pan = static_cast<int>(static_cast<unsigned int>(pan) +
                        static_cast<unsigned int>(g_SoundManagerQueue.effects.pans[i][sample]));
                pan /= count;
            }
            g_SoundManagerQueue.effects.counts[i] = 0;
            if (!g_SoundEffectBuffers[sound]) continue;
            g_SoundEffectBuffers[sound]->Stop();
            g_SoundEffectBuffers[sound]->SetCurrentPosition(0);
            g_SoundEffectBuffers[sound]->SetPan(pan);
            if (g_SoundEffectVolume)
            {
                SoundBufferView *buffer = g_SoundEffectBuffers[sound];
                float level = static_cast<float>(g_SoundEffectVolume / 100.0);
                float remaining = static_cast<float>(1.0 - level);
                float cubic = static_cast<float>(static_cast<double>(remaining) * remaining * remaining);
                float gain = static_cast<float>(1.0 - cubic);
                int base = g_SoundVolumeRecords[sound].value + 5000;
                buffer->SetVolume(static_cast<int>(gain * base) - 5000);
            }
            else g_SoundEffectBuffers[sound]->SetVolume(-10000);
            g_SoundEffectBuffers[sound]->Play(0, 0, 0);
        }
    }
    LeaveSoundSection();
    return g_SoundManagerQueue.commands[0].opcode;
}

DWORD WINAPI MusicStreamingThread(void *)
{
    BOOL done = FALSE;
    MSG message;
    // These three shared-pointer reads are distinct in the native worker.
    MusicControllerView *volatile &shared =
        *reinterpret_cast<MusicControllerView *volatile *>(&g_MusicController);
    do
    {
        DWORD wake = MsgWaitForMultipleObjects(1, &g_MusicEvent, FALSE, INFINITE, 0x4bf);
        MusicControllerView *current = shared;
        if (!current) done = TRUE;
        switch (wake)
        {
        case WAIT_OBJECT_0:
            if (current && current->active)
            {
                current->busy = 1;
                UpdateMusicStream(shared);
                shared->busy = 0;
            }
            break;
        case WAIT_OBJECT_0 + 1:
            while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
                if (message.message == WM_QUIT) done = TRUE;
            break;
        }
    } while (!done);
    return 0;
}

void __stdcall EnqueueSoundCommand(SoundManagerQueueView *self, int opcode,
    int value, const char *text)
{
    EnterSoundSection();
    for (int i = 0; i < 31; ++i)
    {
        if (self->commands[i].opcode) continue;
        self->commands[i].opcode = opcode;
        self->commands[i].value = value;
        strcpy(self->commands[i].text, text);
        self->commands[i].step = 0;
        break;
    }
    LeaveSoundSection();
}

SoundBufferView *__stdcall FirstMusicBuffer(MusicControllerView *self)
{
    return self->buffers ? (self->bufferCount > 0 ? self->buffers[0] : 0) : 0;
}

void __stdcall SetMusicBufferVolume(MusicControllerView *self, int baseVolume)
{
    SoundBufferView **buffers = self->buffers;
    if (g_MusicVolume)
    {
        SoundBufferView *buffer = buffers[0];
        float level = static_cast<float>(g_MusicVolume / 100.0);
        float remaining = static_cast<float>(1.0 - level);
        float square = static_cast<float>(static_cast<double>(remaining) * remaining);
        float gain = static_cast<float>(1.0 - square);
        buffer->SetVolume(static_cast<int>(gain * (baseVolume + 5000)) - 5000);
    }
    else buffers[0]->SetVolume(-10000);
}
