#include "PauseMenu.hpp"
#include "SoundQueue.hpp"
#include "VmBindings.hpp"

void __stdcall ResumePauseMenu(PauseMenuView *self)
{
    SecondaryGameplayView *gameplay = reinterpret_cast<SecondaryGameplayView *>(g_SecondaryDialogueSession);
    gameplay->flags &= ~0x10u;
    if (g_StageVariant != 6 || gameplay->time.current >= 300)
        EnqueueSoundCommand(&g_SoundManagerQueue, 7, 0, "");
    g_AnimationTimeScale = self->savedTimeScale;
    g_SecondaryGameplayAudioState = self->savedAudioState;
}

void __stdcall SignalPauseMenuChild(const AnimationVmId &id)
{
    AnimationVmId localId(id.value);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, localId);
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 6;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 6;
    }
}

void __stdcall DeletePauseMenuReplay(MenuReplayView *replay)
{
    if (replay)
    {
        DestroyMenuReplayContents(replay);
        ::operator delete(replay);
    }
}
