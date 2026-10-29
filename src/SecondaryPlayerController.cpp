#include "SecondaryPlayerController.hpp"
#include "InputBackend.hpp"
#include "AngleMath.hpp"

namespace
{
__forceinline int Subtract(int a, int b)
{ return static_cast<int>(static_cast<unsigned int>(a) - b); }
__forceinline int Product(int a, int b)
{ return static_cast<int>(static_cast<unsigned int>(a) * b); }
__forceinline SecondaryControllerHudView *Hud()
{ return reinterpret_cast<SecondaryControllerHudView *>(g_SecondaryActorHud); }
__forceinline SecondaryControllerHostView *Host()
{ return reinterpret_cast<SecondaryControllerHostView *>(g_SecondaryProgramHost); }
__forceinline bool SpecialRequested()
{
    return SecondaryActorEnemy() && !SecondaryActorEnemy()->active &&
        g_SecondaryPatternPower / g_SecondaryPatternPowerUnit != 0 && (g_InputState.current & 2);
}
__forceinline void Decrement(SecondaryDamageTimerView *time)
{
    time->previous = time->current;
    if (*time->scale > static_cast<double>(0.99f) && *time->scale < 1.01f)
        time->fraction = static_cast<float>(time->fraction - 1.0);
    else
    {
        float step = *time->scale * 1.0f;
        time->fraction = time->fraction - step;
    }
    time->current = static_cast<int>(time->fraction);
}
__forceinline void Advance(StageTimerView *time)
{
    time->previous = time->current;
    if (*time->scale > static_cast<double>(0.99f) && *time->scale < 1.01f)
    {
        time->current = static_cast<int>(static_cast<unsigned int>(time->current) + 1);
        time->fraction = static_cast<float>(time->fraction + 1.0);
    }
    else
    {
        time->fraction = *time->scale + time->fraction;
        time->current = static_cast<int>(time->fraction);
    }
}
__forceinline CameraVector Difference(const CameraVector &a, const CameraVector &b)
{
    CameraVector result = { a.x - b.x, a.y - b.y, a.z - b.z };
    return result;
}
__forceinline CameraVector Sum(const CameraVector &a, const CameraVector &b)
{
    CameraVector result = { a.x + b.x, a.y + b.y, a.z + b.z };
    return result;
}
__forceinline CameraVector Half(const CameraVector &a)
{
    CameraVector result = { static_cast<float>(a.x * 0.5),
        static_cast<float>(a.y * 0.5), static_cast<float>(a.z * 0.5) };
    return result;
}
}

