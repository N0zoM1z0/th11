#include "SecondaryReplayState.hpp"
#include "FileSystem.hpp"
#include "SharedFile.hpp"
#include "ResourceFile.hpp"
#include "ByteCipher.hpp"
#include "Archive.hpp"

extern unsigned int g_ReplayStatusMask;
extern const char g_ReplayPathFormat[] = "replay/%s";
extern "C" unsigned int __security_cookie;
extern "C" void __fastcall __security_check_cookie(unsigned int value);
extern "C" int __cdecl sprintf(char *destination, const char *format, ...);
extern "C" void *__cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void *memory);

// The replay loader is a 441-byte VC9 stack-cookie owner. Its calls deliberately
// retain their native mixed ABI: OpenSharedFileForRead receives ESI, the shared
// read helper receives EDI, resource loading receives EAX/EDX plus one stack
// word, the byte transform receives AL plus five stack words, and decompression
// receives outputSize in EAX plus three stack words.
unsigned char *__stdcall ReadSharedReplayBytes(unsigned int size);
int __cdecl CloseSharedReplayReader();

__declspec(naked) int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source)
{
    __asm {
        sub esp, 108h
        mov eax, dword ptr [__security_cookie]
        xor eax, esp
        mov dword ptr [esp + 104h], eax
        mov eax, dword ptr [esp + 110h]
        push ebp
        mov ebp, dword ptr [esp + 110h]
        mov edx, ebp
        push esi
        sub edx, eax
        push edi
        mov ecx, eax
        lea esi, [edx + 1DCh]
    loc_00436b91:
        mov dl, byte ptr [ecx]
        mov byte ptr [esi + ecx], dl
        inc ecx
        test dl, dl
        jne loc_00436b91
        test byte ptr [g_ReplayStatusMask], 20h
        jne loc_00436c0a
        push eax
        lea eax, [esp + 14h]
        push offset g_ReplayPathFormat
        push eax
        call sprintf
        add esp, 0Ch
        lea ecx, [esp + 10h]
        push ecx
        call FileExists
        test eax, eax
        je loc_00436bea
        lea esi, [esp + 10h]
        call OpenSharedFileForRead
        test eax, eax
        jne loc_00436bea
        lea edi, [eax + 24h]
        call ReadSharedReplayBytes
        mov dword ptr [ebp + 14h], eax
        cmp dword ptr [eax], 72313174h
        je loc_00436bf2
    loc_00436be5:
        call CloseSharedReplayReader
    loc_00436bea:
        or eax, 0FFFFFFFFh
        jmp loc_00436cff
    loc_00436bf2:
        cmp word ptr [eax + 4], 4
        jne loc_00436be5
        mov edi, dword ptr [eax + 1Ch]
        call ReadSharedReplayBytes
        mov edi, eax
        call CloseSharedReplayReader
        jmp loc_00436c1c
    loc_00436c0a:
        push 0
        lea edx, [esp + 10h]
        push edx
        call LoadResourceFile
        mov dword ptr [ebp + 14h], eax
        lea edi, [eax + 24h]
    loc_00436c1c:
        mov eax, dword ptr [ebp + 14h]
        mov eax, dword ptr [eax + 20h]
        push eax
        call malloc
        mov ecx, dword ptr [ebp + 14h]
        add esp, 4
        mov dword ptr [ebp + 1C4h], eax
        mov eax, dword ptr [ecx + 1Ch]
        push eax
        push 800h
        push 0E1h
        push eax
        push edi
        mov al, 0AAh
        call ScatterXorBlocks
        mov edx, dword ptr [ebp + 14h]
        mov eax, dword ptr [edx + 1Ch]
        push eax
        push 40h
        push 7Ah
        push eax
        push edi
        mov al, 3Dh
        call ScatterXorBlocks
        mov eax, dword ptr [ebp + 14h]
        mov ecx, dword ptr [ebp + 1C4h]
        mov edx, dword ptr [eax + 1Ch]
        mov eax, dword ptr [eax + 20h]
        push ecx
        push edx
        push edi
        call DecompressArchiveData
        mov eax, dword ptr [ebp + 1C4h]
        mov dword ptr [ebp + 18h], eax
        add eax, 70h
        xor esi, esi
        push ebx
    loc_00436c85:
        mov ecx, dword ptr [ebp + 18h]
        mov ecx, dword ptr [ecx + 58h]
        cmp ecx, 8
        jl loc_00436c95
        mov ecx, 6
    loc_00436c95:
        cmp esi, ecx
        jge loc_00436ce6
        movsx ecx, word ptr [eax]
        add ecx, 5
        lea edx, [ecx + ecx*8]
        mov dword ptr [ebp + edx*4], eax
        movsx ecx, word ptr [eax]
        lea ecx, [ecx + ecx*8]
        lea edx, [eax + 90h]
        mov dword ptr [ebp + ecx*4 + 0A4h], edx
        movsx ecx, word ptr [eax]
        lea edx, [ecx + ecx*8]
        mov ebx, dword ptr [ebp + edx*4 + 0A4h]
        lea ecx, [ebp + edx*4]
        mov edx, dword ptr [eax + 4]
        lea edx, [edx + edx*2]
        lea edx, [ebx + edx*2]
        mov dword ptr [ecx + 0ACh], edx
        mov ecx, dword ptr [eax + 8]
        lea eax, [eax + ecx + 90h]
        inc esi
        jmp loc_00436c85
    loc_00436ce6:
        test byte ptr [g_ReplayStatusMask], 20h
        pop ebx
        jne loc_00436cfd
        test edi, edi
        je loc_00436cfd
        push edi
        call free
        add esp, 4
    loc_00436cfd:
        xor eax, eax
    loc_00436cff:
        mov ecx, dword ptr [esp + 110h]
        pop edi
        pop esi
        pop ebp
        xor ecx, esp
        call __security_check_cookie
        add esp, 108h
        ret 8
    }
}
