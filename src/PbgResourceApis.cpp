#include "PbgResFileRuntime.hpp"
#include "PbgLowLevelFile.hpp"
#include "Archive.hpp"
#include <string.h>

extern const unsigned int g_PbgFileVtable[];
extern const char g_PbgFileReadMode[];
unsigned int __stdcall GetPbgArchivedResourceSize(ArchiveStateView *archive,
    const char *name);

namespace
{
__forceinline const char *ArchiveResourceBasename(const char *filename)
{
    const char *slash = strchr(filename, '/');
    return slash ? slash + 1 : filename;
}

__forceinline void InitializeLocalPbgStream(PbgLowLevelFileView *stream)
{
    stream->vtable = g_PbgFileVtable;
    stream->handle = INVALID_HANDLE_VALUE;
    stream->access = 0;
}
}

// Complete 0x4421E0..0x4422B9, 218-byte native resource-data API.
// Mounts are checked first; the standalone disk branch creates a real local
// Pbg::File stream and relies on its full C++ destructor on all exit paths.
void *__fastcall ReadPbgResourceByName(const char *filename)
{
    PbgResResourceRecordView *mounted = FindPbgResourceRecord(filename);
    if (mounted)
    {
        return ReadArchivedFile(reinterpret_cast<ArchiveStateView *>(mounted),
                                ArchiveResourceBasename(filename), 0);
    }

    PbgLowLevelFileView file;
    InitializeLocalPbgStream(&file);
    file.Open(filename, g_PbgFileReadMode);
    unsigned int size = file.Size();
    return file.ReadWhole(size);
}

// Complete 0x4422C0..0x442389, 202-byte companion size lookup.
// The indexed archive branch reports the unpacked entry size, whereas the
// disk path opens/closes its own short-lived local Pbg::File stream.
unsigned int __fastcall GetPbgResourceSizeByName(const char *filename)
{
    PbgResResourceRecordView *mounted = FindPbgResourceRecord(filename);
    if (mounted)
    {
        return GetPbgArchivedResourceSize(
            reinterpret_cast<ArchiveStateView *>(mounted),
            ArchiveResourceBasename(filename));
    }

    PbgLowLevelFileView file;
    InitializeLocalPbgStream(&file);
    file.Open(filename, g_PbgFileReadMode);
    return file.Size();
}
