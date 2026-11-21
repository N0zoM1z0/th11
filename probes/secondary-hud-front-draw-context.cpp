#include "SecondaryHud.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"

typedef int (__fastcall *SecondaryHudFrontDrawEntry)(SecondaryHudView *);
int __fastcall SecondaryHudFrontDrawCallback(SecondaryHudView *owner);

// Keep the native callback's public ECX argument boundary observable to
// VC9 /GL. A volatile indirect call prevents specializing its EBX handoff
// to the probe's own register allocation.
extern "C" int __stdcall SecondaryHudFrontDrawProbe(
    SecondaryHudView *owner, RenderBatchView *renderer, AnimationVmView *vm)
{
    volatile SecondaryHudFrontDrawEntry callback =
        SecondaryHudFrontDrawCallback;
    const int result = callback(owner);
    return result + vm->DrawByMode(renderer);
}
