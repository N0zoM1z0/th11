#include "SecondaryDamage.hpp"
#include <string.h>

int SecondaryDamageTimerView::Elapsed(int period) const
{
    return current != previous && current % period == 0;
}

void __stdcall SignalSecondaryProjectileHit(unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (vm)
    {
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 2;
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 2;
    }
}

void __stdcall ClearSecondaryProjectileHit(unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (vm)
    {
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 3;
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 3;
    }
}

void __stdcall PositionSecondaryDamageAnimation(CameraVector *output, const CameraVector *position)
{
    output->x = static_cast<float>(position->x + 32.0 + 192.0);
    output->y = static_cast<float>(position->y + 16.0);
    output->z = position->z;
}

SecondaryDamageRegionView *__stdcall AllocateSecondaryDamageRegion(
    SecondaryDamageReferencesView *self, const CameraVector *position,
    float radius, float growth, int frame, int damage)
{
    SecondaryDamageRegionView *region = &self->regions[0];
    for (int i = 0; i < 32; ++i, ++region)
    {
        if (region->flags & 1) continue;
        memset(region, 0, sizeof(*region));
        region->flags |= 3;
        memset(&region->motion, 0, sizeof(region->motion));
        region->motion.position = *position;
        region->radius = radius;
        region->growth = growth;
        SecondaryDamageTimerView &time = region->time;
        if (!(time.flags & 1))
        {
            time.fraction = 0.0f;
            time.current = 0;
            time.previous = -999999;
            time.scale = &g_AnimationTimeScale;
            time.flags |= 1;
        }
        time.current = frame;
        time.previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
        time.fraction = static_cast<float>(frame);
        region->damage = damage;
        region->accumulated = 0;
        region->limit = 999999;
        region->period = 4;
        return region;
    }
    return region;
}

int __stdcall SampleSecondarySpecialDamage2(SecondaryDamageEnemyView *self, const CameraVector *position)
{
    float distance = (position->x - self->x) * (position->x - self->x) +
        (position->y - self->y) * (position->y - self->y);
    return distance < self->radius * self->radius ? 17 : 0;
}

int __stdcall SampleSecondarySpecialDamage(const CameraVector *position, const float *extent)
{
    SecondaryDamageEnemyView *self = reinterpret_cast<SecondaryDamageEnemyView *>(g_MeshEffectEnemy);
    if (self->active)
    {
        int selection = static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3 + g_MeshEffectSelection1);
        switch (selection)
        {
        case 0:
        case 1:
        case 5:
            break;
        case 2:
            return SampleSecondarySpecialDamage2(self, position);
        case 3:
            return SampleSecondarySpecialDamage3(self, position, extent);
        case 4:
            return SampleSecondarySpecialDamage4(self, position);
        }
    }
    return 0;
}
