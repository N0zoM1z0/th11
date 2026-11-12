#include "SecondaryMasterInitialization.hpp"
#include "SecondaryActorState.hpp"
#include <new>
#include <string.h>

void __stdcall DestroySecondaryMaster(SecondaryMasterView *self);

// Full native 0x411070..0x4110D0: create a 0x78-byte master, set its
// initial flags, publish the singleton, and run the real script/chain setup.
// The native unconditionally passes the possibly-null result to Initialize.
SecondaryMasterView *__stdcall CreateSecondaryMaster(const char *filename)
{
    SecondaryMasterView *self = static_cast<SecondaryMasterView *>(
        ::operator new(sizeof(SecondaryMasterView)));
    if (self)
    {
        unsigned int &stateFlags = *reinterpret_cast<unsigned int *>(
            reinterpret_cast<unsigned char *>(self) + 0x60);
        stateFlags &= ~1u;
        memset(self, 0, sizeof(SecondaryMasterView));
        *reinterpret_cast<unsigned int *>(self) |= 2u;
        g_SecondaryMaster = self;
    }
    if (InitializeSecondaryHost(self, filename) != 0)
    {
        if (self)
        {
            DestroySecondaryMaster(self);
            ::operator delete(self);
        }
        return 0;
    }
    return self;
}

// Full native 0x411100..0x41111C: teardown and deallocation of the
// separately owned singleton. The destructor itself clears the global.
void __cdecl DeleteSecondaryMaster()
{
    SecondaryMasterView *self = g_SecondaryMaster;
    if (self)
    {
        DestroySecondaryMaster(self);
        ::operator delete(self);
    }
}
