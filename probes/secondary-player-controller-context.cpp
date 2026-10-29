#include "SecondaryPlayerController.hpp"

extern "C" float __stdcall SecondaryPlayerControllerProbe(SecondaryDamageReferencesView *self,
    const CameraVector *position, ChainElement *update, ChainElement *draw)
{
    update->callback = reinterpret_cast<ChainCallback>(UpdateSecondaryPlayerCallback);
    update->unknown0c = 0;
    update->auxiliaryCallback = 0;
    update->context = self;
    update->flags = (update->flags & ~2u) | 1;
    draw->callback = reinterpret_cast<ChainCallback>(DrawSecondaryPlayerCallback);
    draw->unknown0c = 0;
    draw->auxiliaryCallback = 0;
    draw->context = self;
    draw->flags = (draw->flags & ~2u) | 1;
    int updated = UpdateSecondaryPlayerCallback(self);
    int drawn = DrawSecondaryPlayerCallback(self);
    RefreshSecondaryPlayerEmitters(self);
    float forward = SecondaryPlayerAimAngle(self, position);
    float inverse = AngleToSecondaryPlayer(position);
    return updated + drawn + forward + inverse;
}
