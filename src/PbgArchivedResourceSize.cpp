#include "Archive.hpp"
#include <string.h>

// Native 0x441890..0x4418C2 (51 bytes): look up one resource within the
// selected mounted Pbg archive and return its unpacked byte length.
// The original takes the archive receiver and name in private EAX/EBX.
unsigned int __stdcall GetPbgArchivedResourceSize(ArchiveStateView *archive,
    const char *name)
{
    ArchiveEntryView *entry = archive->entries;
    if (entry)
    {
        for (int remaining = archive->entryCount; remaining > 0;
             --remaining, ++entry)
        {
            if (_stricmp(name, entry->name) == 0)
                return entry->size;
        }
    }
    return 0;
}
