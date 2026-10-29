#include "SecondaryPattern.hpp"
#include "SecondaryEmitterAnimation.hpp"
#include "AngleMath.hpp"

namespace
{
__forceinline int Selection()
{
    return static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3 + g_MeshEffectSelection1);
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

__forceinline void ReleaseEmitterVm(unsigned int id)
{
    AnimationVmView *vm = EmitterVm(g_RenderBatch, id);
    if (!vm) return;
    vm->flags.value |= 0x04000000;
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
            node->vm->flags |= 0x04000000;
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

__forceinline unsigned int PointIndex(unsigned int table, int count, int index)
{
    return static_cast<unsigned int>(g_SecondaryEmitterOffsetIndices[table + count - 1]) + index;
}

__forceinline const SecondaryEmitterFloatPair *ConfigPoint(SecondaryPatternOwnerView *owner,
    unsigned int table, int count, int index, unsigned int offset)
{
    return reinterpret_cast<const SecondaryEmitterFloatPair *>(
        reinterpret_cast<unsigned char *>(owner->configuration) + offset + PointIndex(table, count, index) * 8);
}

__forceinline void ReadFixedPoint(SecondaryIntegerPair *output, SecondaryPatternOwnerView *owner,
    unsigned int table, int count, int index, unsigned int offset)
{
    const SecondaryEmitterFloatPair *point = ConfigPoint(owner, table, count, index, offset);
    output->x = static_cast<int>(point->x * 128.0);
    output->y = static_cast<int>(point->y * 128.0);
}

__forceinline void PositionEmitter(SecondaryEmitterObjectView *emitter, SecondaryPatternOwnerView *owner)
{
    SecondaryIntegerPair *offset = owner->focused ? &emitter->offsets[1] : &emitter->offsets[0];
    int x = static_cast<int>(static_cast<unsigned int>(owner->x) + offset->x);
    int y = static_cast<int>(static_cast<unsigned int>(owner->y) + offset->y);
    emitter->pendingX = x;
    emitter->pendingY = y;
    emitter->x = x;
    emitter->y = y;
}
}

void __stdcall ResetSecondaryPlayerEmitters(SecondaryDamageReferencesView *self)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    unsigned int table = static_cast<unsigned int>(Selection()) * 16;
    int count = g_SecondaryPatternPower / g_SecondaryPatternPowerUnit;
    if (g_SecondaryPatternPower >= g_SecondaryPatternMaximumPower)
    {
        for (int i = 0; i < count; ++i)
        {
            SecondaryEmitterObjectView *emitter = &owner->emitters[i];
            ReleaseEmitterVm(emitter->idB4);
            emitter->idB4 = 0;
            switch (Selection())
            {
            case 0: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x17, 0xb).value; break;
            case 1: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x18, 0xb).value; break;
            case 2: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x19, 0xb).value; break;
            case 3: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x28, 0xb).value; break;
            case 4: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x29, 0xb).value; break;
            case 5: emitter->idB4 = CreateSecondaryEmitterAnimationVmId(self->file, 0x2a, 0xb).value; break;
            }
        }
    }
    else
    {
        RenderBatchView *renderer = g_RenderBatch;
        for (int i = 0; i < 8; ++i) SignalEmitterVm(renderer, owner->emitters[i].idB4);
    }
    if (!self->patternSlot) owner->angle8c14 = NormalizeAngle(0.0f);
    if (self->patternSlot == count) return;
    int i = 0;
    for (; i < count; ++i)
    {
        SecondaryEmitterObjectView *emitter = &owner->emitters[i];
        emitter->x = owner->x;
        emitter->y = owner->y;
        ReleaseEmitterVm(emitter->idB0);
        emitter->idB0 = 0;
        emitter->index = i;
        switch (Selection())
        {
        case 0:
            emitter->onDraw = SecondaryEmitterOrbitCallback;
            emitter->angle = NormalizeAngle(static_cast<float>(i * static_cast<double>(6.2831855f) / count -
                static_cast<double>(1.5707964f)));
            ReadFixedPoint(&emitter->offsets[0], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[1], owner, table, count, i, 0x78);
            PositionEmitter(emitter, owner);
            emitter->idB0 = CreateAnimationVmId(self->file, 0x14, 0xb).value;
            break;
        case 1:
            ReadFixedPoint(&emitter->offsets[0], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[1], owner, table, count, i, 0x78);
            PositionEmitter(emitter, owner);
            emitter->idB0 = CreateAnimationVmId(self->file, 0x15, 0xb).value;
            break;
        case 2:
            emitter->onDraw = SecondaryEmitterAimCallback;
            emitter->polar[0] = *ConfigPoint(owner, table, count, i, 0x28);
            emitter->polar[1] = *ConfigPoint(owner, table, count, i, 0x78);
            PositionEmitter(emitter, owner);
            emitter->idB0 = CreateAnimationVmId(self->file, 0x16, 0xb).value;
            break;
        case 3:
            ReadFixedPoint(&emitter->offsets[0], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[1], owner, table, count, i, 0x148);
            PositionEmitter(emitter, owner);
            emitter->idB0 = CreateAnimationVmId(self->file,
                static_cast<int>(static_cast<unsigned int>(g_SecondaryEmitterScripts[
                    static_cast<unsigned int>(count) * 8 + i]) + 0x15), 0xb).value;
            break;
        case 4:
            ReadFixedPoint(&emitter->offsets[0], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[1], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[2], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[3], owner, table, count, i, 0x78);
            ReadFixedPoint(&emitter->offsets[4], owner, table, count, i, 0xc8);
            ReadFixedPoint(&emitter->offsets[5], owner, table, count, i, 0x118);
            ReadFixedPoint(&emitter->offsets[6], owner, table, count, i, 0x168);
            {
                SecondaryIntegerPair *offset = owner->focused ? &emitter->offsets[1] : &emitter->offsets[0];
                emitter->pendingX = static_cast<int>(static_cast<unsigned int>(owner->x) + offset->x);
                emitter->pendingY = static_cast<int>(static_cast<unsigned int>(owner->y) + offset->y);
            }
            emitter->idB0 = CreateAnimationVmId(self->file,
                static_cast<int>(static_cast<unsigned int>(owner->multiplier) + 0x22), 0xb).value;
            break;
        case 5:
            ReadFixedPoint(&emitter->offsets[0], owner, table, count, i, 0x28);
            ReadFixedPoint(&emitter->offsets[1], owner, table, count, i, 0x78);
            PositionEmitter(emitter, owner);
            emitter->idB0 = CreateAnimationVmId(self->file, 0x27, 0xb).value;
            break;
        }
        emitter->state = 2;
    }
    for (; i < 8; ++i)
    {
        SecondaryEmitterObjectView *emitter = &owner->emitters[i];
        emitter->state = 0;
        SignalEmitterVm(g_RenderBatch, emitter->idB0);
    }
    self->patternSlot = count;
    for (i = 0; i < 8; ++i) owner->emitters[i].refresh = 1;
}

void __cdecl ResetSecondaryPatternSlot()
{
    SecondaryDamageReferencesView *self = reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryActorReferences);
    self->patternSlot = 0;
    ResetSecondaryPlayerEmitters(self);
}
