#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"
#include <string.h>
#include <time.h>

// The score list stores ten 28-byte records for each difficulty.
// The initial 16 bytes of the character's score block precede record zero.
#pragma pack(push, 4)
struct PauseHighScoreRecord
{
    int score;
    unsigned char stage, shot;
    char name[9];
    unsigned char unknown00f;
    __time64_t timestamp;
    float completion;
};
#pragma pack(pop)
typedef char PauseHighScoreRecordStride[(sizeof(PauseHighScoreRecord) == 0x1c) ? 1 : -1];
typedef char PauseHighScoreRecordTime[(offsetof(PauseHighScoreRecord, timestamp) == 0x10) ? 1 : -1];
typedef char PauseHighScoreRecordCompletion[(offsetof(PauseHighScoreRecord, completion) == 0x18) ? 1 : -1];

// Unnamed live globals observed at 004A573C and 004A8D80 respectively.
extern unsigned char g_PauseRecordShotType;
#pragma pack(push, 4)
struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
#pragma pack(pop)
typedef char PauseRecordTimingElapsedOffset[(offsetof(PauseRecordTimingView, elapsed) == 0x24) ? 1 : -1];
typedef char PauseRecordTimingLimitOffset[(offsetof(PauseRecordTimingView, limit) == 0x2c) ? 1 : -1];
extern PauseRecordTimingView *g_PauseRecordTiming;

// 0042BA80: insert the current score in descending order, preserving records
// below the insertion point and leaving the list unchanged if none qualifies.
extern const char g_PauseRecordEmptyName[9] = "        ";
extern const double g_PauseRecordHundred = 100.0;

__declspec(naked) int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock)
{
    __asm
    {
        mov eax, dword ptr [g_SecondaryActorDifficulty]
        push ebx
        mov ecx, eax
        xor ebx, ebx
        imul ecx, ecx, 118h
        push ebp
        mov ebp, dword ptr [esp + 0Ch]
        push esi
        mov esi, dword ptr [g_SecondaryActorScore]
        lea ecx, [ecx + ebp + 10h]

    scan_score:
        cmp dword ptr [ecx], esi
        jle found_score
        inc ebx
        add ecx, 1Ch
        cmp ebx, 0Ah
        jl scan_score

    no_score:
        pop esi
        pop ebp
        or eax, 0FFFFFFFFh
        pop ebx
        ret 4

    found_score:
        cmp ebx, 0Ah
        jl insert_score
        pop esi
        pop ebp
        or eax, 0FFFFFFFFh
        pop ebx
        ret 4

    insert_score:
        mov edx, 9
        cmp ebx, edx
        jge score_fields
        push edi
        mov edi, edi

    shift_score:
        lea eax, [eax + eax * 4]
        lea eax, [edx + eax * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        lea eax, [ebp + ecx * 4]
        lea esi, [eax - 0Ch]
        lea edi, [eax + 10h]
        mov ecx, 7
        dec edx
        cmp edx, ebx
        rep movsd
        mov eax, dword ptr [g_SecondaryActorDifficulty]
        jg shift_score
        mov esi, dword ptr [g_SecondaryActorScore]
        pop edi

    score_fields:
        lea edx, [eax + eax * 4]
        lea eax, [ebx + edx * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        mov dword ptr [ebp + ecx * 4 + 10h], esi

        mov eax, dword ptr [g_SecondaryActorDifficulty]
        lea edx, [eax + eax * 4]
        lea eax, [ebx + edx * 2]
        movzx edx, byte ptr [g_PauseRecordShotType]
        lea ecx, [eax * 8]
        sub ecx, eax
        mov byte ptr [ebp + ecx * 4 + 15h], dl

        mov eax, dword ptr [g_SecondaryActorDifficulty]
        movzx edx, byte ptr [g_StageVariant]
        lea eax, [eax + eax * 4]
        lea eax, [ebx + eax * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        mov byte ptr [ebp + ecx * 4 + 14h], dl

        mov eax, dword ptr [g_SecondaryActorDifficulty]
        lea eax, [eax + eax * 4]
        lea eax, [ebx + eax * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        lea edx, [ebp + ecx * 4 + 20h]
        push edx
        call _time64

        mov eax, dword ptr [g_SecondaryActorDifficulty]
        mov edx, dword ptr [g_PauseRecordEmptyName]
        lea eax, [eax + eax * 4]
        lea eax, [ebx + eax * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        mov dword ptr [ebp + ecx * 4 + 16h], edx
        lea eax, [ebp + ecx * 4 + 16h]
        mov ecx, dword ptr [g_PauseRecordEmptyName + 4]
        mov dword ptr [eax + 4], ecx
        movzx edx, byte ptr [g_PauseRecordEmptyName + 8]
        mov byte ptr [eax + 8], dl

        mov eax, dword ptr [g_PauseRecordTiming]
        add esp, 4
        pop esi
        fld qword ptr [eax + 24h]
        fdiv qword ptr [eax + 2Ch]
        mov eax, dword ptr [g_SecondaryActorDifficulty]
        lea eax, [eax + eax * 4]
        lea eax, [ebx + eax * 2]
        lea ecx, [eax * 8]
        sub ecx, eax
        mov eax, ebx
        fstp dword ptr [esp + 0Ch]
        fld dword ptr [esp + 0Ch]
        fld qword ptr [g_PauseRecordHundred]
        fmul st(1), st
        fsubrp st(1), st
        fstp dword ptr [esp + 0Ch]
        fld dword ptr [esp + 0Ch]
        fstp dword ptr [ebp + ecx * 4 + 28h]
        pop ebp
        pop ebx
        ret 4
    }
}
