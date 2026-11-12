#include "StageResource.hpp"
#include "MeshEffects.hpp"

struct MeshEffectActorView;
extern MeshEffectActorView *g_MeshEffectActor;

// Native 0x406860..0x4068BC, complete 93-byte stage timer transition.
// The original uses a signed integer value and stores the rounded float.
void __stdcall EndSecondarySpecialState(int value)
{
    unsigned char *owner = reinterpret_cast<unsigned char *>(g_MeshEffectActor);
    unsigned int flags = *reinterpret_cast<unsigned int *>(owner + 0x8bc0);
    if (!(flags & 1))
    {
        *reinterpret_cast<float *>(owner + 0x8bb8) = 0.0f;
        *reinterpret_cast<int *>(owner + 0x8bb4) = 0;
        *reinterpret_cast<int *>(owner + 0x8bb0) = -999999;
        *reinterpret_cast<float **>(owner + 0x8bbc) = &g_AnimationTimeScale;
        *reinterpret_cast<unsigned int *>(owner + 0x8bc0) = flags | 1;
    }
    *reinterpret_cast<int *>(owner + 0x8bb4) = value;
    *reinterpret_cast<int *>(owner + 0x8bb0) = value - 1;
    *reinterpret_cast<float *>(owner + 0x8bb8) = static_cast<float>(value);
}
// Physical queue layout already used by the real MeshEffects producer.
struct MeshEffectSoundQueueView
{
    int slots[12], pending[12], frames[12][128];
};
extern MeshEffectSoundQueueView g_MeshEffectSoundQueue;

// Native 0x44A300..0x44A339: mark the existing or first empty 0x31 sound
// event pending, without resetting other events or truncating the list.
void __cdecl QueueSecondarySpecialPulse()
{
    int index = 0;
    int *slot = g_MeshEffectSoundQueue.slots;
    while (*slot >= 0)
    {
        if (*slot == 0x31) goto Found;
        ++slot;
        ++index;
        if (index >= 12) return;
    }
    if (index >= 12) return;
    g_MeshEffectSoundQueue.slots[index] = 0x31;
Found:
    g_MeshEffectSoundQueue.pending[index] = -1;
}
