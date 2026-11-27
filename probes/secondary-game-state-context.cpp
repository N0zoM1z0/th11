#include "../src/SecondaryPattern.hpp"
#include "../src/SecondaryPlayerController.hpp"
#include "../src/SecondaryPlayerDeath.hpp"
#include "../src/StageResource.hpp"

// Native 0x4A56E0..0x4A576F is one 0x90-byte global game-state object. Its
// constructor (0x420E20) and member calls pass its address, so VC9 must treat
// every field as reachable through pointer stores and reload it afterwards.
// The reconstruction still names the fields as separate globals; this link
// context exposes their addresses the way the native object's address is
// exposed. It contains no game logic and is not part of any exact owner.
extern int g_SecondaryActorScore, g_MeshEffectSelection0, g_MeshEffectSelection1,
    g_SecondaryCollisionCount;
extern unsigned int g_ReplayStageBombs, g_ReplayStageCharacter, g_ReplayStageBonus,
    g_ReplayStageFlags, g_ReplayStageExtraScore, g_ReplayStatusMask;
extern int g_SecondaryPatternMaximumPower;
// 0x4C37E4 is a field of the address-exposed main runtime object.
extern unsigned int g_ReplayModeFlags;

void *g_SecondaryGameStateFields[16];

extern "C" __declspec(dllexport) void __stdcall ExposeSecondaryGameStateFields()
{
    void **field = g_SecondaryGameStateFields;
    field[0] = &g_SecondaryActorScore;
    field[1] = &g_SecondaryPatternPower;
    field[2] = &g_SecondaryPlayerValue;
    field[3] = &g_MeshEffectSelection0;
    field[4] = &g_MeshEffectSelection1;
    field[5] = &g_SecondaryPlayerLives;
    field[6] = &g_SecondaryPatternMaximumPower;
    field[7] = &g_SecondaryCollisionCount;
    field[8] = &g_ReplayStageBombs;
    field[9] = &g_ReplayStageCharacter;
    field[10] = &g_StageVariant;
    field[11] = &g_ReplayStageBonus;
    field[12] = &g_ReplayStageFlags;
    field[13] = &g_ReplayStageExtraScore;
    field[14] = &g_ReplayStatusMask;
    field[15] = &g_ReplayModeFlags;
}
