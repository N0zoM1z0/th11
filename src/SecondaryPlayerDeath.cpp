#include "SecondaryPlayerDeath.hpp"

namespace
{
__forceinline int Subtract(int left, int right)
{
    return static_cast<int>(static_cast<unsigned int>(left) - right);
}

__forceinline void SetTimer(StageTimerView *time, int frame)
{
    if (!(time->flags & 1))
    {
        time->fraction = 0.0f;
        time->current = 0;
        time->previous = -999999;
        time->scale = &g_AnimationTimeScale;
        time->flags |= 1;
    }
    time->current = frame;
    time->previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
    time->fraction = static_cast<float>(frame);
}

__forceinline AnimationVmView *EmitterVm(RenderBatchView *renderer, unsigned int id)
{
    if (!id) return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(renderer);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}

__forceinline void SignalEmitterVm(RenderBatchView *renderer, unsigned int id)
{
    AnimationVmView *vm = EmitterVm(renderer, id);
    if (!vm) return;
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
}
}

void __stdcall BeginSecondaryPlayerDeath(SecondaryDamageReferencesView *self)
{
    SecondaryPlayerControllerView *owner = reinterpret_cast<SecondaryPlayerControllerView *>(self);
    g_SecondaryActorScore = Subtract(g_SecondaryActorScore, 10000000);
    g_SecondaryPlayerLives = Subtract(g_SecondaryPlayerLives, 1);
    if (g_SecondaryActorScore < g_SecondaryScoreFloor) g_SecondaryActorScore = g_SecondaryScoreFloor;
    g_SecondaryScoreFraction = 0;
    if (g_SecondaryPlayerLives >= 0)
        UpdateSecondaryLivesDisplay(g_SecondaryActorHud, g_SecondaryPlayerLives, g_SecondaryLifeFragments);

    owner->mode = 2;
    owner->stateTime.SetFrame(-1);
    SetTimer(&owner->statusTime, 180);
    BindAnimationScript(self->file, 0, &owner->animation);

    RenderBatchView *renderer = g_RenderBatch;
    SecondaryPatternOwnerView *pattern = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    for (int i = 0; i < 8; ++i)
    {
        SecondaryEmitterObjectView *emitter = &pattern->emitters[i];
        emitter->state = 0;
        SignalEmitterVm(renderer, emitter->idB0);
        SignalEmitterVm(renderer, emitter->idB4);
    }
    self->patternSlot = 0;
    if (g_SecondaryControllerMode->mode != 1) ApplySecondaryPlayerDeathPenalty(&owner->position);

    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if (spell->flags & 1)
    {
        if (spell->age >= 60)
        {
            spell->value8e4 = 0;
            spell->flags &= ~0x22u;
        }
        else if (SecondaryActorEnemy()->active &&
            static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3 + g_MeshEffectSelection1) != 5)
            spell->flags |= 0x20;
    }
    g_SecondaryPlayerValue = Subtract(g_SecondaryPlayerValue, 0x400);
    if (g_SecondaryPlayerValue > 0x400) g_SecondaryPlayerValue = 0x400;
    else if (g_SecondaryPlayerValue < -0x400) g_SecondaryPlayerValue = -0x400;
}

void __stdcall UpdateSecondaryLivesDisplay(SecondaryActorHudView *hud, int lives, int fragments)
{
    SecondaryLivesDisplayView *display = reinterpret_cast<SecondaryLivesDisplayView *>(hud);
    int i = 0;
    for (; i < lives; ++i)
    {
        display->lives[i].flags.value |= 2;
        reinterpret_cast<SecondaryActorAnimationView *>(&display->lives[i])->event = 2;
    }
    if (i >= 9) return;
    display->lives[i].flags.value |= 2;
    reinterpret_cast<SecondaryActorAnimationView *>(&display->lives[i])->event =
        static_cast<short>(static_cast<unsigned int>(fragments) + 7);
    for (++i; i < 9; ++i) display->lives[i].flags.value &= ~2u;
}
