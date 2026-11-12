#include "../src/SecondaryMasterInitialization.hpp"
extern "C" int __stdcall SecondaryMasterInitializationProbe(SecondaryMasterView *self,
    const char *scriptFilename)
{
    return InitializeSecondaryHost(self, scriptFilename);
}
