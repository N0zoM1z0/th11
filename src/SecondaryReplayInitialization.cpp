#include "ChainRegistration.hpp"
#include "SecondaryDialogue.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "Random.hpp"
#include "SecondaryReplayState.hpp"
#include "ReplayHeaderStorageInitialization.hpp"

// Native private ABI: EAX=mode, ECX=replay source, [ESP+4]=state. The
// chain helpers consume ESI/EBX, cleanup consumes ECX/EAX, and segment append
// consumes EBX/EDI. This complete owner keeps those register contracts intact.
extern unsigned int g_ReplayModeFlags, g_ReplayStatusMask;
extern unsigned int g_ReplayStageCharacter, g_ReplayStageFlags, g_ReplayStageBonus;
extern unsigned int g_ReplayStageBombs, g_ReplayStageExtraScore;
extern unsigned int g_StageVariant;
extern int g_SecondaryActorScore, g_SecondaryPatternPower, g_SecondaryPlayerValue;
extern int g_SecondaryCollisionCount;
extern RandomStateView g_TextRandomState;
extern "C" void *__cdecl ReplayInitAllocate(unsigned int size);
extern "C" void *__cdecl ReplayInitMemset(void *destination, int value, unsigned int size);
#pragma comment(linker, "/alternatename:_ReplayInitAllocate=??2@YAPAXI@Z")
#pragma comment(linker, "/alternatename:_ReplayInitMemset=_memset")
struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source);
int __fastcall ReplayRecordUpdate(void *context);
int __fastcall ReplayPlaybackUpdate(void *context);
int __fastcall ReplayAuxiliaryUpdate(void *context);
int __fastcall ReplayDrawCallback(void *context);

