#include "Archive.hpp"
#include <string.h>

ArchiveEntryView *__stdcall FindArchiveEntry(ArchiveStateView *self, const char *name)
{
    ArchiveEntryView *entry = self->entries;
    if (!entry)
        return 0;
    for (int remaining = self->entryCount; remaining > 0; --remaining, ++entry)
        if (_stricmp(name, entry->name) == 0)
            return entry;
    return 0;
}
