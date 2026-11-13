#include "SecondaryPlayerController.hpp"

// Native 0x497ED0 is the double 1/128. The game uses this exact x87 sequence
// to convert its two integer coordinates before resetting eight E4-stride
// controller-region flags; keep the register-level ABI and operation order.
extern const double g_SecondaryIntegerPositionScale;

__declspec(noinline) void __fastcall SetSecondaryIntegerPlayerPosition(const int *position)
{
    __asm {
        mov eax, dword ptr [g_SecondaryControllerMode]
        mov edx, dword ptr [ecx]
        mov dword ptr [eax+888h], edx
        mov ecx, dword ptr [ecx+4]
        mov dword ptr [eax+88Ch], ecx
        fild dword ptr [eax+888h]
        fld qword ptr [g_SecondaryIntegerPositionScale]
        mov ecx, 1
        fmul st(1), st(0)
        fxch st(1)
        fstp dword ptr [eax+87Ch]
        fimul dword ptr [eax+88Ch]
        fstp dword ptr [eax+880h]
        mov dword ptr [eax+7644h], ecx
        mov dword ptr [eax+7728h], ecx
        mov dword ptr [eax+780Ch], ecx
        mov dword ptr [eax+78F0h], ecx
        mov dword ptr [eax+79D4h], ecx
        mov dword ptr [eax+7AB8h], ecx
        mov dword ptr [eax+7B9Ch], ecx
        mov dword ptr [eax+7C80h], ecx
    }
}
