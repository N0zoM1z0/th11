#include "SecondaryDebugHudLifetime.hpp"
#include "AnimationVmFactory.hpp"
#include "SecondaryPlayerDeath.hpp"

// The actual HUD initializer remains an ordinary C++ entry. This
// independent dynamic file/script/layer call keeps CreateAnimationVmId
// generic under VC9 whole-program optimization, avoiding specialization
// of its real three-parameter interface to HUD constants.
extern "C" unsigned int __stdcall SecondaryDebugHudAnimationInitProbe(
    AnimationFileView *file, int script, int layer,
    SecondaryActorHudView *hud, int lives, int fragments)
{
    InitializeSecondaryDebugHudAnimations();
    // Native UpdateSecondaryLivesDisplay keeps its EDI/EDX/stack interface
    // because it has several real callers; a dynamic caller prevents /GL
    // from folding the HUD initializer's global arguments into the callee.
    UpdateSecondaryLivesDisplay(hud, lives, fragments);
    return CreateAnimationVmId(file, script, layer).value;
}
