#include "SecondaryActorState.hpp"
#include "SecondaryActor.hpp"
#include "StageScript.hpp"

// Native 00411120..00411188: master actor scheduler. Keep the next pointer
// before actor state update or virtual deletion, since either can unlink it.
int __stdcall UpdateSecondaryMasterActors(SecondaryMasterView *master)
{
    SecondaryActorNode *node = master->first;
    while (node)
    {
        SecondaryActorNode *next = node->next;
        if (!(node->actor->state.flags & 0x200000) && !UpdateSecondaryActorState(&node->actor->state))
            node->actor->state.flags &= ~0x4000u;
        else if (node->actor)
            delete node->actor;
        node = next;
    }
    StageTimerView *timer = reinterpret_cast<StageTimerView *>(
        reinterpret_cast<unsigned char *>(master) + 0x50);
    AdvanceStageTimer(timer);
    return 1;
}
