#include "SecondaryDamage.hpp"
#include "SoundQueue.hpp"

struct SecondaryPositionalSoundRecord
{
    short unknown000, value;
    unsigned char unknown004[4];
};
extern const SecondaryPositionalSoundRecord g_SecondaryPositionalSoundRecords[];
extern int g_SecondaryPositionalSoundValues[];

void __stdcall QueueSecondaryActorSound(int sound, float x)
{
    int pan = static_cast<int>(x * 1000.0 / 192.0);
    int value = g_SecondaryPositionalSoundRecords[sound].value;
    SoundEffectQueue *queue = &g_SoundManagerQueue.effects;
    int i;
    for (i = 0; i < 12; ++i)
    {
        int id = queue->ids[i];
        if (id < 0) break;
        if (id == sound)
        {
            int count = queue->counts[i];
            if (count < 128)
            {
                queue->pans[i][count] = pan;
                ++queue->counts[i];
            }
            return;
        }
    }
    if (i < 12)
    {
        queue->ids[i] = sound;
        g_SecondaryPositionalSoundValues[sound] = value;
        queue->pans[i][0] = pan;
        ++queue->counts[i];
    }
}
