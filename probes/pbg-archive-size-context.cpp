#include "Archive.hpp"
unsigned int __stdcall GetPbgArchivedResourceSize(ArchiveStateView *archive,
    const char *name);
// A genuine dynamic archive/filename call to retain the original 16-byte
// entry table layout under a bounded VC9 /GL producer.
extern "C" unsigned int __stdcall PbgArchivedSizeProbe(
    ArchiveStateView *archive, const char *filename, int operation)
{
    if (operation) return GetPbgArchivedResourceSize(archive,filename);
    ArchiveEntryView *entry = FindArchiveEntry(archive, filename);
    return entry ? entry->size : 0;
}
