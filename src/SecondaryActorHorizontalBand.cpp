#include "SecondaryActorState.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorViewport.hpp"

// Original TH11 0x412B10..0x412C47. Unlike the circular-clear path,
// this one tests an open horizontal interval and a strict Y threshold.
void __stdcall ClearSecondaryActorsInHorizontalBand(
    const CameraVector *center, float halfWidth, int effects)
{
    SecondaryMasterView *master = g_SecondaryMaster;
    SecondaryActorNode *node = master->first;
    while (node)
    {
        SecondaryActorManagerView *actor = node->actor;
        SecondaryActorNode *next = node->next;
        SecondaryActorStateView &state = actor->state;
        if (state.flags & 0x800)
        {
            const CameraVector &point = state.world.position;
            if (point.x > center->x - halfWidth &&
                point.x < center->x + halfWidth &&
                point.y < center->y)
            {
                SecondaryCounterView &counter = state.counter;
                if (counter.flags & 1)
                {
                    counter.scaled = static_cast<int>(
                        static_cast<unsigned int>(counter.scaled) - 99999u);
                    const int base = static_cast<int>(
                        static_cast<unsigned int>(counter.baseline) * 7u);
                    const int scaledDelta = static_cast<int>(
                        static_cast<unsigned int>(counter.scaled) -
                        static_cast<unsigned int>(base));
                    counter.value = static_cast<int>(
                        static_cast<unsigned int>(scaledDelta / 7) +
                        static_cast<unsigned int>(counter.baseline));
                }
                else
                {
                    counter.value = static_cast<int>(
                        static_cast<unsigned int>(counter.value) - 99999u);
                }

                const SecondaryActorViewportPoint *viewportPoint =
                    reinterpret_cast<const SecondaryActorViewportPoint *>(&point);
                if (!viewportPoint->IsOutside(2.0f, 2.0f) && effects)
                    SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &point,
                        8, 0xffffffffu, -1.5707963705062866f,
                        0.6000000238418579f);
            }
        }
        node = next;
    }
}
