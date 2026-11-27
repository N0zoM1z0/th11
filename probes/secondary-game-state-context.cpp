// Native 0x4A56E0..0x4A576F is one 0x90-byte global game-state object. Its
// constructor (0x420E20) and member calls pass its address, so VC9 must treat
// every field as reachable through pointer stores and reload it afterwards.
// The reconstruction still names the fields as separate globals; this link
// context exposes their addresses the way the native object's address is
// exposed. It contains no game logic and is not part of any exact owner.
extern int g_SecondaryActorScore, g_SecondaryHudDisplayValue, g_SecondaryPatternPower,
    g_SecondaryPlayerValue, g_MeshEffectSelection0, g_SecondaryHudCharacterIndex,
    g_MeshEffectSelection1, g_SecondaryHudPartnerIndex, g_SecondaryPlayerLives,
    g_SecondaryLifeFragments, g_MenuPreviousVariant, g_SecondaryPatternMaximumPower,
    g_SecondaryCollisionCount;
extern unsigned int g_ReplayStageBombs, g_ReplayStageCharacter, g_StageVariant,
    g_ReplayStageBonus, g_ReplayStageFlags, g_ReplayStageExtraScore,
    g_ReplayStatusMask, g_StageResourceFlags;
extern unsigned char g_PauseRecordShotType;
// 0x4C37E4 is a field of the address-exposed main runtime object.
extern unsigned int g_ReplayModeFlags;

const void *g_SecondaryGameStateFields[24];

extern "C" __declspec(dllexport) void __stdcall ExposeSecondaryGameStateFields()
{
    const void **field = g_SecondaryGameStateFields;
    field[0] = &g_SecondaryActorScore;
    field[1] = &g_SecondaryHudDisplayValue;
    field[2] = &g_SecondaryPatternPower;
    field[3] = &g_SecondaryPlayerValue;
    field[4] = &g_MeshEffectSelection0;
    field[5] = &g_SecondaryHudCharacterIndex;
    field[6] = &g_MeshEffectSelection1;
    field[7] = &g_SecondaryHudPartnerIndex;
    field[8] = &g_SecondaryPlayerLives;
    field[9] = &g_SecondaryLifeFragments;
    field[10] = &g_MenuPreviousVariant;
    field[11] = &g_SecondaryPatternMaximumPower;
    field[12] = &g_SecondaryCollisionCount;
    field[13] = &g_ReplayStageBombs;
    field[14] = &g_ReplayStageCharacter;
    field[15] = &g_StageVariant;
    field[16] = &g_ReplayStageBonus;
    field[17] = &g_ReplayStageFlags;
    field[18] = &g_ReplayStageExtraScore;
    field[19] = &g_ReplayStatusMask;
    field[20] = &g_StageResourceFlags;
    field[21] = &g_PauseRecordShotType;
    field[22] = &g_ReplayModeFlags;
}
