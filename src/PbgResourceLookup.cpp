#include "PbgResFileRuntime.hpp"
#include <windows.h>
#include <string.h>

// Original suffix physically starts at 0x496534: the five bytes ".dat\0".
extern const char g_PbgArchiveResourceSuffix[5];

// 0x4420E0..0x4421AA: the 203-byte resource-name normalization and lookup
// boss. It copies a bounded-stack-array name without adding truncation or
// guards that the historical original did not contain. The slash separator
// selects a .dat extension; a no-slash path is copied unchanged.
PbgResResourceRecordView *__fastcall FindPbgResourceRecord(const char *filename)
{
    char normalized[MAX_PATH];
    const char *slash = strchr(filename, '/');
    const char *path = slash ? slash + 1 : filename;
    int suffixOffset = static_cast<int>(path - filename) - 1;
    const char *read=filename;
    char ch;
    do { ch = *read; normalized[read - filename] = ch; ++read; } while (ch);
    if (suffixOffset >= 0)
    {
        unsigned int first = *reinterpret_cast<const unsigned int *>(g_PbgArchiveResourceSuffix);
        unsigned char last = g_PbgArchiveResourceSuffix[4];
        char *destination = normalized + suffixOffset;
        *reinterpret_cast<unsigned int *>(destination) = first;
        destination[4] = last;
    }
    PbgResResourceRecordView *record = g_PbgResFileEntries;
    int count = g_PbgResFileCount;
    for (int i = 0; i < count; ++i, ++record)
    {
        if (strcmp(record->filename, normalized) == 0)
            return record;
    }
    return 0;
}
