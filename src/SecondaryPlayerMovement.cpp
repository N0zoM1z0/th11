#include "SecondaryPlayerMovement.hpp"
#include "InputBackend.hpp"
#include "AnimationVmFactory.hpp"
#include "AnimationSpriteBinding.hpp"

namespace
{
__forceinline int Add(int a, int b)
{ return static_cast<int>(static_cast<unsigned int>(a) + b); }
__forceinline int Subtract(int a, int b)
{ return static_cast<int>(static_cast<unsigned int>(a) - b); }
__forceinline int Negate(int a)
{ return static_cast<int>(0u - static_cast<unsigned int>(a)); }
__forceinline int Product(int a, int b)
{ return static_cast<int>(static_cast<unsigned int>(a) * b); }
__forceinline int Selection()
{ return Add(Product(g_MeshEffectSelection0, 3), g_MeshEffectSelection1); }
__forceinline SecondaryControllerHostView *Host()
{ return reinterpret_cast<SecondaryControllerHostView *>(g_SecondaryProgramHost); }
__forceinline SecondaryControllerHudView *Hud()
{ return reinterpret_cast<SecondaryControllerHudView *>(g_SecondaryActorHud); }
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
__forceinline void SignalEmitter(RenderBatchView *renderer, unsigned int id)
{
    AnimationVmView *vm = EmitterVm(renderer, id);
    if (!vm) return;
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
}
__forceinline void MoveScript(SecondaryDamageReferencesView *self, int script)
{
    SecondaryPlayerControllerView *owner = reinterpret_cast<SecondaryPlayerControllerView *>(self);
    BindAnimationScript(self->file, script, &owner->animation);
}
__forceinline void SelectOffset(SecondaryEmitterObjectView *emitter, int selection)
{
    emitter->offsets[1] = emitter->offsets[selection + 2];
    emitter->offsets[0] = emitter->offsets[1];
}
}

