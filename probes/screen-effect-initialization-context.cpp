#include "SecondaryHud.hpp"

extern "C" int __stdcall SecondaryHudOwnersProbe(SecondaryGameplayView *gameplay,
    SecondaryEclDebugView *debug, SecondaryHudView *hud)
{
    int status = SecondaryGameplayUpdateCallback(gameplay);
    UpdateSecondaryEclDebug(debug);
    SecondaryHudUpdateCallback(hud);
    return status;
}
