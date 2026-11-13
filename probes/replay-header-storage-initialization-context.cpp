#include "../src/ReplayHeaderStorageInitialization.hpp"

extern "C" void __stdcall ReplayHeaderStorageInitializationProbe(void *header)
{
    unsigned char *embedded = static_cast<unsigned char *>(header) + 0x18;
    __asm {
        mov esi, embedded
        call InitializeReplayHeaderStorage
    }
}
