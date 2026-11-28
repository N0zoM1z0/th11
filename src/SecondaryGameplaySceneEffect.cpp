#include "SecondaryGameplay.hpp"

// Gameplay objects whose update and draw chain elements sit at +8/+0xC, named
// by address like g_SecondaryGameplayChainsD74/EC0. 0x4A8E8C is the 0x1A8-byte
// game.cpp object built by 0x420E90; 0x4A8D6C is also g_MeshEffectSpell.
extern SecondaryGameplayChainsView *g_SecondaryGameplayChainsD6C;
extern SecondaryGameplayChainsView *g_SecondaryGameplayChainsE8C;
extern SecondaryGameplayChainsView *g_SecondaryGameplayChainsEB0;

// Each lives in its own TU in the target, so each repeats the two tests; a
// shared __forceinline helper schedules the ECX load before the EDX constant.
// 0x40BE20
void __cdecl PauseSecondaryGameplayItems()
{
    SecondaryGameplayChainsView *self = g_SecondaryGameplayChainsD6C;
    if (self->update) self->update->flags |= 2;
    if (self->draw) self->draw->flags |= 2;
}

// 0x421070
void __cdecl PauseSecondaryGameplaySceneEffect()
{
    SecondaryGameplayChainsView *self = g_SecondaryGameplayChainsE8C;
    if (self->update) self->update->flags |= 2;
    if (self->draw) self->draw->flags |= 2;
}

// 0x42BD90
void __cdecl PauseSecondaryGameplayBoss()
{
    SecondaryGameplayChainsView *self = g_SecondaryGameplayChainsEB0;
    if (self->update) self->update->flags |= 2;
    if (self->draw) self->draw->flags |= 2;
}
