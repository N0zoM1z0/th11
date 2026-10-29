// Independent object input; this diagnostic image is never executed.
#include "SecondaryDamage.hpp"
extern "C" void __stdcall SecondaryDamageSoundProbe(int sound, float x)
{
    QueueSecondaryActorSound(sound, x);
}
