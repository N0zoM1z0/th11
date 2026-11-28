#include "StageTween.hpp"

// zunlib.cpp timer advance (0x459270). Native callers pass the timer in ESI;
// /GL assigns that private convention from this ordinary definition.
int __stdcall AdvanceStageTimer(StageTimerView *self)
{
    self->previous = self->current;
    if (*self->scale > static_cast<double>(0.99f) && *self->scale < 1.01f)
    {
        self->current++;
        self->fraction = static_cast<float>(self->fraction + 1.0);
    }
    else
    {
        self->fraction = *self->scale + self->fraction;
        self->current = static_cast<int>(self->fraction);
    }
    return self->current;
}

extern "C" int __stdcall StageTimerProbe(StageVectorTween *tween)
{
    return AdvanceStageTimer(&tween->time);
}
