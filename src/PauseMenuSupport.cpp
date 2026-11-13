#include "PauseMenu.hpp"
#include "SoundQueue.hpp"
#include "VmBindings.hpp"

extern "C" void __cdecl DestroyTitleRecordStorage(TitleMenuRecordView *record);
#pragma comment(linker, "/alternatename:_DestroyTitleRecordStorage=??1TitleMenuRecordView@@QAE@XZ")

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

__declspec(naked) void __stdcall DeletePauseMenuReplay()
{
    __asm
    {
        test esi, esi
        jz replay_delete_done
        push esi
        call DestroyTitleRecordStorage
        push esi
        call FreeHeapBlock
        pop ecx
    replay_delete_done:
        ret
    }
}


extern "C" __declspec(naked) void __cdecl FreeHeapBlock(void *record)
{
    __asm
    {
        mov edi, edi
        push ebp
        mov ebp, esp
        pop ebp
        jmp free
    }
}
