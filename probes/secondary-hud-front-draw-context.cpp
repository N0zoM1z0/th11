#include "SecondaryHud.hpp"
#include "AnimationImmediateDraw.hpp"

// The ordinary callback retains its actual HUD context. An independent
// generic renderer/VM call keeps the drawing helper unspecialized in
// the VC9 whole-program producer.
int __fastcall SecondaryHudFrontDrawCallback(SecondaryHudView *self);
extern "C" int __stdcall SecondaryHudFrontDrawProbe(
    SecondaryHudView *owner, RenderBatchView *renderer, AnimationVmView *vm)
{
    const int result = SecondaryHudFrontDrawCallback(owner);
    return result + DrawAnimationVmByMode(renderer, vm);
}
