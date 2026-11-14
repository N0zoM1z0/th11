#include "SecondaryGameplay.hpp"
#include "SharedFile.hpp"
#include "ErrorLog.hpp"
#include "ReplayPayloadChecksum.hpp"
#include "ByteCipher.hpp"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

unsigned char *__stdcall CompressReplayData(const void *data, int size, int *packedSize);

namespace
{
struct StatisticsFileHeader
{
    unsigned int magic;
    int fileSize;
    unsigned char unknown008[8];
    int packedSize, dataSize;
};
struct StatisticsBlock
{
    unsigned short tag;
    unsigned char unknown002[2];
    unsigned int checksum;
    unsigned char unknown008[4];
    unsigned int index;
    unsigned char unknown010[0x68d4 - 0x10];
};
struct StatisticsTrailer
{
    unsigned char unknown000[4];
    unsigned int checksum;
    unsigned char bytes[0x448 - 8];
};
struct StatisticsView
{
    StatisticsFileHeader *header;
    unsigned char unknown004[4];
    StatisticsBlock blocks[7];
    StatisticsTrailer trailer;
};
typedef char StatisticsTrailerOffset[(offsetof(StatisticsView, trailer) == 0x2ddd4) ? 1 : -1];

inline void WriteStatisticsSection(const void *buffer, unsigned int size)
{
    if (g_SharedFile == INVALID_HANDLE_VALUE)
        return;
    DWORD written;
    WriteFile(g_SharedFile, buffer, size, &written, 0);
    if (size != written)
    {
        CloseHandle(g_SharedFile);
        UnlockSharedFile();
    }
}
}

// 00437A70. Serialize the statistics header, the used record blocks and the
// trailer, compress and scramble the payload, then write scoreth11.dat.
int __cdecl SaveSecondaryGameplayStatisticsFile()
{
    StatisticsView *statistics = reinterpret_cast<StatisticsView *>(g_SecondaryGameplayStatistics);
    if (!statistics->header)
        return -1;
    unsigned char *buffer = static_cast<unsigned char *>(malloc(0x200000));
    *reinterpret_cast<StatisticsFileHeader *>(buffer) = *statistics->header;
    int position = sizeof(StatisticsFileHeader);
    for (unsigned int i = 0; i < 7; ++i)
    {
        StatisticsBlock *block = &statistics->blocks[i];
        if (block->tag == 0x5243)
        {
            block->index = i;
            block->checksum = reinterpret_cast<ReplayPayloadChecksumView *>(block)->SumPayloadBytes(0x68d4);
            memcpy(buffer + position, block, 0x68d4);
            position += 0x68d4;
        }
    }
    unsigned int sum = 0;
    for (int i = 0; i < 0x440; ++i)
        sum += statistics->trailer.bytes[i];
    statistics->trailer.checksum = sum;
    *reinterpret_cast<StatisticsTrailer *>(buffer + position) = statistics->trailer;
    statistics->header->dataSize = position + 0x430;
    unsigned char *packed = CompressReplayData(buffer + sizeof(StatisticsFileHeader),
        statistics->header->dataSize, &statistics->header->packedSize);
    statistics->header->fileSize = statistics->header->packedSize + sizeof(StatisticsFileHeader);
    GatherXorBlocks(packed, statistics->header->packedSize, 0xac, 0x35, 0x10, statistics->header->packedSize);
    if (OpenSharedFileForWrite("scoreth11.dat"))
    {
        AppendErrorLog(&g_RuntimeLog, "\x65\x72\x72\x6f\x72\x20\x3a\x20\x83\x58\x83\x52\x83\x41\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xaa\x8f\x91\x82\xab\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\x0a");
        if (packed) free(packed);
        free(buffer);
        return -1;
    }
    WriteStatisticsSection(statistics->header, sizeof(StatisticsFileHeader));
    WriteStatisticsSection(packed, statistics->header->packedSize);
    CloseSharedFile();
    if (packed) free(packed);
    free(buffer);
    return 0;
}
