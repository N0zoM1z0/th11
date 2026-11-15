#include "StageTween.hpp"

extern "C" const double g_StageTimerScaleLower = static_cast<double>(0.99f);
extern "C" const float g_StageTimerScaleUpper = 1.01f;
extern "C" const double g_StageTimerOne = 1.0;
extern "C" int __cdecl _ftol2_sse(void);

// The native timer helper is called with its StageTimerView in ESI. It saves
// ECX, updates previous before either branch, and returns current in EAX.
__declspec(naked) int __stdcall AdvanceStageTimer(StageTimerView *self)
{
    __asm
    {
        push ecx
        mov edx, dword ptr [esi + 4]
        mov ecx, dword ptr [esi + 0ch]
        mov dword ptr [esi], edx
        fld dword ptr [ecx]
        fcomp qword ptr [g_StageTimerScaleLower]
        fnstsw ax
        test ah, 41h
        jne advance_stage_timer_fractional
        fld dword ptr [g_StageTimerScaleUpper]
        fcomp dword ptr [ecx]
        fnstsw ax
        test ah, 41h
        jne advance_stage_timer_fractional
        fld dword ptr [esi + 8]
        inc edx
        fadd qword ptr [g_StageTimerOne]
        mov dword ptr [esi + 4], edx
        mov eax, edx
        fstp dword ptr [esi + 8]
        pop ecx
        ret

    advance_stage_timer_fractional:
        fld dword ptr [ecx]
        fadd dword ptr [esi + 8]
        fstp dword ptr [esp]
        fld dword ptr [esp]
        fst dword ptr [esi + 8]
        call _ftol2_sse
        mov dword ptr [esi + 4], eax
        pop ecx
        ret
    }
}

extern "C" int __stdcall StageTimerProbe(StageTimerView *timer)
{
    __asm
    {
        push esi
        mov esi, timer
        call AdvanceStageTimer
        pop esi
        ret 4
    }
}