int __stdcall UpdateSecondaryPlayerController(SecondaryDamageReferencesView *self)
{
    SecondaryPlayerControllerView *owner = reinterpret_cast<SecondaryPlayerControllerView *>(self);
    switch (owner->mode)
    {
    case 0:
        g_SecondaryControllerGauge = 0;
        owner->y = Subtract(0xf000, Product(owner->stateTime.current, 0x2800) / 60);
        owner->position.y = static_cast<float>(owner->y * (1.0 / 128.0));
        RefreshSecondaryPlayerEmitters(self);
        owner->fireCount = 0;
        owner->unknown924 = 0;
        if (owner->stateTime.current >= 30)
        {
            ClearSceneProjectiles(1);
            ClearSecondaryObjects(0, 1);
        }
        else
        {
            float radius = static_cast<float>(owner->stateTime.current * 512.0 / 30.0 + 64.0);
            ClearSceneProjectilesInRadius(&owner->savedPosition, radius, 0, 1);
            float innerRadius = static_cast<float>(radius * 0.25);
            ClearSceneProjectilesInRadius(&owner->savedPosition, innerRadius, 0, 0);
            ClearSecondaryObjectsInRadius(&owner->savedPosition, radius, 0, 1);
            ClearSecondaryObjectsInRadius(&owner->savedPosition, innerRadius, 0, 0);
        }
        if (owner->stateTime.current < 60) break;
        owner->mode = 1;
        SetStageTimer(&owner->stateTime, 0);
        // The state0 completion enters state1 in the same frame.
    case 1:
        if (Hud() && !Hud()->blocked && SpecialRequested())
        {
            ActivateSecondaryPlayerSpecial();
            g_SecondaryPatternPower = Subtract(g_SecondaryPatternPower, g_SecondaryPatternPowerUnit);
            ResetSecondaryPlayerEmitters(self);
        }
        if (owner->stateTime.current < 30)
        {
            ClearSceneProjectiles(1);
            ClearSecondaryObjects(0, 1);
        }
        UpdateSecondaryPlayerMovement(self);
        break;
    case 4:
        g_SecondaryControllerGauge = 0;
        if (owner->stateTime.current >= 8)
        {
            BeginSecondaryPlayerDeath(self);
            Host()->deaths = static_cast<int>(static_cast<unsigned int>(Host()->deaths) + 1);
            Host()->deathState = 0;
        }
        else
        {
            if (SpecialRequested())
            {
                SetStageTimer(&owner->stateTime, 60);
                ActivateSecondaryPlayerSpecial();
                g_SecondaryPatternPower = Subtract(g_SecondaryPatternPower, g_SecondaryPatternPowerUnit);
                ResetSecondaryPlayerEmitters(self);
                owner->mode = 1;
            }
            break;
        }
        // State4 after the death transition enters state2 processing immediately.
    case 2:
        g_SecondaryControllerGauge = 0;
        if (owner->stateTime.current == 3)
        {
            int power = g_SecondaryPatternPower;
            g_SecondaryPatternPower = Subtract(g_SecondaryPatternPower, g_SecondaryPatternMaximumPower);
            if (g_SecondaryPatternPower < 0) g_SecondaryPatternPower = 0;
            CameraVector target = { 0.0f, static_cast<float>(owner->position.y - 224.0), 0.0f };
            float angle = SecondaryPlayerAimAngle(self, &target);
            int kinds[7] = { 1, 1, 1, 1, 1, 1, 1 };
            if (g_SecondaryPlayerLives <= 0)
                kinds[3] = 6;
            else if (power >= Product(g_SecondaryPatternPowerUnit, 7))
            {
                kinds[6] = kinds[5] = kinds[3] = kinds[0] = kinds[1] = kinds[4] = kinds[2] = 4;
            }
            else if (power >= Product(g_SecondaryPatternPowerUnit, 6))
            {
                kinds[6] = kinds[5] = kinds[0] = kinds[1] = kinds[4] = kinds[2] = 4;
            }
            else if (power >= Product(g_SecondaryPatternPowerUnit, 5))
            {
                kinds[5] = kinds[3] = kinds[1] = kinds[4] = kinds[2] = 4;
            }
            else if (power >= Product(g_SecondaryPatternPowerUnit, 4))
            {
                kinds[5] = kinds[1] = kinds[4] = kinds[2] = 4;
            }
            else if (power >= Product(g_SecondaryPatternPowerUnit, 3) || g_SecondaryPlayerLives < 3)
            {
                kinds[5] = kinds[3] = kinds[1] = 4;
            }
            else
                kinds[4] = kinds[2] = 4;
            for (int i = 0; i < 7; ++i)
                SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &owner->position,
                    kinds[i], 0xffffff, static_cast<float>(i * static_cast<double>(3.1415927f) / 28.0 +
                        angle - static_cast<double>(0.3926991f)), 3.0f);
            ResetSecondaryPlayerEmitters(self);
        }
        if (owner->stateTime.current >= 30)
        {
            if (g_SecondaryPlayerLives < 0)
            {
                if (g_SecondaryControllerMode->mode == 1) EnterSecondaryContinueMenu();
                else EnterSecondaryGameOverMenu();
            }
            else
            {
                owner->mode = 0;
                g_AnimationTimeScale = 1.0f;
                AllocateSecondaryDamageRegion(self, &owner->position, 32.0f, 16.0f, 30, 150);
                CameraVector position = owner->position;
                owner->position.x = 0.0f;
                owner->savedPosition = position;
                owner->position.y = 480.0f;
                owner->x = 0;
                owner->y = 0xf000;
                SetStageTimer(&owner->statusTime, 280);
                SetStageTimer(&owner->stateTime, 0);
            }
        }
        break;
    case 3:
        if (owner->stateTime.current != 4 && owner->stateTime.current == 15)
        {
            ClearSceneProjectiles(0);
            ClearSecondaryObjects(1, 0);
        }
        break;
    }
    for (int i = 0; i < 32; ++i)
    {
        SecondaryDamageRegionView *region = &owner->regions[i];
        if (!(region->flags & 1)) continue;
        if (!(region->motion.flags & 1))
        {
            reinterpret_cast<SecondaryVector2 *>(&region->motion.velocity)->SetPolar(
                region->motion.angle, region->motion.speed);
            region->motion.velocity.z = 0.0f;
        }
        else
        {
            region->motion.radius = region->motion.radialVelocity + region->motion.radius;
            region->motion.angle = NormalizeAngle(region->motion.speed + region->motion.angle);
        }
        ComposeSecondaryMotion(&region->motion);
        region->radius = region->growth + region->radius;
        region->angle = *reinterpret_cast<float *>(&region->unknown00c) + region->angle;
        Decrement(&region->time);
        if (region->time.current <= 0) region->flags &= ~1u;
    }
    bool flash = false;
    if (owner->statusTime.current > 0)
    {
        Decrement(reinterpret_cast<SecondaryDamageTimerView *>(&owner->statusTime));
        if (owner->stateTime.current != owner->stateTime.previous && owner->stateTime.current % 3 == 0)
        {
            owner->animation.flags.value |= 0x8000;
            reinterpret_cast<SecondaryActorAnimationView *>(&owner->animation)->color = 0xff0000ff;
            flash = true;
        }
    }
    if (!flash) owner->animation.flags.value &= ~0x8000u;
    UpdateSecondaryControllerAnimation(&owner->animation);
    owner->minimum = Difference(owner->position, owner->halfSize);
    owner->maximum = Sum(owner->position, owner->halfSize);
    owner->scaledMinimum = Difference(owner->position, Half(owner->size));
    owner->scaledMaximum = Sum(owner->position, Half(owner->size));
    owner->otherMinimum = Difference(owner->position, owner->otherSize);
    // These two native bounds add the extent first for Y/Z.
    CameraVector otherMaximum = { owner->position.x + owner->otherSize.x,
        owner->otherSize.y + owner->position.y, owner->otherSize.z + owner->position.z };
    owner->otherMaximum = otherMaximum;
    owner->fullMinimum = Difference(owner->position, owner->size);
    CameraVector fullMaximum = { owner->position.x + owner->size.x,
        owner->size.y + owner->position.y, owner->size.z + owner->position.z };
    owner->fullMaximum = fullMaximum;
    Advance(&owner->stateTime);
    Advance(&owner->auxiliaryTime);
    if (!Hud()->blocked && Host() && Host()->enabled && owner->stateTime.current % 60 == 0)
    {
        g_SecondaryPlayerValue = static_cast<int>(static_cast<unsigned int>(g_SecondaryPlayerValue) + 1);
        if (g_SecondaryPlayerValue > 1024) g_SecondaryPlayerValue = 1024;
        else if (g_SecondaryPlayerValue < -1024) g_SecondaryPlayerValue = -1024;
    }
    if (!Hud()->blocked && Host() && Host()->enabled && !(Hud()->flags & 0x10))
    {
        UpdateSecondaryPattern(self);
        UpdateSecondaryProjectiles(self);
        return 1;
    }
    StageTimerView *time = &owner->patternTime;
    if (!(time->flags & 1))
    {
        time->fraction = 0.0f;
        time->current = 0;
        time->previous = -999999;
        time->scale = &g_AnimationTimeScale;
        time->flags |= 1;
    }
    time->current = -1;
    time->fraction = -1.0f;
    time->previous = -2;
    self->tracked = 0;
    self->trackingLocked = 0;
    UpdateSecondaryProjectiles(self);
    return 1;
}
