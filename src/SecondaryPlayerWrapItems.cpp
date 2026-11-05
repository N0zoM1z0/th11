#include "SecondaryPlayerWrapItems.hpp"
#include "InputBackend.hpp"
#include "MeshEffects.hpp"

namespace
{
__forceinline int Add(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) + right); }
__forceinline void Advance(SecondaryPlayerMovementView *owner)
{ owner->unknown924 = Add(owner->unknown924, 1); }
__forceinline void Refresh(SecondaryPlayerMovementView *owner)
{
    for (int i = 0; i < 8; ++i) owner->emitters[i].refresh = 1;
}
}

void __stdcall UpdateSecondaryPlayerHorizontalWrap(SecondaryDamageReferencesView *self)
{
    SecondaryPlayerMovementView *owner = reinterpret_cast<SecondaryPlayerMovementView *>(self);
    int state = owner->fireCount;
    if (!state) return;
    SecondaryControllerHudView *hud = reinterpret_cast<SecondaryControllerHudView *>(g_SecondaryActorHud);
    SecondaryControllerHostView *host = reinterpret_cast<SecondaryControllerHostView *>(g_SecondaryProgramHost);
    if (state >= 99 || (!(g_InputState.current & 9) && !hud->blocked && (!host || host->enabled)))
    {
        if (state == 1)
        {
            if (!owner->direction)
            {
                owner->unknown924 = 0;
                Advance(owner);
                owner->fireCount = 2;
                return;
            }
            if (owner->direction != 3)
            {
                owner->fireCount = 0;
                Advance(owner);
                return;
            }
            Advance(owner);
            return;
        }
        if (state == 2)
        {
            if (owner->unknown924 <= 8)
            {
                if (owner->direction == 3 && !hud->blocked && host && host->enabled)
                {
                    owner->fireCount = 99;
                    owner->unknown924 = 0;
                    QueueMeshEffectSound(47);
                    ResetSecondaryPlayerEmitters(self);
                }
                Advance(owner);
                return;
            }
        }
        else if (state == 3)
        {
            if (!owner->direction)
            {
                owner->unknown924 = 0;
                Advance(owner);
                owner->fireCount = 4;
                return;
            }
            if (owner->direction != 4)
            {
                owner->fireCount = 0;
                Advance(owner);
                return;
            }
            Advance(owner);
            return;
        }
        else if (state == 4)
        {
            if (owner->unknown924 <= 8)
            {
                if (owner->direction == 4 && !hud->blocked && host && host->enabled)
                {
                    owner->fireCount = 100;
                    owner->unknown924 = 0;
                    QueueMeshEffectSound(48);
                    ResetSecondaryPlayerEmitters(self);
                }
                Advance(owner);
                return;
            }
        }
        else
        {
            if (state == 99)
            {
                if (owner->x <= -0x6800)
                {
                    owner->x = Add(owner->x, 0xd000);
                    Refresh(owner);
                }
            }
            else if (state == 100)
            {
                if (owner->x >= 0x6800)
                {
                    owner->x = Add(owner->x, -0xd000);
                    Refresh(owner);
                }
            }
            else
            {
                Advance(owner);
                return;
            }
            if (owner->unknown924 < 45)
            {
                Advance(owner);
                return;
            }
        }
    }
    owner->fireCount = 0;
    owner->unknown924 = 0;
    Advance(owner);
}

void __stdcall AttractSecondaryPlayerItems()
{
    SecondaryPatternOwnerView *player = reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryActorReferences);
    SecondaryAttractionItemView *item = &g_SecondaryAttractionItems->items[0];
    for (int index = 0; index < 0x896; ++index, ++item)
    {
        if (item->state && item->state == 1)
        {
            item->parameter = reinterpret_cast<SecondaryAttractionConfigurationView *>(player->configuration)->parameter;
            item->state = 3;
        }
    }
}
