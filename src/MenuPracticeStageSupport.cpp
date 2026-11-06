#include "MenuPracticeStage.hpp"
#include "AsciiLifecycle.hpp"
#include "SoundQueue.hpp"

void __stdcall StartMenuTransitionOverlay(float x, float y)
{
    CameraVector position = {x, y, 0.0f};
    AsciiManagerObject *self = g_AsciiManager;
    if (!self->unknown184b4)
        self->unknown184b4 = CreateDirectPositionAnimationVmId(
            reinterpret_cast<AnimationFileView *>(self->animation), &position, 0x10, 0x16).value;
}

int __stdcall EnqueueMenuMusicFade(float duration)
{
    float scale = g_AnimationTimeScale;
    if (scale != 0.0f && !(1.0f < scale)) duration /= scale;
    int value = static_cast<int>(duration);
    EnqueueSoundCommand(&g_SoundManagerQueue, 5, value, "");
    return 0;
}
