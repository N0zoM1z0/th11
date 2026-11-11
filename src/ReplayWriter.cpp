#include "PauseMenu.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SharedFile.hpp"
#include "ByteCipher.hpp"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The recorder is distinct from the playback view: these layouts describe
// only fields read by 00436420 and its segment allocator at 00437300.
#pragma pack(push, 4)
struct ReplayWriteHeader
{
    char name[8];
    unsigned char unknown008[4];
    __time64_t timestamp;
    int score;
    unsigned char unknown018[0x54 - 0x18];
    float slowRate;
    int stageCount, character, shot, difficulty, stage;
    unsigned char unknown06c[4];
};
#pragma pack(pop)
struct ReplayWriteFileHeader
{
    unsigned char unknown000[0x0c];
    int dataEnd;
    unsigned char unknown010[0x1c - 0x10];
    int compressedSize, rawSize;
};
struct ReplayWriteStage
{
    int unknown000, eventCount, bytes;
    unsigned char unknown00c[0x90 - 0x0c];
};
struct ReplayWriteSegment;
struct ReplayWriteSegmentNode
{
    ReplayWriteSegment *segment;
    ReplayWriteSegmentNode *next, *previous;
};
struct ReplayWriteSegment
{
    unsigned char events[0x1518];
    unsigned char *eventEnd;
    unsigned char secondary[0x18a0 - 0x151c];
    unsigned char *secondaryEnd;
    ReplayWriteSegmentNode node;
};
struct ReplayWriteStream
{
    ReplayWriteSegmentNode *first, *unknown, *last;
};
struct ReplayWriteState
{
    unsigned char unknown000[0x14];
    ReplayWriteFileHeader *fileHeader;
    ReplayWriteHeader *header;
    ReplayWriteStage *stages[8];
    unsigned char unknown03c[4];
    ReplayWriteStream streams[8];
    int segmentCount;
    unsigned char unknown0a4[0x1d4 - 0xa4];
    int recordingStage;
    unsigned int flags;
};
typedef char ReplayHeaderStride[(sizeof(ReplayWriteHeader) == 0x70) ? 1 : -1];
typedef char ReplayStageStride[(sizeof(ReplayWriteStage) == 0x90) ? 1 : -1];
typedef char ReplaySegmentStride[(sizeof(ReplayWriteSegment) == 0x18b0) ? 1 : -1];
typedef char ReplayWriterFlagsOffset[(offsetof(ReplayWriteState, flags) == 0x1d8) ? 1 : -1];
typedef char ReplayHeaderTimestampOffset[(offsetof(ReplayWriteHeader, timestamp) == 0x0c) ? 1 : -1];
typedef char ReplayHeaderStageOffset[(offsetof(ReplayWriteHeader, stage) == 0x68) ? 1 : -1];
typedef char ReplayFileHeaderPackedOffset[(offsetof(ReplayWriteFileHeader, compressedSize) == 0x1c) ? 1 : -1];
typedef char ReplayStreamStartOffset[(offsetof(ReplayWriteState, streams) == 0x40) ? 1 : -1];
typedef char ReplayStreamTailOffset[((offsetof(ReplayWriteState, streams) + 7 * sizeof(ReplayWriteStream) + offsetof(ReplayWriteStream, last)) == 0x9c) ? 1 : -1];
typedef char ReplaySegmentEventEndOffset[(offsetof(ReplayWriteSegment, eventEnd) == 0x1518) ? 1 : -1];
typedef char ReplaySegmentSecondaryEndOffset[(offsetof(ReplayWriteSegment, secondaryEnd) == 0x18a0) ? 1 : -1];


struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
extern PauseRecordTimingView *g_PauseRecordTiming;

// These two name tables are observed at 004A3A88 and 004A3AA0.
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayDifficultyNames[];

// The native compressor is 004423A0; its original ownership is unresolved.
unsigned char *__stdcall CompressReplayData(const void *input, int size, int *compressedSize);

