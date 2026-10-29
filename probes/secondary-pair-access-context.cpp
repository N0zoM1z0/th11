// Independent object input; this diagnostic image is never executed.
#include "SecondaryPairAccess.hpp"
extern "C" float __stdcall SecondaryPairAccessProbe(SecondaryPairTween *self,
    SecondaryVector2 *output, const SecondaryVector2 *input, float scale, int mode)
{
    SetSecondaryPairMode(self, mode);
    SetSecondaryPairInitial(self, input);
    SetSecondaryPairTarget(self, input);
    ResetSecondaryPairControl1(self);
    ResetSecondaryPairControl2(self);
    SetSecondaryPairDuration(self, mode);
    ResetSecondaryPairTimer(self);
    int duration = GetSecondaryPairDuration(self);
    StopSecondaryPairTween(self);
    SubtractSecondaryPair(output, input, &self->target);
    ScaleSecondaryPair(output, input, scale);
    AddSecondaryPair(output, input, &self->initial);
    return output->x + output->y + duration;
}