int __stdcall UpdateSecondaryPlayerMovement(SecondaryDamageReferencesView *self)
{
    SecondaryPlayerMovementView *owner = reinterpret_cast<SecondaryPlayerMovementView *>(self);
    int dx = 0, dy = 0;
    unsigned int input = g_InputState.current;
    if ((input & 0x50) == 0x50) owner->direction = 5;
    else if ((input & 0x60) == 0x60) owner->direction = 7;
    else if ((input & 0x90) == 0x90) owner->direction = 6;
    else if ((input & 0xa0) == 0xa0) owner->direction = 8;
    else if (input & 0x20) owner->direction = 2;
    else if (input & 0x10) owner->direction = 1;
    else if (input & 0x40) owner->direction = 3;
    else if (input & 0x80) owner->direction = 4;
    else owner->direction = 0;

    if (!Host() || !Host()->enabled || owner->auxiliaryTime.current < 4)
    {
        owner->focused = 0;
        owner->blend = 30;
    }
    else owner->focused = (g_InputState.current >> 3) & 1;

    if (owner->fireCount == 99) dx = -150;
    else if (owner->fireCount == 100) dx = 150;
    else
    {
        if (owner->focused)
        {
            if (!owner->focusedId)
                owner->focusedId = CreateAnimationVmId(g_SecondaryActorFiles->file, 0x4a, 0xb).value;
            switch (owner->direction)
            {
            case 1: dy = Negate(owner->focusedSpeed); break;
            case 2: dy = owner->focusedSpeed; break;
            case 3: dx = Negate(owner->focusedSpeed); break;
            case 4: dx = owner->focusedSpeed; break;
            case 5: dx = Negate(owner->focusedDiagonalSpeed); dy = dx; break;
            case 6: dx = owner->focusedDiagonalSpeed; dy = Negate(dx); break;
            case 7: dy = owner->focusedDiagonalSpeed; dx = Negate(dy); break;
            case 8: dx = owner->focusedDiagonalSpeed; dy = dx; break;
            }
        }
        else
        {
            if (FindAnimationVmById(g_RenderBatch, AnimationVmId(owner->focusedId)))
                SignalSecondaryAnimationEvent(AnimationVmId(owner->focusedId), 1);
            owner->focusedId = 0;
            switch (owner->direction)
            {
            case 1: dy = Negate(owner->normalSpeed); break;
            case 2: dy = owner->normalSpeed; break;
            case 3: dx = Negate(owner->normalSpeed); break;
            case 4: dx = owner->normalSpeed; break;
            case 5: dx = Negate(owner->normalDiagonalSpeed); dy = dx; break;
            case 6: dx = owner->normalDiagonalSpeed; dy = Negate(dx); break;
            case 7: dy = owner->normalDiagonalSpeed; dx = Negate(dy); break;
            case 8: dx = owner->normalDiagonalSpeed; dy = dx; break;
            }
        }
    }

    int script = -1;
    if (!(owner->flags & 2))
    {
        if (dx < 0 && owner->previousX >= 0) MoveScript(self, 1);
        if (dx > 0 && owner->previousX <= 0) script = 3;
        if (!dx && owner->previousX < 0) MoveScript(self, 2);
        if (!dx && owner->previousX > 0) script = 4;
    }
    else
    {
        if (dx < 0 && owner->previousX >= 0) MoveScript(self, 0x1b);
        if (dx > 0 && owner->previousX <= 0) script = 0x1d;
        if (!dx && owner->previousX < 0) MoveScript(self, 0x1c);
        if (!dx && owner->previousX > 0) script = 0x1e;
    }
    if (script >= 0) MoveScript(self, script);
    owner->previousX = dx;
    owner->previousY = dy;

    if (Selection() == 2)
    {
        if (!(g_InputState.current & 9))
        {
            owner->movingFrames = Add(owner->movingFrames, 1);
            if (owner->movingFrames > 10)
            {
                dx = Add(dx, dx);
                dy = Add(dy, dy);
                AnimationVmId id = CreateAnimationVmId(self->file, 0x13, 0xb);
                AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
                if (vm->file)
                    BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file),
                        owner->primarySprite, vm);
                PositionSecondaryAnimationVm(vm, &owner->position);
            }
        }
        else owner->movingFrames = 0;
    }
    owner->movement.x = dx * g_AnimationTimeScale;
    owner->movement.y = dy * g_AnimationTimeScale;
    if (owner->direction) owner->rememberedMovement = owner->movement;
    int stepX = static_cast<int>(owner->movement.x);
    owner->stepX = stepX;
    int stepY = static_cast<int>(owner->movement.y);
    owner->fixedPosition.x = Add(owner->fixedPosition.x, stepX);
    owner->fixedPosition.y = Add(owner->fixedPosition.y, stepY);
    owner->stepY = stepY;
    if (!Selection()) UpdateSecondaryPlayerHorizontalWrap(self);
    if (Selection() == 1)
    {
        if (!owner->direction && !(g_InputState.current & 9))
        {
            owner->stillFrames = Add(owner->stillFrames, 1);
            if (owner->stillFrames > 10) AttractSecondaryPlayerItems();
        }
        else owner->stillFrames = 0;
    }
    if (owner->fireCount < 99)
    {
        if (owner->fixedPosition.x < -0x5c00)
        {
            if (!owner->fireCount && !Selection()) { owner->fireCount = 1; owner->unknown924 = 0; }
            owner->fixedPosition.x = -0x5c00;
        }
        else if (owner->fixedPosition.x > 0x5c00)
        {
            if (!owner->fireCount && !Selection()) { owner->fireCount = 3; owner->unknown924 = 0; }
            owner->fixedPosition.x = 0x5c00;
        }
        if (owner->fixedPosition.y < 0x1000) owner->fixedPosition.y = 0x1000;
        else if (owner->fixedPosition.y > 0xd800) owner->fixedPosition.y = 0xd800;
    }
    owner->position.x = static_cast<float>(owner->fixedPosition.x * (1.0 / 128.0));
    owner->position.y = static_cast<float>(owner->fixedPosition.y * (1.0 / 128.0));
    if (!FindAnimationVmById(g_RenderBatch, AnimationVmId(owner->focusedId))) owner->focusedId = 0;
    else
    {
        CameraVector position = { static_cast<float>(owner->position.x + 32.0 + 192.0),
            static_cast<float>(owner->position.y + 16.0), owner->position.z };
        PositionSecondaryAnimationRaw(owner->focusedId, &position);
    }
    if (Selection() == 4 && !Hud()->blocked)
    {
        if (g_InputState.repeat & 0x400)
        {
            owner->multiplier = Add(owner->multiplier, 1) % 5;
            for (int i = 0; i < owner->patternSlot; ++i)
            {
                SecondaryEmitterObjectView *emitter = &owner->emitters[i];
                ReleaseSecondaryAnimation(emitter->idB0);
                emitter->idB0 = 0;
                emitter->idB0 = CreateAnimationVmId(self->file, Add(owner->multiplier, 0x22), 0xb).value;
            }
        }
        SelectOffset(&owner->emitters[0], owner->multiplier);
        SelectOffset(&owner->emitters[1], owner->multiplier);
        SelectOffset(&owner->emitters[2], owner->multiplier);
        SelectOffset(&owner->emitters[3], owner->multiplier);
        SelectOffset(&owner->emitters[4], owner->multiplier);
        SelectOffset(&owner->emitters[5], owner->multiplier);
        SelectOffset(&owner->emitters[6], owner->multiplier);
        SelectOffset(&owner->emitters[7], owner->multiplier);
    }
    if (owner->flags & 8) owner->collapseFrames = Add(owner->collapseFrames, 1);
    RenderBatchView *renderer = g_RenderBatch;
    for (int i = 0; i < 8; ++i)
    {
        SecondaryEmitterObjectView *emitter = &owner->emitters[i];
        if (!emitter->state) continue;
        if (!(owner->flags & 8))
        {
            SecondaryIntegerPair *offset = owner->focused ? &emitter->offsets[1] : &emitter->offsets[0];
            emitter->pending = owner->fixedPosition + *offset;
            if (emitter->onUpdate) emitter->onUpdate(emitter);
        }
        else
        {
            emitter->pending.x = owner->fixedPosition.x;
            emitter->pending.y = owner->fixedPosition.y;
            if (owner->collapseFrames >= 30)
            {
                emitter->state = 0;
                SignalEmitter(renderer, emitter->idB0);
                SignalEmitter(renderer, emitter->idB4);
                owner->patternSlot = 0;
                continue;
            }
        }
        if (emitter->refresh)
        {
            emitter->refresh = 0;
            emitter->position.x = emitter->pending.x;
            emitter->position.y = emitter->pending.y;
        }
        else if (owner->blend >= 30)
        {
            SecondaryIntegerPair delta = (emitter->pending - emitter->position) * owner->blend / 100;
            if (delta.x || delta.y) emitter->position += delta;
            else emitter->position = emitter->pending;
        }
        CameraVector position = { static_cast<float>(emitter->position.x * (1.0 / 128.0)),
            static_cast<float>(emitter->position.y * (1.0 / 128.0)), 0.0f };
        PositionSecondaryAnimation(emitter->idB0, &position);
        PositionSecondaryAnimation(emitter->idB4, &position);
        renderer = g_RenderBatch;
    }
    return 0;
}

void __stdcall SignalSecondaryAnimationEvent(AnimationVmId id, short event)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (!vm) return;
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
}
