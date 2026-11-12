#include "SharedFile.hpp"
#include "FileSystem.hpp"
#include "ResourceFile.hpp"
#include "ByteCipher.hpp"
#include "Archive.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ReplayInitStateView;
extern unsigned int g_ReplayStatusMask;

// Two independently bounded shared-file helpers: the first reads an EDI-sized
// allocation, and the second closes/unlocks the current read stream.
unsigned char *__stdcall ReadSharedReplayBytes(unsigned int size); // 0x458980
int __cdecl CloseSharedReplayReader();                         // 0x4589D0

namespace
{
struct ReplayLoadFileHeader
{
    unsigned int magic;
    unsigned short version;
    unsigned char unknown006[0x1c - 6];
    unsigned int packedSize;
    unsigned int rawSize;
};
typedef char ReplayLoadFileHeaderSize[(sizeof(ReplayLoadFileHeader) == 0x24) ? 1 : -1];

template<typename T> __forceinline T &ReplayAt(void *base, unsigned int offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<unsigned char *>(base) + offset);
}
}

// 0x436B60..0x436D18, the full native 441-byte replay stage loader.
// The disk and archive branches genuinely use distinct ownership: the
// disk-packed buffer is freed after unpacking; the archive buffer is not.
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source)
{
    const char *filename = static_cast<const char *>(source);
    char *targetName = &ReplayAt<char>(state, 0x1dc);
    const char *read = filename;
    do
    {
        *targetName++ = *read;
    } while (*read++ != 0);

    unsigned char *packed;
    if (!(g_ReplayStatusMask & 0x20))
    {
        char path[256];
        sprintf(path, "replay/%s", filename);
        if (!FileExists(path) || OpenSharedFileForRead(path) != 0)
            return -1;
        ReplayLoadFileHeader *file =
            reinterpret_cast<ReplayLoadFileHeader *>(ReadSharedReplayBytes(0x24));
        ReplayAt<ReplayLoadFileHeader *>(state, 0x14) = file;
        if (file->magic != 0x72313174 || file->version != 4)
        {
            CloseSharedReplayReader();
            return -1;
        }
        packed = ReadSharedReplayBytes(file->packedSize);
        CloseSharedReplayReader();
    }
    else
    {
        unsigned char *resource = LoadResourceFile(filename, 0, 0);
        ReplayAt<ReplayLoadFileHeader *>(state, 0x14) =
            reinterpret_cast<ReplayLoadFileHeader *>(resource);
        packed = resource + 0x24;
    }

    ReplayLoadFileHeader *file = ReplayAt<ReplayLoadFileHeader *>(state, 0x14);
    unsigned char *unpacked = static_cast<unsigned char *>(malloc(file->rawSize));
    ReplayAt<unsigned char *>(state, 0x1c4) = unpacked;
    ScatterXorBlocks(packed, file->packedSize, 0xaa, 0xe1, 0x800, file->packedSize);
    ScatterXorBlocks(packed, file->packedSize, 0x3d, 0x7a, 0x40, file->packedSize);
    DecompressArchiveData(file->rawSize, packed, file->packedSize, unpacked);
    ReplayAt<unsigned char *>(state, 0x18) = unpacked;

    unsigned char *cursor = unpacked + 0x70;
    int count = ReplayAt<int>(unpacked, 0x58);
    if (count >= 8) count = 6;
    for (int index = 0; index < count; ++index)
    {
        short stage = ReplayAt<short>(cursor, 0);
        unsigned char *events = cursor + 0x90;
        ReplayAt<unsigned char *>(state, static_cast<unsigned int>(stage + 5) * 0x24) = cursor;
        ReplayAt<unsigned char *>(state, 0xa4 + static_cast<unsigned int>(stage) * 0x24) = events;
        ReplayAt<unsigned char *>(state, 0xac + static_cast<unsigned int>(stage) * 0x24) =
            events + ReplayAt<int>(cursor, 4) * 6;
        cursor += 0x90 + ReplayAt<int>(cursor, 8);
    }
    if (!(g_ReplayStatusMask & 0x20) && packed)
        free(packed);
    return 0;
}
