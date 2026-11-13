// The native match finder has a private ECX/EDX/stack ABI. Its first
// fastcall argument is intentionally not consumed; EDX carries the ring
// position and one stack DWORD is the output-position pointer.
struct ReplayCompressionNode { int parent, left, right; };
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;
extern "C" void __cdecl ReplayCompressionReplaceNodePrivate();

// 00442910..00442AAF, complete 416-byte / 113-instruction native owner.
__declspec(naked) int __fastcall FindReplayCompressionMatch(
    int *unused, int position, int *matchPosition)
{
    __asm {
        test edx, edx
        jne loc_00442919
        xor eax, eax
        ret 4
    loc_00442919:
        mov eax, dword ptr [g_ReplayCompressionRoot]
        push ebx
        push ebp
        push esi
        xor ebx, ebx
        push edi
    loc_00442924:
        mov edi, eax
        xor esi, esi
        sub edi, edx
        _emit 08Dh
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
    loc_00442930:
        lea ecx, [esi + edx]
        lea ebp, [edi + ecx]
        and ebp, 01FFFh
        movzx ebp, byte ptr [ebp + g_ReplayCompressionWindow]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A41
        lea ecx, [esi + eax + 1]
        and ecx, 01FFFh
        movzx ebp, byte ptr [ecx + g_ReplayCompressionWindow]
        lea ecx, [esi + edx + 1]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A2C
        lea ecx, [esi + eax + 2]
        and ecx, 01FFFh
        movzx ebp, byte ptr [ecx + g_ReplayCompressionWindow]
        lea ecx, [esi + edx + 2]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A2F
        lea ecx, [esi + eax + 3]
        and ecx, 01FFFh
        movzx ebp, byte ptr [ecx + g_ReplayCompressionWindow]
        lea ecx, [esi + edx + 3]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A34
        lea ecx, [esi + eax + 4]
        and ecx, 01FFFh
        movzx ebp, byte ptr [ecx + g_ReplayCompressionWindow]
        lea ecx, [esi + edx + 4]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A39
        lea ecx, [esi + eax + 5]
        and ecx, 01FFFh
        movzx ebp, byte ptr [ecx + g_ReplayCompressionWindow]
        lea ecx, [esi + edx + 5]
        and ecx, 01FFFh
        movzx ecx, byte ptr [ecx + g_ReplayCompressionWindow]
        sub ecx, ebp
        jne loc_00442A3E
        add esi, 6
        cmp esi, 012h
        jl loc_00442930
        jmp loc_00442A41
    loc_00442A2C:
        inc esi
        jmp loc_00442A41
    loc_00442A2F:
        add esi, 2
        jmp loc_00442A41
    loc_00442A34:
        add esi, 3
        jmp loc_00442A41
    loc_00442A39:
        add esi, 4
        jmp loc_00442A41
    loc_00442A3E:
        add esi, 5
    loc_00442A41:
        cmp esi, ebx
        jl loc_00442A52
        cmp esi, 012h
        mov edi, dword ptr [esp + 014h]
        mov ebx, esi
        mov dword ptr [edi], eax
        jge loc_00442A76
    loc_00442A52:
        test ecx, ecx
        lea ecx, [eax + eax*2]
        jl loc_00442A62
        lea ecx, [ecx*4 + g_ReplayCompressionNodes + 8]
        jmp loc_00442A69
    loc_00442A62:
        lea ecx, [ecx*4 + g_ReplayCompressionNodes + 4]
    loc_00442A69:
        mov esi, dword ptr [ecx]
        test esi, esi
        je loc_00442A84
        mov eax, esi
        jmp loc_00442924
    loc_00442A76:
        call ReplayCompressionReplaceNodePrivate
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        ret 4
    loc_00442A84:
        mov dword ptr [ecx], edx
        lea ecx, [edx + edx*2]
        pop edi
        add ecx, ecx
        add ecx, ecx
        pop esi
        mov dword ptr [ecx + g_ReplayCompressionNodes], eax
        pop ebp
        mov eax, ebx
        mov dword ptr [ecx + g_ReplayCompressionNodes + 8], 0
        mov dword ptr [ecx + g_ReplayCompressionNodes + 4], 0
        pop ebx
        ret 4
    }
}
