#include "../src/SecondaryPlayerController.hpp"

extern "C" void __stdcall SecondaryPauseEntryProbe(int gameOver)
{
    if (gameOver)
        EnterSecondaryGameOverMenu();
    else
        EnterSecondaryContinueMenu();
}
