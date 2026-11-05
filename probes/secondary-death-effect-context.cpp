#include "SecondaryDeathEffect.hpp"

extern "C" SecondaryDeathEffect *__stdcall SecondaryDeathEffectProbe(
    SecondaryDeathEffectManager *manager, const CameraVector *position)
{
    ApplySecondaryPlayerDeathPenalty(position);
    return AllocateSecondaryDeathEffect(manager, position);
}
