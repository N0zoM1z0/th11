#include "SecondaryPlayerDeath.hpp"

extern "C" int __stdcall SecondaryPlayerDeathProbe(SecondaryDamageReferencesView *self,
    SecondaryActorHudView *hud, int lives, int fragments, AnimationFileView *file,
    int script, AnimationVmView *vm)
{
    int result = UpdateSecondaryPlayerCallback(self);
    UpdateSecondaryLivesDisplay(hud, lives, fragments);
    BindAnimationScript(file, script, vm);
    return result;
}
