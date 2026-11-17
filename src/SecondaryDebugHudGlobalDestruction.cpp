#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"

// Native 0x41B360: the scene shutdown path deletes the real owning
// 0x4458-byte HUD object. The C++ delete expression invokes its
// separately restored destructor before operator delete.
void DestroySecondaryDebugHudSingleton()
{
    SecondaryDebugHudOwned *hud =
        reinterpret_cast<SecondaryDebugHudOwned *>(g_SecondaryActorHud);
    if (hud)
        delete hud;
}
