#include "SecondaryReplayState.hpp"
#include "FileSystem.hpp"
#include "SharedFile.hpp"
#include "ResourceFile.hpp"
#include "ByteCipher.hpp"
#include "Archive.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned int g_ReplayStatusMask;
unsigned char *__stdcall ReadSharedReplayBytes(unsigned int size);
int __cdecl CloseSharedReplayReader();

namespace
{
struct ReplayLoadFileHeader
{
    unsigned int magic;
    unsigned short version;
    unsigned char unknown006[0x1c - 6];
    int packedSize, rawSize;
};
struct ReplayLoadHeader
{
    unsigned char unknown000[0x58];
    int stageCount;
};
struct ReplayLoadStage
{
    short stage;
    unsigned char unknown002[2];
    int eventCount, bytes;
};
}

// 00436B60. Load a replay file (or the archived copy), decrypt and unpack it,
// then index each stage record and its event stream.
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source)
{
    unsigned int size;
    char path[256];
    unsigned char *data;
    strcpy(reinterpret_cast<char *>(state) + 0x1dc, static_cast<const char *>(source));
    if (!(g_ReplayStatusMask & 0x20))
    {
        sprintf(path, "replay/%s", static_cast<const char *>(source));
        if (!FileExists(path) || OpenSharedFileForRead(path))
            return -1;
        state->fileHeader = ReadSharedReplayBytes(0x24);
        ReplayLoadFileHeader *file = static_cast<ReplayLoadFileHeader *>(state->fileHeader);
        if (file->magic != 0x72313174)
        {
            CloseSharedReplayReader();
            return -1;
        }
        if (file->version != 4)
        {
            CloseSharedReplayReader();
            return -1;
        }
        data = ReadSharedReplayBytes(file->packedSize);
        CloseSharedReplayReader();
    }
    else
    {
        state->fileHeader = LoadResourceFile(static_cast<const char *>(source), &size, 0);
        data = static_cast<unsigned char *>(state->fileHeader) + 0x24;
    }
    state->unpackedData = static_cast<unsigned char *>(
        malloc(static_cast<ReplayLoadFileHeader *>(state->fileHeader)->rawSize));
    ScatterXorBlocks(data, static_cast<ReplayLoadFileHeader *>(state->fileHeader)->packedSize,
        0xaa, 0xe1, 0x800, static_cast<ReplayLoadFileHeader *>(state->fileHeader)->packedSize);
    ScatterXorBlocks(data, static_cast<ReplayLoadFileHeader *>(state->fileHeader)->packedSize,
        0x3d, 0x7a, 0x40, static_cast<ReplayLoadFileHeader *>(state->fileHeader)->packedSize);
    DecompressArchiveData(static_cast<ReplayLoadFileHeader *>(state->fileHeader)->rawSize, data,
        static_cast<ReplayLoadFileHeader *>(state->fileHeader)->packedSize, state->unpackedData);
    state->header = state->unpackedData;
    ReplayLoadStage *stage = reinterpret_cast<ReplayLoadStage *>(state->unpackedData + 0x70);
    for (int i = 0; i < (static_cast<ReplayLoadHeader *>(state->header)->stageCount < 8
            ? static_cast<ReplayLoadHeader *>(state->header)->stageCount : 6); ++i)
    {
        state->stageLinks[stage->stage].stageHeader = stage;
        state->stageLinks[stage->stage].eventsBegin = reinterpret_cast<unsigned char *>(stage) + 0x90;
        state->stageLinks[stage->stage].eventsEnd =
            state->stageLinks[stage->stage].eventsBegin + stage->eventCount * 6;
        stage = reinterpret_cast<ReplayLoadStage *>(reinterpret_cast<unsigned char *>(stage) + stage->bytes + 0x90);
    }
    if (!(g_ReplayStatusMask & 0x20) && data)
        free(data);
    return 0;
}
