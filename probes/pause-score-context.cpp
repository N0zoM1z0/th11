#include "../src/PauseContinueMenu.hpp"

extern "C" void __stdcall PauseScoreProbe()
{
    CommitPauseMenuScore();
}
