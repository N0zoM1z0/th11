#include "SecondaryActorInstructionAnimation.hpp"

SecondaryActorManagerView *__stdcall CreateSecondaryHostActor(const char *script,
    const SecondaryHostSpawnArguments *arguments)
{
    SecondaryMasterView *master = g_SecondaryMaster;
    SecondaryActorManagerView *actor = new SecondaryActorManagerView(script);
    actor->state.local.position = arguments->position;
    actor->state.unknown14ec = arguments->value00c;
    actor->state.counter.value = arguments->value014;
    actor->state.counter.unknown004 = arguments->value014;
    actor->state.effect.id = arguments->value010;
    actor->state.flags = (actor->state.flags & ~0x8000u) |
        ((static_cast<unsigned int>(arguments->mirrored) << 15) & 0x8000u);
    actor->main.difficulty = static_cast<unsigned char>(1u << g_SecondaryActorDifficulty);
    *reinterpret_cast<SecondaryHostSpawnParameters *>(actor->state.unknown124) = arguments->parameters;
    StageTimerView &timer = actor->state.timer1558;
    if (!(timer.flags & 1))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.current = 2;
    timer.fraction = 2.0f;
    timer.previous = 1;
    actor->state.unknown11c = 0;
    actor->state.flags = (actor->state.flags & ~0x400000u) | ((arguments->flags << 22) & 0x400000u);
    if (arguments->value014 >= 1000) actor->state.flags |= 0x4000000;
    UpdateSecondaryActorState(&actor->state);
    if (actor->state.flags & 0x80000)
    {
        int id = actor->state.effect.id;
        if (id == 1) id = 10;
        else if (id == 4) id = 11;
        actor->state.effect.id = id;
    }
    int *render = reinterpret_cast<int *>(actor->state.unknown1544);
    render[0] = (master->unknown074 & 1) + 2;
    render[1] = 0x51;
    SecondaryHostAnimationSelection &selection = SecondaryHostInstructions::Selection(&actor->state);
    if (selection.savedFile == 1)
    {
        switch (selection.savedScript)
        {
        case 0:
        case 0x31:
            render[1] = 0x51;
            break;
        case 5:
        case 0x32:
            render[1] = 0x4e;
            break;
        case 15:
        case 0x34:
            render[1] = 0x57;
            break;
        case 10:
        case 0x33:
            render[1] = 0x54;
            break;
        }
    }
    render[2] = 0;
    SecondaryActorNode *node = &actor->state.node;
    if (!master->first)
    {
        master->count = SecondaryHostInstructions::AddInteger(master->count, 1);
        ++master->unknown074;
        master->first = node;
        master->last = node;
        return actor;
    }
    SecondaryActorNode *last = master->last;
    if (last->next)
    {
        node->next = last->next;
        last->next->previous = node;
    }
    last->next = node;
    node->previous = last;
    master->count = SecondaryHostInstructions::AddInteger(master->count, 1);
    ++master->unknown074;
    master->last = node;
    return actor;
}
