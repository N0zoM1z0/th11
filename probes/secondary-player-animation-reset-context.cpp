#include "SecondaryPlayerController.hpp"
// Isolated natural caller; other unresolved game bindings are diagnostic only.
void __cdecl ResetSecondaryPlayerControllerState();
extern "C" void __cdecl SecondaryPlayerAnimationResetProbe()
{
    ResetSecondaryPlayerControllerState();
}
