#include "../src/SecondaryActorState.hpp"
extern int __stdcall InitializeSecondaryHost(SecondaryMasterView *self, const char *scriptFilename);
extern "C" int __stdcall SecondaryMasterInitializationProbe(SecondaryMasterView *self,
    const char *scriptFilename)
{
    return InitializeSecondaryHost(self, scriptFilename);
}
