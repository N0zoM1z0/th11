#include "PbgDirectoryLoader.hpp"
#include "PbgLowLevelFile.hpp"
#include "PbgResourceEntry.hpp"
#include "ByteCipher.hpp"
#include "Archive.hpp"
#include <stdlib.h>

extern const char g_PbgFileReadMode[];

// Native 0x441A60 builds a heap-owned 16-byte entry array from the decoded
// data, with an EH vector constructor and per-entry heap path duplication.
PbgResourceEntryView *__stdcall ParsePbgDirectoryEntries(
    const unsigned char *data, unsigned int count, unsigned int tailOffset);

namespace
{
__forceinline unsigned char CallStreamOpen(PbgLowLevelFileView *file,
    const char *path, const char *mode)
{
    typedef unsigned char (__thiscall *Open)(PbgLowLevelFileView *,
        const char *, const char *);
    return reinterpret_cast<Open>(file->vtable[0])(file,path,mode);
}
__forceinline unsigned int CallStreamRead(PbgLowLevelFileView *file,
    void *data, unsigned int size)
{
    typedef unsigned int (__thiscall *Read)(PbgLowLevelFileView *,void *,unsigned int);
    return reinterpret_cast<Read>(file->vtable[2])(file,data,size);
}
__forceinline unsigned int CallStreamSize(PbgLowLevelFileView *file)
{
    typedef unsigned int (__thiscall *Size)(PbgLowLevelFileView *);
    return reinterpret_cast<Size>(file->vtable[5])(file);
}
__forceinline unsigned char CallStreamSeek(PbgLowLevelFileView *file,
    int offset, unsigned int origin)
{
    typedef unsigned char (__thiscall *Seek)(PbgLowLevelFileView *,int,unsigned int);
    return reinterpret_cast<Seek>(file->vtable[6])(file,offset,origin);
}
__forceinline void DisposeFailedStream(PbgResourceSessionView *session)
{
    PbgLowLevelFileView *file = reinterpret_cast<PbgLowLevelFileView *>(session->stream);
    if (file)
    {
        typedef void (__thiscall *DeletingDestructor)(PbgLowLevelFileView *,unsigned int);
        reinterpret_cast<DeletingDestructor>(file->vtable[7])(file,1);
    }
    session->stream = 0;
}
}

// Complete original 0x441910..0x441A5F, 336-byte session-directory loader.
// The stream at +0xC is opened through its virtual method. After unpacking
// the THA1 header, the directory is read from the end of the physical file,
// decoded and parsed into the session's own heap 16-byte directory entries.
// All failure branches close and clear the stream; no defensive checks or
// compensating frees are added where the original has none.
unsigned char __fastcall LoadPbgSessionDirectory(PbgResourceSessionView *session,
    const char *filename)
{
    PbgDirectoryHeaderView header = {0,0,0,0};
    PbgLowLevelFileView *stream =
        reinterpret_cast<PbgLowLevelFileView *>(session->stream);
    if (!stream) return 0;

    if (!CallStreamOpen(stream,filename,g_PbgFileReadMode))
        goto failure;
    if (!CallStreamRead(stream,&header,sizeof(header)))
        goto failure;

    ScatterXorBlocks(reinterpret_cast<unsigned char *>(&header),16,0x1B,
                     0x37,16,16);
    if (header.signature != 0x31414854)
        goto failure;

    header.directoryOffset -= 0x075BCD15;
    header.compressedLength -= 0x3ADE68B1;
    session->count = header.entryCount + 0xF7E7F8AC;

    {
        const unsigned int endOffset = CallStreamSize(stream);
        const unsigned int directoryStart = endOffset - header.compressedLength;
        CallStreamSeek(stream, directoryStart, 0);
        unsigned char *packed = static_cast<unsigned char *>(malloc(header.compressedLength));
        if (!packed) goto failure;

        unsigned char *decoded = 0;
        if (CallStreamRead(stream,packed,header.compressedLength))
        {
            ScatterXorBlocks(packed,header.compressedLength,0x3E,
                             0x9B,0x80,header.compressedLength);
            decoded = DecompressArchiveData(header.directoryOffset, packed,
                                             header.compressedLength,0);
            if (decoded)
            {
                PbgResourceEntryView *entries = ParsePbgDirectoryEntries(
                    decoded,session->count,directoryStart);
                session->entries = reinterpret_cast<unsigned int>(entries);
                if (entries)
                {
                    free(packed);
                    free(decoded);
                    return 1;
                }
            }
        }
        free(packed);
        if (decoded) free(decoded);
    }

failure:
    DisposeFailedStream(session);
    return 0;
}
