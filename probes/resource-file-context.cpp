// Non-runnable calls retain unknown paths, output cells, modes and lock owners.
#include "../src/ResourceFile.hpp"
#include "../src/RuntimeCritical.hpp"

extern "C" unsigned char *__stdcall ResourceFileProbe(const char *a, unsigned int *sizeA, int modeA,
                                                      const char *b, unsigned int *sizeB, int modeB)
{
    unsigned char *first = LoadResourceFile(a, sizeA, modeA);
    unsigned char *second = LoadResourceFile(b, sizeB, modeB);
    return first ? first : second;
}

extern "C" void __stdcall RuntimeCriticalProbe(MainRuntimeView *self, int index)
{
    LeaveRuntimeCriticalSection(self, index);
}
