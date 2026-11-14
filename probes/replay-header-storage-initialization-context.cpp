#include "../src/ReplayHeaderStorageInitialization.hpp"

// Non-runnable link context: owners clear a flag and reset their embedded
// settings block at +0x24, as the native callers do.
struct ReplayHeaderStorageOwner
{
    unsigned char unknown000[0x20];
    unsigned int flags;
    ReplayHeaderStorageView storage;
};
extern "C" void __stdcall ReplayHeaderStorageInitializationProbe(ReplayHeaderStorageOwner *owner)
{
    owner->flags &= ~1u;
    InitializeReplayHeaderStorage(&owner->storage);
    owner->flags |= 2;
}