// 00437300: append a newly allocated chunk to the current stage's intrusive
// list. The node lives at offset 18A4 inside its 18B0-byte allocation.
ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment(ReplayWriteState *state, int stage)
{
    ReplayWriteSegment *chunk = static_cast<ReplayWriteSegment *>(malloc(sizeof(ReplayWriteSegment)));
    if (chunk)
    {
        memset(chunk, 0, sizeof(*chunk));
        chunk->eventEnd = reinterpret_cast<unsigned char *>(chunk);
        chunk->secondaryEnd = chunk->secondary;
        chunk->node.segment = chunk;
    }

    ReplayWriteSegmentNode **next = &state->streams[stage].first;
    while (*next)
        next = &(*next)->next;

    if (chunk)
    {
        // A first node refers back to the embedded list sentinel, not a heap
        // node; its 'next' word aliases the list's first pointer.
        chunk->node.previous = reinterpret_cast<ReplayWriteSegmentNode *>(
            reinterpret_cast<unsigned char *>(next) - offsetof(ReplayWriteSegmentNode, next));
        *next = &chunk->node;
    }
    ++state->segmentCount;
    return chunk ? &chunk->node : 0;
}

namespace
{
__forceinline int EventBytes(const ReplayWriteSegment *chunk)
{
    const unsigned char *start = reinterpret_cast<const unsigned char *>(chunk);
    return static_cast<int>((chunk->eventEnd - start) / 6) * 6;
}
__forceinline int ExtraBytes(const ReplayWriteSegment *chunk)
{
    return static_cast<int>(chunk->secondaryEnd - chunk->secondary);
}
__forceinline void WriteReplaySection(const void *buffer, unsigned int size)
{
    if (g_SharedFile == INVALID_HANDLE_VALUE)
        return;
    DWORD written;
    WriteFile(g_SharedFile, buffer, size, &written, 0);
    if (written != size)
    {
        CloseHandle(g_SharedFile);
        UnlockSharedFile();
    }
}
__forceinline unsigned int AlignFour(unsigned int length)
{
    return (length + 3u) & ~3u;
}
}

