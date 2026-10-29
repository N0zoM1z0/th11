#include "SecondaryPattern.hpp"
#include "ResourceFile.hpp"
#include "InputBackend.hpp"
#include "AnimationImmediateDraw.hpp"

int __stdcall DrawSecondaryPlayerEmitters(SecondaryDamageReferencesView *self)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    if (owner->mode != 2)
    {
        RenderBatchView *renderer = g_RenderBatch;
        AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(reinterpret_cast<unsigned char *>(self) + 0x14);
        SecondaryActorAnimationView *animation = reinterpret_cast<SecondaryActorAnimationView *>(vm);
        animation->position.x = static_cast<float>(owner->position.x + 32.0 + 192.0);
        animation->position.y = static_cast<float>(owner->position.y + 16.0);
        animation->position.z = owner->position.z;
        DrawAnimationVmByMode(renderer, vm);
        SecondaryEmitterObjectView *emitter = &owner->emitters[0];
        for (int remaining = 8; remaining; --remaining, ++emitter)
            if (emitter->onDraw) emitter->onDraw(emitter);
    }
    return 1;
}

int __stdcall LoadSecondaryProjectilePatterns(SecondaryDamageReferencesView *self, const char *filename)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    owner->configuration = reinterpret_cast<SecondaryPatternConfigurationView *>(LoadResourceFile(filename, 0, 0));
    if (!owner->configuration) return -1;
    for (int i = 0; i < owner->configuration->patternCount; ++i)
    {
        SecondaryProjectileParameters *&entry = owner->configuration->Pattern(i);
        entry = reinterpret_cast<SecondaryProjectileParameters *>(
            reinterpret_cast<unsigned int>(entry) + reinterpret_cast<unsigned int>(owner->configuration));
        for (SecondaryProjectileParameters *parameters = owner->configuration->Pattern(i);
            parameters->period >= 0; ++parameters)
        {
            parameters->onSpawn = g_SecondaryProjectileSpawnCallbacks[reinterpret_cast<unsigned int>(parameters->onSpawn)];
            parameters->onUpdate = g_SecondaryProjectileUpdateCallbacks[reinterpret_cast<unsigned int>(parameters->onUpdate)];
            parameters->unknown02c = g_SecondaryProjectileOtherCallbacks[parameters->unknown02c];
            parameters->onHit = g_SecondaryProjectileHitCallbacks[reinterpret_cast<unsigned int>(parameters->onHit)];
        }
    }
    return 0;
}

int __stdcall EmitSecondaryPatternFrame(SecondaryDamageReferencesView *self, int frame)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    int level = g_SecondaryPatternPower / g_SecondaryPatternPowerUnit;
    int selection = static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3 + g_MeshEffectSelection1);
    if (selection != 4)
    {
        if (owner->focused)
            level = static_cast<int>(static_cast<unsigned int>(level) + owner->configuration->levelCount + 1);
    }
    else
        level = static_cast<int>(static_cast<unsigned int>(level) +
            (static_cast<unsigned int>(owner->configuration->levelCount) + 1) * owner->multiplier);
    for (SecondaryProjectileParameters *parameters = owner->configuration->Pattern(level);
        parameters->period >= 0; ++parameters)
    {
        if (frame % parameters->period == parameters->phase)
            SpawnSecondaryProjectile(self, parameters, &owner->position, static_cast<unsigned int>(frame));
    }
    return 0;
}

int __stdcall UpdateSecondaryPattern(SecondaryDamageReferencesView *self)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    if (owner->mode != 1)
    {
        self->tracked = 0;
        self->trackingLocked = 0;
        return 0;
    }
    if (self->patternTime.current < 0)
    {
        if (owner->fireCount >= 99 || !(g_InputState.current & 1)) return 0;
        SetStageTimer(&self->patternTime, 0);
    }
    StageTimerView &time = self->patternTime;
    if (time.current != time.previous)
        EmitSecondaryPatternFrame(self, time.current);
    if (time.current >= 14)
    {
        if (g_InputState.current & 1) OffsetSecondaryTimer(&time, -14.0f);
        else SetStageTimer(&time, -1);
    }
    else AdvanceStageTimer(&time);
    return 0;
}
