#include "SecondaryReplayState.hpp"
#include "SecondaryReplayStageLifecycle.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "SecondaryDamage.hpp"

struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
void __stdcall SetSecondaryIntegerPlayerPosition(const int *position);
extern unsigned int g_ReplayModeFlags;
extern unsigned int g_ReplayStageBombs, g_ReplayStageFlags;
extern unsigned int g_ReplayStageBonus, g_ReplayStageExtraScore;

// Native 0x436DA0..0x436F23 is a private register-ABI stage owner. Keep the
// native register lifetimes and exact branch layout: ESI replay root, EBP zero
// / selected stage header, EBX secondary controller, EDI selected stage.
__declspec(naked) void __stdcall UpdateSecondaryReplayStage()
{
    __asm {
        push ecx
        push ebx
        push ebp
        push esi
        mov esi, dword ptr [g_SecondaryReplayCurrent]
        mov eax, dword ptr [esi+8]
        xor ebp, ebp
        push edi
        mov ecx, 2
        cmp eax, ebp
        jz update_chain_done
        or dword ptr [eax+4], ecx
    update_chain_done:
        mov eax, dword ptr [esi+1D0h]
        cmp eax, ebp
        jz auxiliary_chain_done
        or dword ptr [eax+4], ecx
    auxiliary_chain_done:
        mov eax, dword ptr [esi+0Ch]
        cmp eax, ebp
        jz draw_chain_done
        or dword ptr [eax+4], ecx
    draw_chain_done:
        mov eax, dword ptr [esi+10h]
        cmp eax, ebp
        jnz playback_stage

        mov eax, dword ptr [g_StageVariant]
        mov ebp, dword ptr [esi+eax*4+1Ch]
        mov ecx, esi
        call ClearReplayStageSegments
        mov edi, dword ptr [g_StageVariant]
        mov ebx, esi
        call AppendReplayWriteSegment
        mov dword ptr [esi+9Ch], eax
        cmp dword ptr [g_ReplayModeFlags], 0
        jnz capture_controller_state

        mov eax, dword ptr [g_SecondaryActorScore]
        mov dword ptr [ebp+0Ch], eax
        mov cx, word ptr [g_SecondaryPatternPower]
        mov word ptr [ebp+10h], cx
        mov ecx, dword ptr [g_SecondaryPlayerValue]
        mov eax, 51EB851Fh
        imul ecx
        sar edx, 5
        mov ecx, edx
        shr ecx, 1Fh
        add ecx, edx
        mov eax, ecx
        cdq
        mov edi, 0Ah
        idiv edi
        sub ecx, edx
        mov dword ptr [ebp+14h], ecx
        mov dx, word ptr [g_SecondaryPlayerLives]
        mov word ptr [ebp+18h], dx
        mov ax, word ptr [g_ReplayStageBombs]
        mov word ptr [ebp+1Ah], ax
        mov ecx, dword ptr [g_ReplayStageFlags]
        mov dword ptr [ebp+1Ch], ecx
        mov edx, dword ptr [g_ReplayStageBonus]
        mov dword ptr [ebp+28h], edx
        mov eax, dword ptr [g_SecondaryCollisionCount]
        mov dword ptr [ebp+34h], eax

    capture_controller_state:
        mov eax, dword ptr [g_SecondaryControllerMode]
        mov ecx, dword ptr [eax+888h]
        mov dword ptr [ebp+20h], ecx
        mov edx, dword ptr [eax+88Ch]
        mov dword ptr [ebp+24h], edx
        mov ecx, dword ptr [g_ReplayStageExtraScore]
        mov dword ptr [ebp+2Ch], ecx
        mov edx, dword ptr [eax+8BACh]
        mov dword ptr [ebp+38h], edx
        mov ecx, dword ptr [g_StageVariant]
        mov dword ptr [esi+1D4h], ecx
        mov edx, ecx
        mov ecx, dword ptr [esi+edx*4+1Ch]
        mov edx, dword ptr [eax+8D20h]
        mov dword ptr [ecx+30h], edx
        mov dword ptr [esi+1CCh], 0
        pop edi
        pop esi
        pop ebp
        pop ebx
        pop ecx
        ret

    playback_stage:
        cmp eax, 1
        jnz replay_stage_done
        mov eax, dword ptr [g_StageVariant]
        lea ecx, [eax+eax*8+2Dh]
        mov edi, dword ptr [esi+ecx*4]
        lea ecx, [edi+20h]
        mov dword ptr [esi+1D4h], eax
        call SetSecondaryIntegerPlayerPosition
        mov edx, dword ptr [edi+30h]
        mov ebx, dword ptr [g_SecondaryControllerMode]
        mov dword ptr [ebx+8D20h], edx
        mov eax, dword ptr [edi+38h]
        mov dword ptr [ebx+8BACh], eax
        cmp dword ptr [edi+38h], ebp
        jz replay_stage_done
        mov dword ptr [ebx+7C90h], ebp
        call ResetSecondaryPlayerEmitters
        mov ebx, dword ptr [g_SecondaryControllerMode]
        call ResetSecondaryPlayerEmitters

    replay_stage_done:
        pop edi
        mov dword ptr [esi+1CCh], ebp
        pop esi
        pop ebp
        pop ebx
        pop ecx
        ret
    }
}
