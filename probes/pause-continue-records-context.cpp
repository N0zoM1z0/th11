#include "../src/PauseContinueMenu.hpp"
#include "../src/SecondaryGameplay.hpp"

// Non-runnable link context: the continue and game-over menus prepare the
// records for their pause view; menu input moves selections by live amounts.
extern "C" void __stdcall PauseContinueRecordsProbe(PauseMenuView *self)
{
    PreparePauseContinueRecords(self);
    MoveSecondaryDebugSelection(&self->selection, self->state);
}
