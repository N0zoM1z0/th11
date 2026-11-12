#include "SecondaryHostMiscLeaves.hpp"
#include "SecondaryActorState.hpp"
#include "Chain.hpp"

// Complete 0x410E60..0x410E82, shared pause helper for the master's two
// independently registered chain nodes. No null master guard in the target.
void __cdecl PauseSecondaryMasterChains()
{
    SecondaryMasterView *master = g_SecondaryMaster;
    if (*reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(master) + 8))
        (*reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(master) + 8))->flags |= 2;
    if (*reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(master) + 12))
        (*reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(master) + 12))->flags |= 2;
}
