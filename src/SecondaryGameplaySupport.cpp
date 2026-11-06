#include "SecondaryGameplay.hpp"

void __stdcall PauseSecondaryGameplayObject(void *object)
{
    SecondaryGameplayChainsView *self = reinterpret_cast<SecondaryGameplayChainsView *>(object);
    if (self->update) self->update->flags |= 2;
    if (self->draw) self->draw->flags |= 2;
}

void __stdcall SetSecondaryGameplayRuntimeMode(MainRuntimeView *runtime, int mode)
{
    int &value = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(runtime) + 0x558);
    if (!(runtime->flags & 0x2000)) value = mode;
    else value = 2;
}

void __stdcall UpdateSecondaryGameplayGauges(SecondaryHostSelectionState *self)
{
    using SecondaryHostInstructions::AddInteger;
    if (64.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->gaugeValue014 < 10000)
        {
            self->gaugeValue014 = AddInteger(self->gaugeValue014, 5000);
            if (self->gaugeValue014 >= 10000) self->gaugeValue014 = 10000;
        }
        else self->gaugeValue014 = AddInteger(self->gaugeValue014, 16);
    }
    else if (80.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->gaugeValue014 < 10000)
        {
            self->gaugeValue014 = AddInteger(self->gaugeValue014, 5000);
            if (self->gaugeValue014 >= 10000) self->gaugeValue014 = 10000;
        }
        else self->gaugeValue014 = AddInteger(self->gaugeValue014, 12);
    }
    else if (128.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->gaugeValue014 < 10000)
        {
            self->gaugeValue014 = AddInteger(self->gaugeValue014, 5000);
            if (self->gaugeValue014 >= 10000) self->gaugeValue014 = 10000;
        }
        else self->gaugeValue014 = AddInteger(self->gaugeValue014, 9);
    }
    else
    {
        self->gaugeValue014 = AddInteger(self->gaugeValue014, -30);
        if (self->gaugeValue014 < 0) self->gaugeValue014 = 0;
    }
    if (self->gaugeTime.current <= 0)
    {
        self->gaugeValue018 = AddInteger(self->gaugeValue018, -30);
        if (self->gaugeValue018 < 0) self->gaugeValue018 = 0;
    }
    OffsetSecondaryTimer(&self->gaugeTime, -1.0f);
    if (self->gaugeValue014 >= 13000) self->gaugeValue014 = 13000;
    if (self->gaugeValue018 >= self->gaugeValue014) self->gaugeValue018 = self->gaugeValue014;
}
