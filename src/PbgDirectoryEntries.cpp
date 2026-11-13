#include "PbgDirectoryEntries.hpp"
#include <new>
#include <stdlib.h>
#include <string.h>

// Full native 0x441A60..0x441BCA, 363 bytes. Natural new[] must construct
// count+1 physical records with a four-byte vector cookie, using the real
// 9-byte entry constructor and 26-byte entry destructor for EH cleanup.
// The original retains an allocated but null name when malloc fails, and
// continues decoding that directory record without inventing early returns.
PbgResourceEntryView *__stdcall ParsePbgDirectoryEntries(
    const unsigned char *data, unsigned int count, unsigned int tailOffset)
{
    PbgResourceEntryView *entries = new PbgResourceEntryView[count + 1];
    if (!entries) return 0;

    int remaining = static_cast<int>(count);
    PbgResourceEntryView *entry = entries;
    if (remaining > 0)
    {
        do
        {
            const unsigned char *cursor = data;
            unsigned char ch;
            do { ch = *cursor; ++cursor; } while (ch);
            unsigned int nameBytes = static_cast<unsigned int>(cursor - data);
            char *copy = static_cast<char *>(malloc(nameBytes));
            if (copy)
            {
                const unsigned char *read = data;
                unsigned char c;
                do { c = *read; copy[read - data] = static_cast<char>(c); ++read; } while (c);
            }
            entry->name = copy;
            const unsigned char *recheck = data;
            do { ch = *recheck; ++recheck; } while (ch);
            int alignedNameBytes = static_cast<int>(recheck - data);
            int remainder = alignedNameBytes % 4;
            if (remainder != 0) alignedNameBytes += 4 - remainder;
            data += alignedNameBytes;
            entry->unknown004 = *reinterpret_cast<const unsigned int *>(data);
            entry->unknown008 = *reinterpret_cast<const unsigned int *>(data + 4);
            entry->unknown00c = *reinterpret_cast<const unsigned int *>(data + 8);
            data += 12;
            ++entry;
        } while (--remaining);
    }
    entries[count].unknown004 = tailOffset;
    entries[count].unknown008 = 0;
    return entries;
}
