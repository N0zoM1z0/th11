#include "SecondaryDebugHudLifetime.hpp"
#include "AnimationVmFactory.hpp"

// The actual HUD initializer remains an ordinary C++ entry. This
// independent dynamic file/script/layer call keeps CreateAnimationVmId
// generic under VC9 whole-program optimization, avoiding specialization
// of its real three-parameter interface to HUD constants.
extern "C" unsigned int __stdcall SecondaryDebugHudAnimationInitProbe(
    AnimationFileView *file, int script, int layer)
{
    InitializeSecondaryDebugHudAnimations();
    return CreateAnimationVmId(file, script, layer).value;
}
