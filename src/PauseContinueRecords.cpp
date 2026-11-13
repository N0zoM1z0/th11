#include "PauseContinueMenu.hpp"
#include "MenuController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"

int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock);

__declspec(naked) void __stdcall PreparePauseContinueRecords()
{
    __asm
    {
        push edi
        mov edi, 7
        cmp dword ptr [g_StageVariant], edi
        jnz stage_selected
        cmp dword ptr [esi + 1F4h], 0
        jz stage_selected
        mov eax, 8
        mov dword ptr [g_SecondaryGameplaySelectionEC8], offset g_MenuStageSelections + 40h
        mov dword ptr [g_StageVariant], eax
        mov dword ptr [g_MenuPreviousVariant], eax

    stage_selected:
        mov eax, dword ptr [g_MeshEffectSelection0]
        mov ecx, dword ptr [g_MeshEffectSelection1]
        lea edx, [ecx + eax * 2]
        add edx, eax
        mov eax, dword ptr [g_SecondaryGameplayStatistics]
        imul edx, edx, 68D4h
        lea eax, [edx + eax + 8]
        push eax
        call InsertPauseContinueScore

        cmp dword ptr [g_StageVariant], edi
        jnz stage_restored
        cmp dword ptr [esi + 1F4h], 0
        jz stage_restored
        mov dword ptr [g_SecondaryGameplaySelectionEC8], offset g_MenuStageSelections
        mov dword ptr [g_StageVariant], edi
        mov dword ptr [g_MenuPreviousVariant], edi

    stage_restored:
        test eax, eax
        jl no_record
        mov edx, 1
        mov dword ptr [esi + 40h], 19h
        mov dword ptr [esi + 108h], edx
        mov ecx, dword ptr [esi + 40h]
        test ecx, ecx
        jz selection_ready
        cmp eax, ecx
        jl selection_ready
        lea eax, [ecx - 1]

    selection_ready:
        push ebp
        lea ebp, [esi + 110h]
        mov dword ptr [esi + 38h], eax
        mov eax, dword ptr [ebp + 8]
        test eax, eax
        jz keyboard_empty
        jg keyboard_positive
        dec eax
        mov dword ptr [ebp], eax
        jmp keyboard_ready

    keyboard_positive:
        xor eax, eax
        mov dword ptr [ebp], eax
        jmp keyboard_ready

    keyboard_empty:
        mov dword ptr [ebp], 0

    keyboard_ready:
        mov ecx, dword ptr [g_SecondaryGameplayStatistics]
        lea edi, [esi + 2CCh]
        add ecx, 2DDE0h
        mov eax, edi
        mov dword ptr [esi + 118h], 5Bh
        mov dword ptr [esi + 1E0h], edx
        sub eax, ecx

    copy_remembered_name:
        mov dl, byte ptr [ecx]
        mov byte ptr [eax + ecx], dl
        inc ecx
        test dl, dl
        jnz copy_remembered_name

        mov dword ptr [esi + 1F0h], 0
        mov edx, offset g_PauseRecordEmptyName
        mov ecx, edi
        _emit 8Dh
        _emit 64h
        _emit 24h
        _emit 00h

    compare_empty_name:
        mov al, byte ptr [ecx]
        cmp al, byte ptr [edx]
        jnz names_differ
        test al, al
        jz names_equal
        mov al, byte ptr [ecx + 1]
        cmp al, byte ptr [edx + 1]
        jnz names_differ
        add ecx, 2
        add edx, 2
        test al, al
        jnz compare_empty_name

    names_equal:
        xor eax, eax
        jmp name_comparison_done

    names_differ:
        sbb eax, eax
        sbb eax, -1

    name_comparison_done:
        test eax, eax
        jz no_keyboard_move
        push -1
        mov eax, ebp
        call MoveSecondaryDebugSelection

    no_keyboard_move:
        mov eax, 8
        mov cl, 20h
        pop ebp
        _emit 8Dh
        _emit 9Bh
        _emit 00h
        _emit 00h
        _emit 00h
        _emit 00h

    trim_name:
        cmp byte ptr [esi + eax + 2CBh], cl
        jnz name_trimmed
        dec eax
        test eax, eax
        jg trim_name

    name_trimmed:
        mov dword ptr [esi + 1F0h], eax
        mov dword ptr [esi + 1F8h], 0
        pop edi
        ret

    no_record:
        mov dword ptr [esi + 1F8h], 1
        pop edi
        ret
    }
}
