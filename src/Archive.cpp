#include "Archive.hpp"
#include "ByteCipher.hpp"

#include <stdlib.h>
#include <string.h>

unsigned char *__stdcall ReadArchivedFile(ArchiveStateView *self, const char *name,
                                          unsigned char *destination)
{
    unsigned char *packed = 0;
    if (!self->stream)
        return 0;

    ArchiveEntryView *entry = FindArchiveEntry(self, name);
    if (entry)
    {
        int packedSize = (entry + 1)->offset - entry->offset;
        unsigned int unpackedSize = entry->size;
        if (packedSize == unpackedSize)
            packed = destination;
        if (!packed)
            packed = static_cast<unsigned char *>(malloc(packedSize));
        if (packed && self->stream->Seek(entry->offset, 0) &&
            self->stream->Read(packed, packedSize))
        {
            const char *entryName = entry->name;
            unsigned int remaining = strlen(entryName);
            const char *cursor = entryName;
            unsigned char checksum = 0;
            while (remaining != 0)
            {
                checksum += *cursor;
                --remaining;
                ++cursor;
            }
            unsigned char ruleIndex = checksum % 8;
            ScatterXorBlocks(packed, packedSize, g_ArchiveCipherRules[ruleIndex].key,
                             g_ArchiveCipherRules[ruleIndex].step,
                             g_ArchiveCipherRules[ruleIndex].blockSize,
                             g_ArchiveCipherRules[ruleIndex].limit);

            unsigned char *result;
            if (packedSize != unpackedSize)
                result = DecompressArchiveData(unpackedSize, packed, packedSize, destination);
            else
                result = packed;
            if (packed != destination && packed)
                free(packed);
            return result;
        }
    }

    ArchiveTrace("info : %s error\r\n", self->filename);
    if (packed)
        free(packed);
    return 0;
}
