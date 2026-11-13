#include <stdlib.h>
#include <string.h>

// Native TH11 replay compressor state. The target uses fixed-image absolute
// data operands and private register ABIs at the tree/search call sites.
struct ReplayCompressionNode { int parent, left, right; };
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;

void __cdecl ResetReplayCompressionTree();
int __fastcall FindReplayCompressionMatch(int *unused, int position, int *matchPosition);
void __fastcall DeleteReplayCompressionNode(int position);
extern "C" void __cdecl ReplayCompressionReplaceNodePrivate();
extern "C" void *__cdecl ReplayCompressionMalloc(unsigned int size);
#pragma comment(linker, "/alternatename:_ReplayCompressionMalloc=_malloc")

// 004423A0..004426BA. Exact 795-byte native owner, including the x86
// stack/register ABI, 18-byte lookahead, bit writer and inline tree unlink.
__declspec(naked) unsigned char *__stdcall CompressReplayData(
    const void *data, int size, int *compressedSize)
{
    __asm {
        sub esp, 024h
        push ebx
        push esi
        mov esi, dword ptr [esp + 034h]
        lea eax, [esi + esi]
        push eax
        mov byte ptr [esp + 0Fh], 080h
        xor ebx, ebx
        call ReplayCompressionMalloc
        add esp, 4
        mov dword ptr [esp + 028h], eax
        test eax, eax
        jne loc_004423CC
        pop esi
        pop ebx
        add esp, 024h
        ret 0Ch
    loc_004423CC:
        mov ecx, dword ptr [esp + 038h]
        push ebp
        push edi
        mov edi, dword ptr [esp + 038h]
        mov ebp, eax
        mov dword ptr [ecx], ebx
        call ResetReplayCompressionTree
        xor eax, eax
        mov dword ptr [esp + 018h], 1
        xor edx, edx
        jmp loc_004423F0
        _emit 08Dh
        _emit 049h
        _emit 000h
    loc_004423F0:
        cmp edx, esi
        jge loc_0044240A
        movzx ecx, byte ptr [edi]
        inc edi
        inc edx
        cmp ecx, -1
        je loc_0044240A
        mov byte ptr [eax + 04C0EE9h], cl
        inc eax
        cmp eax, 012h
        jl loc_004423F0
    loc_0044240A:
        xor esi, esi
        xor edx, edx
        mov ecx, eax
        mov dword ptr [esp + 014h], edi
        mov dword ptr [esp + 020h], ecx
        _emit 0C7h
        _emit 005h
        _emit 0E0h
        _emit 00Eh
        _emit 04Ch
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 005h
        _emit 0E4h
        _emit 08Eh
        _emit 04Ah
        _emit 000h
        _emit 000h
        _emit 020h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 01Dh
        _emit 0ECh
        _emit 08Eh
        _emit 04Ah
        _emit 000h
        _emit 089h
        _emit 01Dh
        _emit 0E8h
        _emit 08Eh
        _emit 04Ah
        _emit 000h
        mov dword ptr [esp + 01Ch], esi
        mov dword ptr [esp + 028h], edx
        test eax, eax
        jle loc_00442677
        jmp loc_00442454
        _emit 08Dh
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
    loc_00442450:
        mov edx, dword ptr [esp + 028h]
    loc_00442454:
        cmp esi, ecx
        jle loc_0044245E
        mov dword ptr [esp + 01Ch], ecx
        mov esi, ecx
    loc_0044245E:
        cmp esi, 2
        mov al, byte ptr [esp + 013h]
        jg loc_004424B6
        movzx edx, al
        or ebx, edx
        shr al, 1
        mov esi, 1
        mov byte ptr [esp + 013h], al
        jne loc_00442484
        mov byte ptr [ebp], bl
        inc ebp
        xor ebx, ebx
        mov byte ptr [esp + 013h], 080h
    loc_00442484:
        mov edi, dword ptr [esp + 018h]
        mov eax, 080h
        _emit 08Dh
        _emit 049h
        _emit 000h
    loc_00442490:
        test byte ptr [edi + 04C0EE8h], al
        je loc_0044249F
        movzx ecx, byte ptr [esp + 013h]
        or ebx, ecx
    loc_0044249F:
        shr byte ptr [esp + 013h], 1
        jne loc_004424B0
        mov byte ptr [ebp], bl
        inc ebp
        xor ebx, ebx
        mov byte ptr [esp + 013h], 080h
    loc_004424B0:
        shr eax, 1
        jne loc_00442490
        jmp loc_00442524
    loc_004424B6:
        shr al, 1
        jne loc_004424C9
        mov byte ptr [ebp], bl
        mov byte ptr [esp + 013h], 080h
        mov al, byte ptr [esp + 013h]
        inc ebp
        xor ebx, ebx
    loc_004424C9:
        mov ecx, 01000h
        mov edi, edi
    loc_004424D0:
        _emit 085h
        _emit 0CAh
        je loc_004424D9
        movzx edi, al
        or ebx, edi
    loc_004424D9:
        shr al, 1
        jne loc_004424EC
        mov byte ptr [ebp], bl
        mov byte ptr [esp + 013h], 080h
        mov al, byte ptr [esp + 013h]
        inc ebp
        xor ebx, ebx
    loc_004424EC:
        shr ecx, 1
        jne loc_004424D0
        mov ecx, 8
        lea edx, [esi - 3]
    loc_004424F8:
        _emit 085h
        _emit 0D1h
        je loc_00442501
        movzx edi, al
        or ebx, edi
    loc_00442501:
        shr al, 1
        mov byte ptr [esp + 013h], al
        jne loc_00442518
        mov byte ptr [ebp], bl
        mov byte ptr [esp + 013h], 080h
        mov al, byte ptr [esp + 013h]
        inc ebp
        xor ebx, ebx
    loc_00442518:
        shr ecx, 1
        jne loc_004424F8
        test esi, esi
        jle loc_0044266B
    loc_00442524:
        mov eax, dword ptr [esp + 014h]
        sub eax, dword ptr [esp + 038h]
        mov dword ptr [esp + 02Ch], esi
        mov dword ptr [esp + 024h], eax
        jmp loc_00442540
        _emit 08Dh
        _emit 0A4h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 049h
        _emit 000h
    loc_00442540:
        mov edi, dword ptr [esp + 018h]
        add edi, 012h
        and edi, 01FFFh
        lea eax, [edi + edi*2]
        add eax, eax
        mov ecx, dword ptr [eax + eax + 04A8ED8h]
        add eax, eax
        test ecx, ecx
        je loc_00442606
        mov edx, dword ptr [eax + 04A8EE0h]
        test edx, edx
        jne loc_00442575
        mov edx, dword ptr [eax + 04A8EDCh]
        jmp loc_0044257F
    loc_00442575:
        mov esi, dword ptr [eax + 04A8EDCh]
        test esi, esi
        jne loc_004425C2
    loc_0044257F:
        lea esi, [edx + edx*2]
        mov dword ptr [esi*4 + 04A8ED8h], ecx
        mov ecx, dword ptr [eax + 04A8ED8h]
        lea ecx, [ecx + ecx*2]
        add ecx, ecx
        add ecx, ecx
        cmp dword ptr [ecx + 04A8EE0h], edi
        jne loc_004425B0
        mov dword ptr [ecx + 04A8EE0h], edx
        mov dword ptr [eax + 04A8ED8h], 0
        jmp loc_00442606
    loc_004425B0:
        mov dword ptr [ecx + 04A8EDCh], edx
        mov dword ptr [eax + 04A8ED8h], 0
        jmp loc_00442606
    loc_004425C2:
        lea eax, [esi + esi*2]
        cmp dword ptr [eax*4 + 04A8EE0h], 0
        lea eax, [eax*4 + 04A8EE0h]
        je loc_004425F6
        jmp loc_004425E0
        _emit 08Dh
        _emit 0A4h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        nop
    loc_004425E0:
        mov esi, dword ptr [eax]
        lea eax, [esi + esi*2]
        cmp dword ptr [eax*4 + 04A8EE0h], 0
        lea eax, [eax*4 + 04A8EE0h]
        jne loc_004425E0
    loc_004425F6:
        mov ecx, esi
        call DeleteReplayCompressionNode
        mov edx, esi
        mov eax, edi
        call ReplayCompressionReplaceNodePrivate
    loc_00442606:
        mov edx, dword ptr [esp + 024h]
        cmp edx, dword ptr [esp + 03Ch]
        mov esi, 1
        jge loc_0044262B
        mov ecx, dword ptr [esp + 014h]
        movzx eax, byte ptr [ecx]
        add dword ptr [esp + 024h], esi
        add ecx, esi
        mov dword ptr [esp + 014h], ecx
        cmp eax, -1
        jne loc_00442631
    loc_0044262B:
        sub dword ptr [esp + 020h], esi
        jmp loc_00442637
    loc_00442631:
        mov byte ptr [edi + 04C0EE8h], al
    loc_00442637:
        mov eax, dword ptr [esp + 018h]
        lea edx, [eax + 1]
        and edx, 01FFFh
        cmp dword ptr [esp + 020h], 0
        mov dword ptr [esp + 018h], edx
        je loc_0044265D
        lea ecx, [esp + 028h]
        push ecx
        call FindReplayCompressionMatch
        mov dword ptr [esp + 01Ch], eax
    loc_0044265D:
        sub dword ptr [esp + 02Ch], esi
        jne loc_00442540
        mov esi, dword ptr [esp + 01Ch]
    loc_0044266B:
        mov ecx, dword ptr [esp + 020h]
        test ecx, ecx
        jg loc_00442450
    loc_00442677:
        shr byte ptr [esp + 013h], 1
        jne loc_00442688
        mov byte ptr [ebp], bl
        inc ebp
        xor ebx, ebx
        mov byte ptr [esp + 013h], 080h
    loc_00442688:
        mov eax, 01000h
        _emit 08Dh
        _emit 049h
        _emit 000h
    loc_00442690:
        shr byte ptr [esp + 013h], 1
        jne loc_004426A1
        mov byte ptr [ebp], bl
        inc ebp
        xor ebx, ebx
        mov byte ptr [esp + 013h], 080h
    loc_004426A1:
        shr eax, 1
        jne loc_00442690
        mov eax, dword ptr [esp + 030h]
        mov edx, dword ptr [esp + 040h]
        sub ebp, eax
        pop edi
        mov dword ptr [edx], ebp
        pop ebp
        pop esi
        pop ebx
        add esp, 024h
        ret 0Ch
    }
}
