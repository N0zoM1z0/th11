#include "SecondaryGameplay.hpp"
#include "StageDrawing.hpp"

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

void __stdcall UpdateSecondaryGameplayGauges(SecondaryGameplaySettingsView *self)
{
    using SecondaryHostInstructions::AddInteger;
    if (64.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->value014 < 10000)
        {
            self->value014 = AddInteger(self->value014, 5000);
            if (self->value014 >= 10000) self->value014 = 10000;
        }
        else self->value014 = AddInteger(self->value014, 16);
    }
    else if (80.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->value014 < 10000)
        {
            self->value014 = AddInteger(self->value014, 5000);
            if (self->value014 >= 10000) self->value014 = 10000;
        }
        else self->value014 = AddInteger(self->value014, 12);
    }
    else if (128.0f > g_SecondaryActorReferences->position.y)
    {
        if (self->value014 < 10000)
        {
            self->value014 = AddInteger(self->value014, 5000);
            if (self->value014 >= 10000) self->value014 = 10000;
        }
        else self->value014 = AddInteger(self->value014, 9);
    }
    else
    {
        self->value014 = AddInteger(self->value014, -30);
        if (self->value014 < 0) self->value014 = 0;
    }
    if (self->time.current <= 0)
    {
        self->value018 = AddInteger(self->value018, -30);
        if (self->value018 < 0) self->value018 = 0;
    }
    AddStageTimer(&self->time, -1.0f);
    if (self->value014 >= 13000) self->value014 = 13000;
    if (self->value018 >= self->value014) self->value018 = self->value014;
}
