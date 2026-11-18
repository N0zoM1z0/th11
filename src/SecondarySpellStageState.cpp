#include "SecondarySpellStage.hpp"
#include "SecondaryActorState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "StageTween.hpp"
#include <stddef.h>

extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;

__declspec(noinline) int StageTimerView::Advance()
{
    int previous = current;
    this->previous = previous;
    if (*scale > static_cast<double>(0.99f) && *scale < 1.01f)
    {
        int next = static_cast<int>(static_cast<unsigned int>(previous) + 1);
        current = next;
        fraction = static_cast<float>(fraction + 1.0);
        return next;
    }
    fraction = *scale + fraction;
    current = static_cast<int>(fraction);
    return current;
}

static __forceinline void SetSpellFollowerEvent(SecondarySpellStageView *self,
    unsigned short event)
{
    for (int i = 0; i < 3; ++i)
    {
        AnimationVmView *vm = FindAnimationVmById(
            g_RenderBatch, AnimationVmId(self->followerIds[i]));
        if (vm)
        {
            reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
            BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
            if (!bound->marker018)
            {
                for (BoundVmNodeView *node = bound->related; node; node = node->next)
                    reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
            }
        }
    }
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
                    SetSpellFollowerEvent(self, 3);
                    self->flags |= 4u;
                }
            }
            else if (g_SecondaryPlayerControllerInstance->position.y > 128.0f)
            {
                SetSpellFollowerEvent(self, 2);
                self->flags &= ~4u;
            }
        }

        const CameraVector &target = g_SecondaryMaster->Slot(0)->state.world.position;
        CameraVector delta = {
            static_cast<float>(target.x - self->position.x),
            static_cast<float>(target.y - self->position.y),
            static_cast<float>(target.z - self->position.z)
        };
        CameraVector scaled = {
            static_cast<float>(delta.x * 0.05),
            static_cast<float>(delta.y * 0.05),
            static_cast<float>(delta.z * 0.05)
        };
        volatile float &positionX = self->position.x;
        positionX = static_cast<float>(positionX + scaled.x);
        volatile float &positionY = self->position.y;
        positionY = static_cast<float>(positionY + scaled.y);
        self->position.z = static_cast<float>(self->position.z + scaled.z);

        PositionSecondaryAnimation(AnimationVmId(self->positionVmId), &self->position);
        if ((self->flags & 0x20u) &&
            !reinterpret_cast<StageAuxiliaryView *>(g_MeshEffectEnemy)->effectActive)
            self->flags &= ~0x20u;
    }
    return 1;
}
