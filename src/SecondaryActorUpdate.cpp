#include "SecondaryActorRuntime.hpp"
#include "MeshEffects.hpp"
#include "AngleMath.hpp"
#include <math.h>
#pragma intrinsic(sin, fabs)

int __stdcall UpdateSecondaryActorState(SecondaryActorStateView *self)
{
    if (self->flags & 0x4000) return 0;
    self->flags |= 0x4000;
    self->snapshot = self->world;
    SecondaryVector2 pair;
    if (self->pairTweens[0].duration)
    {
        StepSecondaryPairTween(&pair, &self->pairTweens[0]);
        if (!(self->local.flags & 1)) self->local.angle = NormalizeAngle(pair.x);
        self->local.speed = pair.y;
    }
    if (self->pairTweens[2].duration)
    {
        StepSecondaryPairTween(&pair, &self->pairTweens[2]);
        self->local.radius = pair.x;
        self->local.radialVelocity = pair.y;
    }
    if (self->pairTweens[1].duration)
    {
        StepSecondaryPairTween(&pair, &self->pairTweens[1]);
        if (!(self->offset.flags & 1)) self->offset.angle = NormalizeAngle(pair.x);
        self->offset.speed = pair.y;
    }
    if (self->pairTweens[3].duration)
    {
        StepSecondaryPairTween(&pair, &self->pairTweens[3]);
        self->offset.radius = pair.x;
        self->offset.radialVelocity = pair.y;
    }
    if (self->localTween.duration)
    {
        CameraVector value = StepStageVectorTween(&self->localTween);
        CameraVector velocity = { value.x - self->local.position.x,
            value.y - self->local.position.y, value.z - self->local.position.z };
        self->local.velocity = velocity;
    }
    else AdvanceSecondaryMotion(&self->local);
    if (self->offsetTween.duration)
    {
        CameraVector value = StepStageVectorTween(&self->offsetTween);
        CameraVector velocity = { value.x - self->offset.position.x,
            value.y - self->offset.position.y, value.z - self->offset.position.z };
        self->offset.velocity = velocity;
    }
    else AdvanceSecondaryMotion(&self->offset);
    ComposeSecondaryMotion(&self->local);
    if (self->flags & 0x400000)
    {
        self->offset.position.x += g_SecondaryActorSceneOffset.x;
        self->offset.position.y += g_SecondaryActorSceneOffset.y;
        self->offset.position.z = g_SecondaryActorSceneOffset.z + self->offset.position.z;
    }
    ComposeSecondaryMotion(&self->offset);
    UpdateSecondaryActorPosition(self);
    double halfWidth = self->hitWidth * 0.5;
    if (self->world.position.x + halfWidth < -192.0 || self->world.position.x - halfWidth > 192.0)
    {
        if ((self->flags & 0x1000) && !(self->flags & 4)) return -1;
    }
    else
    {
        double halfHeight = self->hitHeight * 0.5;
        if (self->world.position.y + halfHeight < 0.0 || self->world.position.y - halfHeight > 448.0)
        {
            if ((self->flags & 0x1000) && !(self->flags & 8)) return -1;
        }
        else self->flags |= 0x1000;
    }
    if (self->flags & 0x1000000)
    {
        if (SecondaryActorEnemy()->active)
        {
            if (!(self->flags & 0x2000000))
            {
                self->scriptBase = self->script1584;
                ChangeSecondaryAnimationScript(&self->animationIds[0], self->script1584);
                self->flags |= 0x2000001;
            }
        }
        else if (self->flags & 0x2000000)
        {
            self->scriptBase = self->script1588;
            ChangeSecondaryAnimationScript(&self->animationIds[0], self->script1588);
            self->flags &= ~0x2000001u;
        }
    }
    if (UpdateSecondaryScripts(self->owner, *self->frame.scale)) return -1;
    if (self->hook1630 && self->hook1630(self)) return -1;
    if ((self->flags & 0x80000) && !(self->flags & 0x20))
    {
        SecondaryActorSpellView *spell = SecondaryActorSpell();
        if ((spell->flags & 1) && spell->phase >= 0xa5 && spell->phase <= 0xae &&
            SecondaryActorEnemy()->active && !(g_SecondaryActorReferences->flags & 4))
        {
            if (!ValidateSecondaryAnimationId(&self->animationIds[9]))
                self->animationIds[9] = CreateAnimationVmId(SecondaryActorFile(0x48), 0x15, 9).value;
            PositionSecondaryAnimation(self->animationIds[9], &self->world.position);
            self->extentX = self->extentY = 192.0f;
        }
        else if (ValidateSecondaryAnimationId(&self->animationIds[9]))
        {
            SignalSecondaryAnimation(&self->animationIds[9]);
            self->animationIds[9] = 0;
            self->extentX = self->extentY = 48.0f;
        }
    }
    self->flags &= ~0x20000u;
    if (!(self->flags & 0x21))
    {
        int damage = self->hook1634 ? self->hook1634(self) :
            SampleSecondaryActorDamage(&self->world.position, &self->extentX);
        int mode = g_SecondaryActorReferences->damageMode;
        if (mode == 2 || mode == 0) damage /= 5;
        unsigned int active = SecondaryActorSpell()->flags & 1;
        if (active && SecondaryActorSpell()->phase >= 0x9e && SecondaryActorSpell()->phase <= 0xa1 &&
            (SecondaryActorEnemy()->active || SecondaryActorReferenceStatus()) &&
            !(g_SecondaryActorReferences->flags & 4)) damage /= 5;
        if (!(active && SecondaryActorSpell()->phase >= 0xa5 && SecondaryActorSpell()->phase <= 0xae &&
              SecondaryActorEnemy()->active && !(g_SecondaryActorReferences->flags & 4)) && damage)
        {
            if (SecondaryGlobalStatusBits()) damage /= 7;
            if (!(self->flags & 0x10) && self->timer1558.current <= 0)
                self->counter.Subtract(damage);
            const char *name = SelectSecondaryActorTransition(self->owner);
            if (name)
            {
                SetStageTimer(&self->frame, 0);
                ReleaseSecondaryAuxiliaryThreads(self->owner);
                ResetSecondaryMainThread(self->owner);
                SecondaryActorManagerView *owner = self->owner;
                owner->current->instruction = FindSecondarySubroutine(owner->program, name);
                owner->current->time = 0.0f;
                if (UpdateSecondaryScripts(self->owner, *self->frame.scale)) return -1;
            }
            if (self->counter.value <= 0 && !(self->flags & 0x80))
            {
                AddSecondaryActorScore(self->unknown14ec);
                if (FinishSecondaryActorEffect(self->owner)) return 1;
            }
            self->flags |= 0x20000;
        }
    }
    const char *name = SelectSecondaryActorTransition(self->owner);
    if (name)
    {
        SetStageTimer(&self->frame, 0);
        ReleaseSecondaryAuxiliaryThreads(self->owner);
        ResetSecondaryMainThread(self->owner);
        SecondaryActorManagerView *owner = self->owner;
        owner->current->instruction = FindSecondarySubroutine(owner->program, name);
        owner->current->time = 0.0f;
    }
    if (!(self->flags & 0x22) && self->timer156c.current <= 0 && !(self->flags & 0x400000))
    {
        if (self->hook1638) self->hook1638(self);
        else
        {
            int result = CheckSecondaryActorCollision(&self->world.position,
                static_cast<float>(self->extentZ * 0.5));
            if ((self->flags & 0x200) && result == 2 && self->frame.current % 3 == 0)
            {
                IncrementSecondaryCollisionCount();
                CreatePositionedAnimationVmId(g_SecondaryActorFiles->file,
                    &g_SecondaryActorReferences->position, 0x9c, 0x16);
                AddSecondaryCollisionValue(g_SecondaryActorCollisionValues[g_SecondaryActorDifficulty]);
                QueueSecondaryActorSound(0x1c, g_SecondaryActorReferences->position.x);
            }
        }
    }
    if (self->flags & 0x10000)
    {
        int direction = self->world.velocity.x < static_cast<double>(-0.1f) ? -1 :
            self->world.velocity.x > static_cast<double>(0.1f) ? 1 : 0;
        if (self->direction != direction)
        {
            int script = 0;
            if (self->direction == -1) script = direction ? 2 : 3;
            else if (self->direction == 0) script = direction == -1 ? 1 : 2;
            else if (self->direction == 1) script = direction ? 1 : 4;
            AnimationVmView *vm = ClearMissingAnimationId(&self->animationIds[0]);
            AnimationFileView *file = vm->file;
            self->direction = direction;
            ReleaseSecondaryAnimation(self->animationIds[0]);
            self->animationIds[0] = 0;
            self->animationIds[0] = CreateAnimationVmId(file,
                static_cast<int>(static_cast<unsigned int>(self->scriptBase) + script), 7).value;
        }
    }
    if (!(self->flags & 0x400000))
    {
        for (int i = 0; i < 8; ++i)
        {
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(self->animationIds[i]));
            if (!vm) self->animationIds[i] = 0;
            else
            {
                PositionSecondaryAnimationVm(vm, &self->world.position);
                if (vm->flags.value & 0x8000000)
                {
                    reinterpret_cast<SecondaryActorAnimationView *>(vm)->angle = self->world.angle;
                    vm->flags.value |= 4;
                }
            }
        }
    }
    else
        for (int i = 0; i < 8; ++i)
            PositionSecondaryAnimationRaw(self->animationIds[i], &self->world.position);
    if (!(self->flags & 0x21) && !(self->flags & 0xc00000))
    {
        SecondaryActorReferencesView *references = g_SecondaryActorReferences;
        if (!references->tracked ||
            fabs(static_cast<float>(references->tracked->state.world.position.x - references->position.x)) <
                fabs(static_cast<float>(self->world.position.x - references->position.x)))
        {
            TrackSecondaryActor(self->owner);
            references->trackingLocked = 1;
        }
    }
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(self->animationIds[0]));
    if (!vm) self->animationIds[0] = 0;
    else if (self->flashFrames)
    {
        vm->flags.value &= ~0x8000u;
        if (self->flags & 0x80000) g_SecondaryActorHud->flags &= ~0x8000u;
        self->flashFrames = static_cast<int>(static_cast<unsigned int>(self->flashFrames) - 1);
    }
    else
    {
        SecondaryActorAnimationView *animation = reinterpret_cast<SecondaryActorAnimationView *>(vm);
        if (self->flags & 0x8000000)
        {
            if (self->frame.current % 4 == 0)
            {
                vm->flags.value |= 0x8000;
                animation->color = 0xffff00ff;
            }
            else vm->flags.value &= ~0x8000u;
        }
        if (self->flags & 0x20000)
        {
            vm->flags.value |= 0x8000;
            animation->color = 0xff0000ff;
            self->flashFrames = 4;
            SecondaryActorSpellView *spell = SecondaryActorSpell();
            bool low = false;
            if ((self->flags & 0x4080000) && (!(spell->flags & 1) || !(spell->flags & 8)))
                low = self->owner->state.counter.unknown008 < ((spell->flags & 1) ? 200 : 900);
            if (low) QueueSecondaryActorSound(0x23, self->world.position.x);
            else QueueSecondaryActorSound(0x13, self->world.position.x);
        }
        else if (self->frame.current % 4)
            vm->flags.value &= ~0x8000u;
        else if (self->flags & 0x4080000)
        {
            SecondaryActorSpellView *spell = SecondaryActorSpell();
            if ((!(spell->flags & 1) || !(spell->flags & 8)) &&
                self->owner->state.counter.unknown008 < ((spell->flags & 1) ? 100 : 500))
            {
                vm->flags.value |= 0x8000;
                animation->color = 0xff0000ff;
            }
        }
    }
    if (self->mesh)
    {
        float radius = self->meshProgress;
        float phaseX = self->meshPhaseX, phaseY = self->meshPhaseY;
        CameraVector *point = self->mesh->positions;
        float amplitude = SecondaryActorSpell()->phase == 0xad ? 1.0f : 8.0f;
        if (self->meshLimit > self->meshProgress)
            self->meshProgress = static_cast<float>(self->meshProgress + 2.0);
        unsigned char alpha = 255;
        if (self->meshProgress < 32.0f) alpha = 0;
        else if (self->meshProgress < 64.0f)
            alpha = static_cast<unsigned char>(static_cast<int>(
                (self->meshProgress - 32.0) * 255.0 * 0.03125));
        float extent = static_cast<float>(radius * 2.0 + 40.0);
        ResetStageMesh(self->mesh, static_cast<float>((self->world.position.x - radius) - 20.0),
            static_cast<float>((self->world.position.y - radius) - 20.0), extent, extent);
        CameraVector center = { static_cast<float>(self->world.position.x + 32.0 + 192.0),
            static_cast<float>(self->world.position.y + 16.0), self->world.position.z };
        RenderVertex *vertex = self->mesh->vertices;
        for (int row = 0; row < self->mesh->rows; ++row)
        {
            double squaredRadius = static_cast<double>(radius) * radius;
            for (int column = 0; column < self->mesh->columns; ++column, ++point, ++vertex)
            {
                CameraVector delta = { point->x - center.x, point->y - center.y, point->z - center.z };
                float squaredDistance = delta.x * delta.x + delta.y * delta.y;
                float remaining = static_cast<float>(squaredRadius - squaredDistance);
                unsigned char *color = reinterpret_cast<unsigned char *>(&vertex->color);
                if (remaining >= 0.0f)
                {
                    float factor = static_cast<float>(remaining / squaredRadius);
                    vertex->color = self->meshColor;
                    color[2] = static_cast<unsigned char>(static_cast<int>(255.0 - factor * (255 - color[2])));
                    color[1] = static_cast<unsigned char>(static_cast<int>(255.0 - (255 - color[1]) * factor));
                    color[0] = static_cast<unsigned char>(static_cast<int>(255.0 - (255 - color[0]) * factor));
                    float magnitude;
                    if (SecondaryActorSpell()->phase == 0xad)
                    {
                        color[3] = static_cast<unsigned char>(static_cast<int>(alpha * factor));
                        magnitude = static_cast<float>(factor * 4.0);
                    }
                    else
                    {
                        color[3] = 255;
                        magnitude = static_cast<float>(factor * 32.0);
                    }
                    D3DXVec3Normalize(&delta, &delta);
                    delta.x = magnitude * delta.x;
                    delta.y = magnitude * delta.y;
                    delta.z = magnitude * delta.z;
                    float wave = static_cast<float>(sin(static_cast<double>(phaseX)));
                    delta.x = wave * factor * amplitude + delta.x;
                    wave = static_cast<float>(sin(static_cast<double>(phaseY)));
                    delta.y = wave * factor * amplitude + delta.y;
                    vertex->x += delta.x;
                    vertex->y += delta.y;
                    vertex->z = 0.0f;
                    point->z = 0.0f;
                }
                else color[3] = 0;
                phaseX = NormalizeAngle(static_cast<float>(phaseX + 0.09817477315664291));
                phaseY = NormalizeAngle(static_cast<float>(phaseY - 0.04908738657832146));
            }
        }
        self->meshPhaseX = NormalizeAngle(static_cast<float>(self->meshPhaseX + 0.19634954631328583));
        self->meshPhaseY = NormalizeAngle(static_cast<float>(self->meshPhaseY + 0.09817477315664291));
    }
    if (self->timer1558.current > 0) OffsetSecondaryTimer(&self->timer1558, -1.0f);
    if (self->timer156c.current > 0) OffsetSecondaryTimer(&self->timer156c, -1.0f);
    int previous = self->frame.current;
    self->frame.previous = previous;
    if (*self->frame.scale > static_cast<double>(0.99f) && *self->frame.scale < 1.01f)
    {
        self->frame.current = static_cast<int>(static_cast<unsigned int>(previous) + 1);
        self->frame.fraction = static_cast<float>(self->frame.fraction + 1.0);
    }
    else
    {
        self->frame.fraction = *self->frame.scale + self->frame.fraction;
        self->frame.current = static_cast<int>(self->frame.fraction);
    }
    return 0;
}
