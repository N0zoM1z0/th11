// Non-runnable stream reads with independent owners, names and destinations.
#include "../src/Archive.hpp"

extern "C" unsigned char *__stdcall ArchiveReadProbe(
    ArchiveStateView *a, const char *nameA, unsigned char *outA,
    ArchiveStateView *b, const char *nameB, unsigned char *outB)
{
    unsigned char *first = ReadArchivedFile(a, nameA, outA);
    unsigned char *second = ReadArchivedFile(b, nameB, outB);
    return first ? first : second;
}
