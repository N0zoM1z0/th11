#include "SecondaryGameplay.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"

// Original TH11 0x40D190: pause both player-linked chain entries and then
// reset the same player's emitter pattern state. The source retains one
// real player singleton capture and does not invent pointer guards.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;

void __cdecl PauseSecondaryGameplayPlayer()
{
    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    SecondaryGameplayChainsView *chains =
        reinterpret_cast<SecondaryGameplayChainsView *>(player);
    chains->update->flags |= 2u;
    chains->draw->flags |= 2u;
    ResetSecondaryPlayerEmitters(
        reinterpret_cast<SecondaryDamageReferencesView *>(player));
}
