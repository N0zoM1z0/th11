#include "ReplayLeaderboardRuntime.hpp"
#include "ResourceFile.hpp"
#include "Archive.hpp"
#include "ByteCipher.hpp"
#include "ReplayPayloadChecksum.hpp"
#include <new>
#include <stdlib.h>
#include <string.h>

namespace
{
struct ReplayLeaderboardFileHeader
{
    unsigned int signature;
    unsigned int unknown004;
    unsigned short version;
    unsigned short unknown00a;
    unsigned int checksum;
    unsigned int packedSize, decodedSize;
};
typedef char ReplayLeaderboardFileHeaderSize[
    (sizeof(ReplayLeaderboardFileHeader) == 0x18) ? 1 : -1];

__forceinline void ResetReplayLeaderboardFileHeader(ReplayLeaderboardRuntimeView *state)
{
    ReplayLeaderboardFileHeader *header = static_cast<ReplayLeaderboardFileHeader *>(malloc(0x18));
    memset(header, 0, 0x18);
    header->signature = 0x31314854;
    header->version = 4;
    header->checksum = 0x100;
    state->sourceFile = reinterpret_cast<unsigned char *>(header);
}
}

// Native 0x437740..0x4377C9. The native implementation uses the quotient of
// offset/0x68D4 to select each of seven score tables, not a copied table.
ReplayLeaderboardRuntimeView::ReplayLeaderboardRuntimeView()
{
    memset(this, 0, sizeof(*this));
    unsigned int bytesRead = 0;
    sourceFile = LoadResourceFile(g_ReplayLeaderboardArchiveName, &bytesRead, 1);
    InitializeReplaySeedArchive(&seed);
    unsigned char *cursor = reinterpret_cast<unsigned char *>(scores);
    int byteOffset = static_cast<int>(cursor - reinterpret_cast<unsigned char *>(this)) - 8;
    while (byteOffset / 0x68d4 < 7)
    {
        InitializeReplayScoreArchive(reinterpret_cast<ReplayScoreArchiveView *>(cursor));
        byteOffset += 0x68d4;
        cursor += 0x68d4;
    }
    LoadReplayLeaderboardRecords(this);
}

// Native 0x437800..0x43785F: allocate the whole owner, construct from its
// physical header/tables, and publish the singleton when allocation succeeds.
void __cdecl CreateReplayLeaderboardRuntime()
{
    g_ReplayLeaderboardRuntime = new ReplayLeaderboardRuntimeView;
}

// Native 0x4378F0..0x437A6C. The original tries to validate and read the
// resource archive first; failed/absent archive data is replaced by a fresh
// minimal 0x18-byte TH11 file header, without aborting initialization.
int __stdcall LoadReplayLeaderboardRecords(ReplayLeaderboardRuntimeView *self)
{
    ReplayLeaderboardFileHeader *header =
        reinterpret_cast<ReplayLeaderboardFileHeader *>(self->sourceFile);
    if (header)
    {
        if (header->signature == 0x31314854 && header->version == 4)
        {
            unsigned char *payload = self->sourceFile + 0x18;
            ScatterXorBlocks(payload, header->packedSize, 0xAC, 0x35,
                0x10, header->packedSize);
            unsigned char *decoded = static_cast<unsigned char *>(
                malloc(header->decodedSize * 4));
            self->decodedFile = decoded;
            DecompressArchiveData(header->decodedSize,
                payload, header->packedSize, decoded);

            unsigned int remaining = header->decodedSize;
            unsigned char *record = decoded;
            while (remaining > 0)
            {
                unsigned short signature = *reinterpret_cast<unsigned short *>(record);
                unsigned short version = *reinterpret_cast<unsigned short *>(record + 2);
                unsigned int length = *reinterpret_cast<unsigned int *>(record + 8);
                if (signature == 0x5243)
                {
                    if (!version && length == 0x68d4 &&
                        reinterpret_cast<ReplayPayloadChecksumView *>(record)->SumPayloadBytes(0x68d4) ==
                            *reinterpret_cast<unsigned int *>(record + 4))
                    {
                        unsigned int index = *reinterpret_cast<unsigned int *>(record + 0x0c);
                        memcpy(&self->scores[index], record, 0x68d4);
                    }
                }
                else if (signature == 0x5453)
                {
                    if (!version && length == 0x448 &&
                        reinterpret_cast<ReplayPayloadChecksumView *>(record)->SumPayloadBytes(0x448) ==
                            *reinterpret_cast<unsigned int *>(record + 4))
                        memcpy(&self->seed, record, 0x448);
                }
                else return 0;
                if (remaining < length) break;
                remaining -= length;
                record += length;
            }
            return 0;
        }
        free(header);
        self->sourceFile = 0;
    }

    ResetReplayLeaderboardFileHeader(self);
    return 0;
}