// Native 004356E0..00435C43: complete 1380-byte three-mode initializer.
__declspec(naked) int __stdcall InitializeSecondaryReplay(ReplayInitStateView *state)
{
    __asm {
        push ebx
        push ebp
        mov ebp, dword ptr [esp+0Ch]
        push esi
        xor esi, esi
        push edi
        mov dword ptr [ebp+10h], eax
        cmp eax, esi
        jne loc_004359fc
        mov eax, dword ptr [g_StageVariant]
        mov ecx, ebp
        mov dword ptr [g_SecondaryReplayCurrent], ebp
        call ClearReplayStageSegments
        mov edi, dword ptr [g_StageVariant]
        mov ebx, ebp
        call AppendReplayWriteSegment
        push 24h
        mov dword ptr [ebp+9Ch], eax
        call ReplayInitAllocate
        add esp, 4
        cmp eax, esi
        je loc_00435757
        xor ecx, ecx
        mov dword ptr [eax+4], ecx
        mov dword ptr [eax+8], ecx
        mov dword ptr [eax+0Ch], ecx
        mov dword ptr [eax+14h], ecx
        mov dword ptr [eax+18h], ecx
        mov dword ptr [eax+1Ch], ecx
        mov dword ptr [eax+20h], ecx
        mov ecx, 4
        mov dword ptr [eax], 72313174h
        mov word ptr [eax+4], cx
        mov dword ptr [eax+10h], 100h
        jmp loc_00435759
    loc_00435757:
        xor eax, eax
    loc_00435759:
        push 70h
        mov dword ptr [ebp+14h], eax
        call ReplayInitAllocate
        mov edi, eax
        add esp, 4
        cmp edi, esi
        je loc_00435785
        lea esi, [edi+18h]
        call InitializeReplayHeaderStorage
        push 70h
        push 0
        push edi
        call ReplayInitMemset
        add esp, 0Ch
        xor esi, esi
        jmp loc_00435787
    loc_00435785:
        xor edi, edi
    loc_00435787:
        push 90h
        mov dword ptr [ebp+18h], edi
        call ReplayInitAllocate
        mov edi, eax
        add esp, 4
        cmp edi, esi
        je loc_004357ae
        push 90h
        push esi
        push edi
        call ReplayInitMemset
        add esp, 0Ch
        jmp loc_004357b0
    loc_004357ae:
        xor edi, edi
    loc_004357b0:
        mov edx, dword ptr [g_StageVariant]
        mov dword ptr [ebp+edx*4+1Ch], edi
        mov eax, dword ptr [g_StageVariant]
        mov ebx, dword ptr [ebp+eax*4+1Ch]
        mov ecx, dword ptr [ebp+18h]
        mov edx, dword ptr [g_MeshEffectSelection0]
        mov dword ptr [ecx+5Ch], edx
        mov eax, dword ptr [ebp+18h]
        mov ecx, dword ptr [g_MeshEffectSelection1]
        mov dword ptr [eax+60h], ecx
        mov eax, dword ptr [g_ReplayStageCharacter]
        mov edx, dword ptr [ebp+18h]
        mov dword ptr [edx+64h], eax
        mov ecx, dword ptr [g_ReplayStatusMask]
        mov eax, dword ptr [ebp+18h]
        shr ecx, 4
        xor cl, byte ptr [eax+0Ah]
        and cl, 1
        xor byte ptr [eax+0Ah], cl
        mov eax, dword ptr [g_SecondaryDialogueSession]
        cmp eax, esi
        je loc_00435816
        mov edi, dword ptr [ebp+18h]
        add edi, 18h
        lea esi, [eax+24h]
        mov ecx, 0Fh
        rep movsd
        xor esi, esi
    loc_00435816:
        mov dx, word ptr [g_StageVariant]
        mov word ptr [ebx], dx
        mov ax, word ptr [g_TextRandomState]
        mov word ptr [ebx+2], ax
        mov dword ptr [g_TextRandomState+4], esi
        mov ecx, dword ptr [ebx+8Ch]
        xor ecx, dword ptr [g_ReplayModeFlags]
        and ecx, 1
        xor dword ptr [ebx+8Ch], ecx
        cmp dword ptr [g_ReplayModeFlags], esi
        je loc_00435853
        mov dword ptr [ebx+20h], esi
        mov dword ptr [ebx+24h], esi
    loc_00435853:
        mov edx, dword ptr [g_SecondaryActorScore]
        mov dword ptr [ebx+0Ch], edx
        mov ax, word ptr [g_SecondaryPatternPower]
        mov word ptr [ebx+10h], ax
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
        mov dword ptr [ebx+14h], ecx
        mov cx, word ptr [g_SecondaryPlayerLives]
        mov word ptr [ebx+18h], cx
        mov dx, word ptr [g_ReplayStageBombs]
        mov word ptr [ebx+1Ah], dx
        mov eax, dword ptr [g_ReplayStageFlags]
        mov dword ptr [ebx+1Ch], eax
        mov ecx, dword ptr [g_ReplayStageBonus]
        mov dword ptr [ebx+28h], ecx
        mov edx, dword ptr [g_SecondaryCollisionCount]
        mov dword ptr [ebx+34h], edx
        xor eax, eax
        add ebx, 3Ch
        mov ecx, 14h
    loc_004358c6:
        mov dword ptr [ebx], eax
        add ebx, 4
        sub eax, 21522153h
        sub ecx, 1
        jne loc_004358c6
        mov eax, dword ptr [ebp+18h]
        mov ecx, dword ptr [g_ReplayStageBonus]
        push 24h
        mov dword ptr [eax+6Ch], ecx
        call ReplayInitAllocate
        add esp, 4
        cmp eax, esi
        je loc_0043590b
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], esi
        mov dword ptr [eax+0Ch], esi
        mov dword ptr [eax+10h], esi
        mov dword ptr [eax], esi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], esi
        mov dword ptr [eax+1Ch], esi
        mov edi, eax
        jmp loc_0043590d
    loc_0043590b:
        xor edi, edi
    loc_0043590d:
        mov edx, dword ptr [edi+4]
        and edx, 0FFFFFFFDh
        mov dword ptr [edi+0Ch], esi
        mov dword ptr [edi+10h], esi
        or edx, 1
        mov ebx, 0Bh
        mov esi, edi
        mov dword ptr [edi+8], offset ReplayRecordUpdate
        mov dword ptr [edi+20h], ebp
        mov dword ptr [edi+4], edx
        call AddToUpdateChain
        push 24h
        mov dword ptr [ebp+8], edi
        call ReplayInitAllocate
        xor edi, edi
        add esp, 4
        cmp eax, edi
        je loc_00435964
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], edi
        mov dword ptr [eax+0Ch], edi
        mov dword ptr [eax+10h], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], edi
        mov dword ptr [eax+1Ch], edi
        mov esi, eax
        jmp loc_00435966
    loc_00435964:
        xor esi, esi
    loc_00435966:
        mov eax, dword ptr [esi+4]
        and eax, 0FFFFFFFDh
        or eax, 1
        mov ebx, 1Bh
        mov dword ptr [esi+8], offset ReplayAuxiliaryUpdate
        mov dword ptr [esi+0Ch], edi
        mov dword ptr [esi+10h], edi
        mov dword ptr [esi+20h], ebp
        mov dword ptr [esi+4], eax
        call AddToUpdateChain
        push 24h
        mov dword ptr [ebp+1D0h], esi
        call ReplayInitAllocate
        add esp, 4
        cmp eax, edi
        je loc_004359bc
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], edi
        mov dword ptr [eax+0Ch], edi
        mov dword ptr [eax+10h], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], edi
        mov dword ptr [eax+1Ch], edi
        mov esi, eax
        jmp loc_004359be
    loc_004359bc:
        xor esi, esi
    loc_004359be:
        mov ecx, dword ptr [esi+4]
        and ecx, 0FFFFFFFDh
        or ecx, 1
        mov ebx, 38h
        mov dword ptr [esi+8], offset ReplayDrawCallback
        mov dword ptr [esi+0Ch], edi
        mov dword ptr [esi+10h], edi
        mov dword ptr [esi+20h], ebp
        mov dword ptr [esi+4], ecx
        call AddToDrawChain
        mov dword ptr [ebp+0Ch], esi
        mov edx, dword ptr [g_StageVariant]
        pop edi
        pop esi
        mov dword ptr [ebp+1D4h], edx
        pop ebp
        xor eax, eax
        pop ebx
        ret 4
    loc_004359fc:
        cmp eax, 1
        jne loc_00435c27
        push ecx
        push ebp
        mov dword ptr [g_SecondaryReplayCurrent], ebp
        call LoadReplayStageData
        test eax, eax
        je loc_00435a20
    loc_00435a16:
        pop edi
        pop esi
        pop ebp
        or eax, 0FFFFFFFFh
        pop ebx
        ret 4
    loc_00435a20:
        mov esi, dword ptr [ebp+18h]
        mov edi, dword ptr [g_SecondaryDialogueSession]
        add edi, 24h
        add esi, 18h
        mov ecx, 0Fh
        rep movsd
        mov eax, dword ptr [g_StageVariant]
        lea eax, [eax+eax*8]
        mov edx, dword ptr [ebp+eax*4+0A4h]
        mov dword ptr [ebp+eax*4+0A8h], edx
        mov edx, dword ptr [ebp+eax*4+0ACh]
        mov dword ptr [ebp+eax*4+0B0h], edx
        lea ecx, [ebp+eax*4+0A4h]
        mov eax, dword ptr [ecx+10h]
        xor edi, edi
        mov dword ptr [ecx+14h], edi
        mov ecx, dword ptr [ebp+18h]
        mov edx, dword ptr [ecx+5Ch]
        mov dword ptr [g_MeshEffectSelection0], edx
        mov ecx, dword ptr [ebp+18h]
        mov edx, dword ptr [ecx+60h]
        mov dword ptr [g_MeshEffectSelection1], edx
        mov ecx, dword ptr [ebp+18h]
        mov edx, dword ptr [ecx+64h]
        mov dword ptr [g_ReplayStageCharacter], edx
        mov cx, word ptr [eax+2]
        mov word ptr [g_TextRandomState], cx
        mov dword ptr [g_TextRandomState+4], edi
        mov edx, dword ptr [eax+0Ch]
        mov dword ptr [g_SecondaryActorScore], edx
        movsx ecx, word ptr [eax+10h]
        mov edx, dword ptr [g_SecondaryPatternMaximumPower]
        cmp ecx, edx
        mov dword ptr [g_SecondaryPatternPower], ecx
        jle loc_00435ac1
        mov dword ptr [g_SecondaryPatternPower], edx
    loc_00435ac1:
        mov ecx, dword ptr [eax+14h]
        imul ecx, ecx, 64h
        mov dword ptr [g_SecondaryPlayerValue], ecx
        movsx edx, word ptr [eax+18h]
        mov dword ptr [g_SecondaryPlayerLives], edx
        movsx ecx, word ptr [eax+1Ah]
        mov dword ptr [g_ReplayStageBombs], ecx
        mov edx, dword ptr [eax+1Ch]
        mov dword ptr [g_ReplayStageFlags], edx
        mov ecx, dword ptr [eax+28h]
        mov dword ptr [g_ReplayStageBonus], ecx
        mov ecx, dword ptr [g_StageVariant]
        lea edx, [ecx+ecx*8+2Dh]
        mov ecx, dword ptr [ebp+edx*4]
        mov edx, dword ptr [ecx+2Ch]
        mov dword ptr [g_ReplayStageExtraScore], edx
        mov eax, dword ptr [eax+34h]
        push 24h
        mov dword ptr [g_SecondaryCollisionCount], eax
        call ReplayInitAllocate
        add esp, 4
        cmp eax, edi
        je loc_00435b3c
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], edi
        mov dword ptr [eax+0Ch], edi
        mov dword ptr [eax+10h], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], edi
        mov dword ptr [eax+1Ch], edi
        mov esi, eax
        jmp loc_00435b3e
    loc_00435b3c:
        xor esi, esi
    loc_00435b3e:
        mov ecx, dword ptr [esi+4]
        and ecx, 0FFFFFFFDh
        or ecx, 1
        mov ebx, 0Bh
        mov dword ptr [esi+8], offset ReplayPlaybackUpdate
        mov dword ptr [esi+0Ch], edi
        mov dword ptr [esi+10h], edi
        mov dword ptr [esi+20h], ebp
        mov dword ptr [esi+4], ecx
        call AddToUpdateChain
        push 24h
        mov dword ptr [ebp+8], esi
        call ReplayInitAllocate
        add esp, 4
        cmp eax, edi
        je loc_00435b91
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], edi
        mov dword ptr [eax+0Ch], edi
        mov dword ptr [eax+10h], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], edi
        mov dword ptr [eax+1Ch], edi
        mov esi, eax
        jmp loc_00435b93
    loc_00435b91:
        xor esi, esi
    loc_00435b93:
        mov edx, dword ptr [esi+4]
        and edx, 0FFFFFFFDh
        or edx, 1
        mov ebx, 1Bh
        mov dword ptr [esi+8], offset ReplayAuxiliaryUpdate
        mov dword ptr [esi+0Ch], edi
        mov dword ptr [esi+10h], edi
        mov dword ptr [esi+20h], ebp
        mov dword ptr [esi+4], edx
        call AddToUpdateChain
        push 24h
        mov dword ptr [ebp+1D0h], esi
        call ReplayInitAllocate
        add esp, 4
        cmp eax, edi
        je loc_00435be9
        and dword ptr [eax+4], 0FFFFFFFEh
        mov dword ptr [eax+8], edi
        mov dword ptr [eax+0Ch], edi
        mov dword ptr [eax+10h], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax+14h], eax
        mov dword ptr [eax+18h], edi
        mov dword ptr [eax+1Ch], edi
        mov esi, eax
        jmp loc_00435beb
    loc_00435be9:
        xor esi, esi
    loc_00435beb:
        mov eax, dword ptr [esi+4]
        and eax, 0FFFFFFFDh
        or eax, 1
        mov ebx, 38h
        mov dword ptr [esi+8], offset ReplayDrawCallback
        mov dword ptr [esi+0Ch], edi
        mov dword ptr [esi+10h], edi
        mov dword ptr [esi+20h], ebp
        mov dword ptr [esi+4], eax
        call AddToDrawChain
        pop edi
        mov dword ptr [ebp+0Ch], esi
        pop esi
        mov dword ptr [ebp+1D4h], 0FFFFFFFFh
        pop ebp
        xor eax, eax
        pop ebx
        ret 4
    loc_00435c27:
        cmp eax, 2
        jne loc_00435c3b
        push ecx
        push ebp
        call LoadReplayStageData
        test eax, eax
        jne loc_00435a16
    loc_00435c3b:
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        ret 4
    }
}
