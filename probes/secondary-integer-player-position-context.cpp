#include "../src/SecondaryPlayerController.hpp"

// Non-runnable link context: the stage owner passes a pointer into its
// current stage record.
extern "C" void __stdcall SecondaryIntegerPlayerPositionProbe(unsigned char **stages, int stage)
{
    SetSecondaryIntegerPlayerPosition(reinterpret_cast<const int *>(stages[stage] + 0x20));
    stages[stage][0] = 1;
}
