#include "ReplayHeaderStorageInitialization.hpp"

extern "C" void *__cdecl ReplayHeaderStorageMemset(
    void *destination, int value, unsigned int size);
#pragma comment(linker, "/alternatename:_ReplayHeaderStorageMemset=_memset")

// Native 00420D80..00420E08: ESI is the embedded header view, not a stack
// parameter. Preserve the exact memset, defaults-copy and byte-field order.
__declspec(naked) void __stdcall InitializeReplayHeaderStorage()
{
    __asm {
        push ebx
        push 3Ch
        xor ebx, ebx
        push ebx
        push esi
        call ReplayHeaderStorageMemset
        or dword ptr [esi+38h], 100h
        mov eax, 258h
        mov word ptr [esi+16h], ax
        mov ecx, eax
        mov byte ptr [esi+1Ah], bl
        mov byte ptr [esi+1Dh], bl
        mov byte ptr [esi+1Eh], bl
        mov dword ptr [esi], 110003h
        mov word ptr [esi+18h], cx
        mov al, 1
        mov byte ptr [esi+1Bh], al
        mov byte ptr [esi+1Ch], al
        mov edx, dword ptr [g_ReplayHeaderStorageDefaults]
        mov dword ptr [esi+4], edx
        mov eax, dword ptr [g_ReplayHeaderStorageDefaults+4]
        mov dword ptr [esi+8], eax
        mov ecx, dword ptr [g_ReplayHeaderStorageDefaults+8]
        mov dword ptr [esi+0Ch], ecx
        mov edx, dword ptr [g_ReplayHeaderStorageDefaults+0Ch]
        mov dword ptr [esi+10h], edx
        mov ax, word ptr [g_ReplayHeaderStorageDefaults+10h]
        mov word ptr [esi+14h], ax
        mov al, 2
        mov byte ptr [esi+1Fh], al
        mov byte ptr [esi+23h], al
        mov eax, 80000000h
        add esp, 0Ch
        mov byte ptr [esi+22h], bl
        mov byte ptr [esi+20h], 64h
        mov byte ptr [esi+21h], 50h
        mov dword ptr [esi+28h], eax
        mov dword ptr [esi+2Ch], eax
        pop ebx
        ret
    }
}
