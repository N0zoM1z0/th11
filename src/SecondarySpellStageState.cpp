#include "SecondarySpellStage.hpp"
#include "SecondaryActorState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "StageTween.hpp"
#include <stddef.h>

extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;


// Match the native renderer lookup boundary: hidden context in ECX, renderer
// in EDX, and the animation ID as the sole stack argument.
#pragma optimize("y", on)
__declspec(noinline) AnimationVmView *__fastcall FindSecondarySpellVm(
    void *context, RenderBatchView *renderer, AnimationVmId id)
{
    if (!id.value)
        return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(renderer);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id.value)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id.value)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}
#pragma optimize("", on)

#pragma optimize("y", on)
__declspec(noinline) int StageTimerView::Advance()
{
    int previous = current;
    this->previous = previous;
    if (*scale > static_cast<double>(0.99f) && *scale < 1.01f)
    {
        ++previous;
        current = previous;
        fraction = static_cast<float>(fraction + 1.0);
        return previous;
    }
    fraction = *scale + fraction;
    current = static_cast<int>(fraction);
    return current;
}
#pragma optimize("", on)

struct SecondarySpellPositionView
{
    float x, y, z;
    void PositionAnimation(AnimationVmId id) const;
};
typedef char SecondarySpellPositionViewSize[(sizeof(SecondarySpellPositionView) == sizeof(CameraVector)) ? 1 : -1];

#pragma optimize("y", on)
__declspec(noinline) void SecondarySpellPositionView::PositionAnimation(AnimationVmId id) const
{
    AnimationVmView *vm = FindSecondarySpellVm(
        const_cast<SecondarySpellPositionView *>(this),
        *reinterpret_cast<RenderBatchView * volatile *>(&g_RenderBatch), id);
    if (!vm)
        return;
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    SecondaryActorAnimationView *actor = reinterpret_cast<SecondaryActorAnimationView *>(vm);
    actor->position.x = static_cast<float>(x + 32.0 + 192.0);
    actor->position.y = static_cast<float>(y + 16.0);
    actor->position.z = z;
    if (!bound->marker018)
    {
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
        {
            float *childPosition = &reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->position.x;
            childPosition[0] = static_cast<float>(x + 32.0 + 192.0);
            childPosition[1] = static_cast<float>(y + 16.0);
            childPosition[2] = z;
        }
    }
}
#pragma optimize("", on)

static __forceinline void SetSpellFollowerEventThree(SecondarySpellStageView *self)
{
    register int remaining = 3;
    unsigned int *follower = self->followerIds;
    do
    {
        AnimationVmId followerId(*follower);
        RenderBatchView *renderer = *reinterpret_cast<RenderBatchView * volatile *>(&g_RenderBatch);
        AnimationVmView *vm = FindSecondarySpellVm(self, renderer, followerId);
        if (vm)
        {
            reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 3;
            BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
            if (!bound->marker018)
                for (BoundVmNodeView *node = bound->related; node; node = node->next)
                    reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 3;
        }
        ++follower;
    } while (--remaining);
}

static __forceinline void SetSpellFollowerEventTwo(SecondarySpellStageView *self)
{
    RenderBatchView *renderer = *reinterpret_cast<RenderBatchView * volatile *>(&g_RenderBatch);
    unsigned int *follower = self->followerIds;
    int remaining = 3;
    do
    {
        AnimationVmView *vm = FindSecondarySpellVm(self, renderer, AnimationVmId(*follower));
        if (vm)
        {
            reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 2;
            BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
            if (!bound->marker018)
                for (BoundVmNodeView *node = bound->related; node; node = node->next)
                    reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 2;
        }
        ++follower;
        --remaining;
    } while (remaining);
}

// Full original 0x40C0A0..0x40C30F. This is a normal C++ object updater:
// VC9 /GL selected EDI for its private receiver in the native caller.
int __stdcall UpdateSecondarySpellStageState(SecondarySpellStageView *self)
{
    if (self->flags & 1u)
    {
        ++self->activeFrames;
        if (self->time.current >= 60)
            g_StagePrimary->flags2ff0 &= ~1u;

        if (self->time.current >= 300 && !(self->flags & 8u))
        {
            const int reduction =
                (self->scoreInitial - self->scoreInitial / 10) /
                (self->scoreDuration - 300);
            int score = self->score - reduction;
            self->score = score - score % 10;
        }

        ExecuteAnimationVm(&self->vm0);
        ExecuteAnimationVm(&self->vm1);
        self->time.Advance();

        if (self->time.current >= 120)
        {
            if (!(self->flags & 4u))
            {
                if (g_SecondaryPlayerControllerInstance->position.y < 96.0f)
                {
                    SetSpellFollowerEventThree(self);
                    self->flags |= 4u;
                }
            }
            else if (g_SecondaryPlayerControllerInstance->position.y > 128.0f)
            {
                SetSpellFollowerEventTwo(self);
                self->flags &= ~4u;
            }
        }

        CameraVector *position = &self->position;
        const CameraVector *target = &g_SecondaryMaster->Slot(0)->state.world.position;
        __declspec(align(8)) CameraVector delta = {
            static_cast<float>(target->x - position->x),
            static_cast<float>(target->y - position->y),
            static_cast<float>(target->z - position->z)
        };
        CameraVector scaled = {
            static_cast<float>(delta.x * 0.05),
            static_cast<float>(delta.y * 0.05),
            static_cast<float>(delta.z * 0.05)
        };
        volatile float &positionX = position->x;
        positionX = static_cast<float>(positionX + scaled.x);
        volatile float &positionY = position->y;
        positionY = static_cast<float>(positionY + scaled.y);
        position->z = static_cast<float>(scaled.z + position->z);

        reinterpret_cast<SecondarySpellPositionView *>(position)->PositionAnimation(AnimationVmId(self->positionVmId));
        if ((self->flags & 0x20u) &&
            !reinterpret_cast<StageAuxiliaryView *>(g_MeshEffectEnemy)->effectActive)
            self->flags &= ~0x20u;
    }
    return 1;
}
