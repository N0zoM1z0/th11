#include "MenuDifficultySelection.hpp"
#include "MenuPrimary.hpp"

void __stdcall ExecuteMenuAnimationEvent3(MenuControllerView *self, int script)
{
    SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, script), 3);
}
