#include "SecondaryGameplay.hpp"

// Original 0x40D500 is a separate 111-byte loader body; this thread entry
// does not contain that loader and returns integer zero on its one RET.
unsigned int __cdecl LoadSecondaryDebugSessionBody();

unsigned int __cdecl LoadSecondaryDebugScript()
{
    LoadSecondaryDebugSessionBody();
    return 0;
}
