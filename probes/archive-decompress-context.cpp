// Non-runnable calls keep allocation size independent of packed length.
#include "../src/Archive.hpp"

extern "C" unsigned char *__stdcall ArchiveDecompressProbe(
    unsigned char *a, int lengthA, unsigned int sizeA, unsigned char *outA,
    unsigned char *b, int lengthB, unsigned int sizeB, unsigned char *outB)
{
    unsigned char *first = DecompressArchiveData(sizeA, a, lengthA, outA);
    unsigned char *second = DecompressArchiveData(sizeB, b, lengthB, outB);
    return first ? first : second;
}