// 00436420..00436B5F. Conventional fastcall signature follows the observed
// ECX filename / EDX name / stack mode entry, without modeling private
// optimized ABIs of its helper functions as source declarations.
void __fastcall WritePauseMenuReplay(const char *filename, const char *name, int mode)
{
    ReplayWriteState *state = reinterpret_cast<ReplayWriteState *>(g_SecondaryControllerMode);
    ReplayWriteHeader *header = state->header;

    strcpy(header->name, name);
    int nameLength = static_cast<int>(strlen(name));
    for (int i = nameLength; i < 8; ++i)
        header->name[i] = ' ';

    if (!(state->flags & 1) && mode)
    {
        ReplayWriteSegment *last = state->streams[7].last->segment;
        short *end = reinterpret_cast<short *>(last->eventEnd);
        end[0] = -1;
        end[1] = -1;
        end[2] = -1;
        last->eventEnd += 6;
        if (EventBytes(last) / 6 >= 900)
            state->streams[7].last = AppendReplayWriteSegment(state, state->recordingStage);
    }

    _mkdir("replay");
    char path[256];
    sprintf(path, "replay/%s", filename);

    int rawSize = 0x70;
    int firstStage = 0, lastStage = 0, stageCount = 0;
    for (int i = 0; i < 8; ++i)
    {
        ReplayWriteStage *stage = state->stages[i];
        if (!stage) continue;
        if (!firstStage) firstStage = i;
        lastStage = i;
        ++stageCount;
        rawSize += 0x90;
        if (!(state->flags & 1)) stage->bytes = 0;
        for (ReplayWriteSegmentNode *node = state->streams[i].first; node; node = node->next)
        {
            ReplayWriteSegment *chunk = node->segment;
            int eventBytes = EventBytes(chunk);
            int extraBytes = ExtraBytes(chunk);
            rawSize += eventBytes + extraBytes;
            if (!(state->flags & 1))
            {
                stage->bytes += eventBytes + extraBytes;
                stage->eventCount += eventBytes / 6;
            }
        }
    }

    header->stageCount = stageCount;
    header->score = g_SecondaryActorScore;
    const float progress = static_cast<float>(
        g_PauseRecordTiming->elapsed / g_PauseRecordTiming->limit);
    header->slowRate = static_cast<float>(100.0 - progress * 100.0);

    unsigned char *raw = static_cast<unsigned char *>(malloc(rawSize));
    memcpy(raw, header, 0x70);
    int position = 0x70;
    for (int i = 0; i < 8; ++i)
    {
        ReplayWriteStage *stage = state->stages[i];
        if (!stage) continue;
        memcpy(raw + position, stage, 0x90);
        position += 0x90;

        for (ReplayWriteSegmentNode *node = state->streams[i].first; node; node = node->next)
        {
            ReplayWriteSegment *chunk = node->segment;
            int size = EventBytes(chunk);
            memcpy(raw + position, chunk->events, size);
            position += size;
        }
        for (ReplayWriteSegmentNode *node = state->streams[i].first; node; node = node->next)
        {
            ReplayWriteSegment *chunk = node->segment;
            int size = ExtraBytes(chunk);
            memcpy(raw + position, chunk->secondary, size);
            position += size;
        }
    }

    int packedSize;
    unsigned char *packed = CompressReplayData(raw, position, &packedSize);
    free(raw);
    GatherXorBlocks(packed, packedSize, 0x3d, 0x7a, 0x40, packedSize);
    GatherXorBlocks(packed, packedSize, 0xaa, 0xe1, 0x800, packedSize);

    state->fileHeader->rawSize = position;
    state->fileHeader->compressedSize = packedSize;
    state->fileHeader->dataEnd = packedSize + 0x24;
    OpenSharedFileForWrite(path);
    WriteReplaySection(state->fileHeader, 0x24);
    WriteReplaySection(packed, packedSize);
    if (packed) free(packed);

    // The metadata comprises two 4-byte-aligned USER sections: a printable
    // information block followed by the original Japanese comment prompt.
    unsigned char *text = static_cast<unsigned char *>(malloc(0xffff));
    memset(text, 0, 0xffff);
    char *cursor = reinterpret_cast<char *>(text + 12);
    *reinterpret_cast<unsigned int *>(text) = 0x52455355; // "USER"
    text[8] = 0;

    cursor += sprintf(cursor, "\x93\x8c\x95\xfb\x92\x6e\x97\xec\x93\x61\x20\x83\x8a\x83\x76\x83\x8c\x83\x43\x83\x74\x83\x40\x83\x43\x83\x8b\x8f\xee\x95\xf1\x0d\x0a");
    cursor += sprintf(cursor, "Version %s\r\n", "1.00a");
    cursor += sprintf(cursor, "Name %s\r\n", header->name);
    struct tm *time = _localtime64(&header->timestamp);
    cursor += sprintf(cursor, "Date %.2d/%.2d/%.2d %.2d:%.2d\r\n",
                      time->tm_year % 100, time->tm_mon + 1, time->tm_mday,
                      time->tm_hour, time->tm_min);
    cursor += sprintf(cursor, "Chara %s\r\n",
                      g_ReplayCharacterNames[header->character * 3 + header->shot]);
    cursor += sprintf(cursor, "Rank %s\r\n", g_ReplayDifficultyNames[header->difficulty]);
    if (header->stage < 8)
    {
        if (firstStage == lastStage)
        {
            if (firstStage == 7) cursor += sprintf(cursor, "Extra Stage\r\n");
            else cursor += sprintf(cursor, "Stage %d\r\n", firstStage);
        }
        else cursor += sprintf(cursor, "Stage %d \x81\x60 %d\r\n", firstStage, lastStage);
    }
    else if (firstStage == 7)
        cursor += sprintf(cursor, "Extra Stage Clear\r\n");
    else
        cursor += sprintf(cursor, "Stage All Clear\r\n");

    cursor += sprintf(cursor, "Score %d\r\n", header->score);
    cursor += sprintf(cursor, "Slow Rate %2.2f\r\n", header->slowRate);
    unsigned int size = AlignFour(static_cast<unsigned int>(cursor + 1 - reinterpret_cast<char *>(text)));
    *reinterpret_cast<unsigned int *>(text + 4) = size;
    WriteReplaySection(text, size);

    memset(text, 0, 0xffff);
    *reinterpret_cast<unsigned int *>(text) = 0x52455355; // "USER"
    text[8] = 1;
    cursor = reinterpret_cast<char *>(text + 12);
    cursor += sprintf(cursor, "\x83\x52\x83\x81\x83\x93\x83\x67\x82\xf0\x8f\x91\x82\xaf\x82\xdc\x82\xb7");
    size = AlignFour(static_cast<unsigned int>(cursor + 1 - reinterpret_cast<char *>(text)));
    *reinterpret_cast<unsigned int *>(text + 4) = size;
    WriteReplaySection(text, size);
    free(text);
    CloseSharedFile();
    state->flags |= 1;
}
