// Non-runnable independent complete-file writes with unknown inputs.
#include "../src/WholeFile.hpp"

extern "C" int __stdcall WholeFileProbe(const char *a, const void *dataA, unsigned int sizeA,
                                       const char *b, const void *dataB, unsigned int sizeB)
{
    int first = WriteWholeFile(a, dataA, sizeA);
    return first + WriteWholeFile(b, dataB, sizeB);
}
