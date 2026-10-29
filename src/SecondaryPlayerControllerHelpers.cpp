#include "SecondaryPlayerController.hpp"
#include <math.h>

void __stdcall RefreshSecondaryPlayerEmitters(SecondaryDamageReferencesView *self)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    for (int i = 0; i < 8; ++i) owner->emitters[i].refresh = 1;
}

float __stdcall SecondaryPlayerAimAngle(SecondaryDamageReferencesView *self, const CameraVector *position)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(self);
    float x = position->x - owner->position.x;
    float y = position->y - owner->position.y;
    if (y == 0.0f && x == 0.0f) return 1.5707964f;
    return static_cast<float>(atan2(y, x));
}

float __stdcall AngleToSecondaryPlayer(const CameraVector *position)
{
    SecondaryPatternOwnerView *owner = reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryActorReferences);
    float x = owner->position.x - position->x;
    float y = owner->position.y - position->y;
    if (y == 0.0f && x == 0.0f) return 1.5707964f;
    return static_cast<float>(atan2(y, x));
}
