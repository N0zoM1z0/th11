#include "../src/SecondaryPlayerController.hpp"
extern void __fastcall SetSecondaryIntegerPlayerPosition(const int *position);
extern "C" void __stdcall SecondaryIntegerPlayerPositionProbe(const int *position)
{
    SetSecondaryIntegerPlayerPosition(position);
}
