#include "SecondaryHudDraw.hpp"

int (__fastcall *g_SecondaryHudDrawProbeCallback)(SecondaryHudView *) = &SecondaryHudDrawCallback;

extern "C" int __stdcall SecondaryHudDrawProbe(SecondaryHudView *self)
{
    return g_SecondaryHudDrawProbeCallback(self);
}
