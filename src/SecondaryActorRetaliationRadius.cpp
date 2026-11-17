#include "SecondaryActorState.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryPattern.hpp"
#include "AngleMath.hpp"

// Same original descriptor (+0x14 is the 0x004A3A64 angle cell) and player
// singleton used by the scene-projectile radius owner at 0x40B210.
extern SecondaryProjectileParameters g_SecondarySceneRetaliationProjectile;
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;

// Full native 0x412C50..0x412E10. The actor-radius postpass is not the
// existing ClearSecondaryActorsInRadius at 0x412990: here a visible actor
// also spawns a retaliation projectile before the optional drop.
void __stdcall ClearSecondaryActorsForSceneRadius(
    const CameraVector *center, float radius, int effects)
{
    for (SecondaryActorNode *node = g_SecondaryMaster->first; node; )
    {
        SecondaryActorManagerView *actor = node->actor;
        SecondaryActorNode *next = node->next;

        if (actor->state.flags & 0x800)
        {
            const CameraVector &point = actor->state.world.position;
            const float dx = static_cast<float>(point.x - center->x);
            const float dy = static_cast<float>(point.y - center->y);
            const float expanded = static_cast<float>(radius + 16.0);
            const float distanceSquared = static_cast<float>(dx * dx + dy * dy);

            // Unlike the scene projectile pool's greater-than rejection,
            // the native actor test accepts only ordered less/equal.
            if (distanceSquared <= expanded * expanded)
            {
                SecondaryCounterView &counter = actor->state.counter;
                if (counter.flags & 1)
                {
                    counter.scaled = static_cast<int>(
                        static_cast<unsigned int>(counter.scaled) - 99999u);
                    const int baseline7 = static_cast<int>(
                        static_cast<unsigned int>(counter.baseline) * 7u);
                    const int delta = static_cast<int>(
                        static_cast<unsigned int>(counter.scaled) -
                        static_cast<unsigned int>(baseline7));
                    counter.value = static_cast<int>(
                        static_cast<unsigned int>(delta / 7) +
                        static_cast<unsigned int>(counter.baseline));
                }
                else
                {
                    counter.value = static_cast<int>(
                        static_cast<unsigned int>(counter.value) - 99999u);
                }

                // This native owner performs its own inline double-boundary
                // viewport test with half-extent 2; it does not call 419920.
                if (!(static_cast<double>(point.x) + 2.0 <= -192.0 ||
                    static_cast<double>(point.x) - 2.0 >= 192.0 ||
                    static_cast<double>(point.y) + 2.0 <= 0.0 ||
                    static_cast<double>(point.y) - 2.0 >= 448.0))
                {
                    const float angle = static_cast<float>(
                        actor->state.world.angle + 3.1415927410125732421875);
                    g_SecondarySceneRetaliationProjectile.angle =
                        NormalizeAngle(angle);
                    SpawnSecondaryProjectile(
                        reinterpret_cast<SecondaryDamageReferencesView *>(
                            g_SecondaryPlayerControllerInstance),
                        &g_SecondarySceneRetaliationProjectile, &point, 0);
                    if (effects)
                        SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &point,
                            8, 0xffffffffu, -1.5707963705062866f,
                            0.6000000238418579f);
                }
            }
        }
        node = next;
    }
}
