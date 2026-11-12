#include "../src/SecondaryActorState.hpp"
extern SecondaryMasterView *__stdcall CreateSecondaryMaster(const char *filename);
extern void __cdecl DeleteSecondaryMaster();
extern "C" SecondaryMasterView *__stdcall SecondaryMasterLifecycleProbe(const char *filename,
    int destroy)
{
    if (destroy)
    {
        DeleteSecondaryMaster();
        return 0;
    }
    return CreateSecondaryMaster(filename);
}
