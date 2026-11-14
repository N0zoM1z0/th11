#include "PauseMenu.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryReplayState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SharedFile.hpp"
#include "ByteCipher.hpp"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The writer and playback loader share the same physical replay-state root.
// The stage-link table is also populated by the original 00436B60 file loader.
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
    ReplayWriteSegment()
    {
        memset(this, 0, sizeof(*this));
        eventEnd = reinterpret_cast<unsigned char *>(this);
        secondaryEnd = secondary;
        node.segment = this;
        node.next = 0;
        node.previous = 0;
    }
    unsigned char events[0x1518];
    unsigned char *eventEnd;
    unsigned char secondary[0x18a0 - 0x151c];
    unsigned char *secondaryEnd;
    ReplayWriteSegmentNode node;
};
typedef ReplayStateView ReplayWriteState;
typedef ReplayStageStreamView ReplayWriteStream;
extern ReplayWriteState *g_SecondaryReplayCurrent;
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


#pragma pack(push, 4)
struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
#pragma pack(pop)
extern PauseRecordTimingView *g_PauseRecordTiming;

// These two name tables are observed at 004A3A88 and 004A3AA0.
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayDifficultyNames[];

// The native compressor is 004423A0; its original ownership is unresolved.
unsigned char *__stdcall CompressReplayData(const void *input, int size, int *compressedSize);

// 00437300: append a newly allocated chunk to the current stage's intrusive
// list. The node lives at offset 18A4 inside its 18B0-byte allocation.
extern "C" void *__cdecl ReplayAllocate(unsigned int size);
extern "C" void *__cdecl ReplayMemset(void *destination, int value, unsigned int size);
extern "C" void *__cdecl ReplayMemcpy(void *destination, const void *source, unsigned int size);
#pragma comment(linker, "/alternatename:_ReplayAllocate=??2@YAPAXI@Z")
#pragma comment(linker, "/alternatename:_ReplayMemset=_memset")
#pragma comment(linker, "/alternatename:_ReplayMemcpy=_memcpy")

namespace
{
inline void InsertReplaySegmentAfter(ReplayWriteSegmentNode *self, ReplayWriteSegmentNode *node)
{
    if (self->next)
    {
        node->next = self->next;
        self->next->previous = node;
    }
    self->next = node;
    node->previous = self;
}
}

// 00437300. Each stage owns a list head {segment, next, previous} at
// 0x3C + 12 * stage; a new segment goes at the tail.
ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment(ReplayWriteState *state, int stage)
{
    ReplayWriteSegment *segment = new ReplayWriteSegment;
    ReplayWriteSegmentNode *node = &segment->node;
    ReplayWriteSegmentNode *tail = reinterpret_cast<ReplayWriteSegmentNode *>(state->unknown03c) + stage;
    while (tail->next)
        tail = tail->next;
    InsertReplaySegmentAfter(tail, node);
    ++state->segmentCount;
    return node;
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
    if (size != written)
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

namespace
{
inline ReplayWriteHeader *HeaderOf(ReplayWriteState *state)
{
    return static_cast<ReplayWriteHeader *>(state->header);
}
inline ReplayWriteFileHeader *FileHeaderOf(ReplayWriteState *state)
{
    return static_cast<ReplayWriteFileHeader *>(state->fileHeader);
}
inline ReplayWriteStage *StageOf(ReplayWriteState *state, int index)
{
    return static_cast<ReplayWriteStage *>(state->stages[index]);
}
inline ReplayWriteSegmentNode *FirstSegment(ReplayWriteState *state, int index)
{
    return static_cast<ReplayWriteSegmentNode *>(state->streams[index].first);
}
inline int EventCount(const ReplayWriteSegment *chunk)
{
    return static_cast<int>(chunk->eventEnd - reinterpret_cast<const unsigned char *>(chunk)) / 6;
}
inline void AddReplayEvent(ReplayWriteSegment *segment, unsigned short current,
    unsigned short pressed, unsigned short released)
{
    reinterpret_cast<unsigned short *>(segment->eventEnd)[0] = current;
    reinterpret_cast<unsigned short *>(segment->eventEnd)[1] = pressed;
    reinterpret_cast<unsigned short *>(segment->eventEnd)[2] = released;
    segment->eventEnd += 6;
}
}

// 00436420..00436B5F. ECX filename, EDX display name, stack mode.
int __fastcall WritePauseMenuReplay(const char *filename, const char *name, int mode)
{
    ReplayWriteState *state = g_SecondaryReplayCurrent;
    int firstStage = 0;
    int lastStage = 0;
    strcpy(HeaderOf(state)->name, name);
    int length = static_cast<int>(strlen(name));
    while (length < 8)
        HeaderOf(state)->name[length++] = ' ';

    if (!(state->flags & 1) && mode)
    {
        ReplayWriteSegment *segment = static_cast<ReplayWriteSegmentNode *>(state->streams[7].last)->segment;
        AddReplayEvent(segment, 0xffff, 0xffff, 0xffff);
        if ((segment->eventEnd - reinterpret_cast<unsigned char *>(segment)) / 6 >= 900)
            state->streams[7].last = AppendReplayWriteSegment(state, state->recordingStage);
    }

    _mkdir("replay");
    char path[256];
    sprintf(path, "replay/%s", filename);

    int stageCount = 0;
    int rawSize = 0x70;
    for (int i = 0; i < 8; ++i)
    {
        if (!state->stages[i]) continue;
        if (!firstStage) firstStage = i;
        lastStage = i;
        if (!(state->flags & 1)) StageOf(state, i)->bytes = 0;
        rawSize += 0x90;
        for (ReplayWriteSegmentNode *node = FirstSegment(state, i); node; node = node->next)
        {
            rawSize += EventBytes(node->segment) + ExtraBytes(node->segment);
            if (!(state->flags & 1))
            {
                StageOf(state, i)->bytes += EventBytes(node->segment) + ExtraBytes(node->segment);
                StageOf(state, i)->eventCount += EventCount(node->segment);
            }
        }
        ++stageCount;
    }

    HeaderOf(state)->stageCount = stageCount;
    HeaderOf(state)->score = g_SecondaryActorScore;
    float progress = static_cast<float>(g_PauseRecordTiming->elapsed / g_PauseRecordTiming->limit);
    float slowRate = static_cast<float>(100.0 - progress * 100.0);
    HeaderOf(state)->slowRate = slowRate;

    unsigned char *raw = static_cast<unsigned char *>(malloc(rawSize));
    memcpy(raw, state->header, 0x70);
    int position = 0x70;
    for (int i = 0; i < 8; ++i)
    {
        if (state->stages[i])
        {
            memcpy(raw + position, state->stages[i], 0x90);
            position += 0x90;
            for (ReplayWriteSegmentNode *node = FirstSegment(state, i); node; node = node->next)
            {
                ReplayWriteSegment *chunk = node->segment;
                memcpy(raw + position, chunk->events, EventBytes(chunk));
                position += EventBytes(chunk);
            }
            for (ReplayWriteSegmentNode *node = FirstSegment(state, i); node; node = node->next)
            {
                ReplayWriteSegment *chunk = node->segment;
                memcpy(raw + position, chunk->secondary, ExtraBytes(chunk));
                position += ExtraBytes(chunk);
            }
        }
    }

    int packedSize;
    unsigned char *packed = CompressReplayData(raw, position, &packedSize);
    free(raw);
    GatherXorBlocks(packed, packedSize, 0x3d, 0x7a, 0x40, packedSize);
    GatherXorBlocks(packed, packedSize, 0xaa, 0xe1, 0x800, packedSize);
    FileHeaderOf(state)->rawSize = position;
    FileHeaderOf(state)->compressedSize = packedSize;
    FileHeaderOf(state)->dataEnd = FileHeaderOf(state)->compressedSize + 0x24;
    OpenSharedFileForWrite(path);
    WriteReplaySection(FileHeaderOf(state), 0x24);
    WriteReplaySection(packed, packedSize);
    if (packed) free(packed);

    // Two USER sections: a printable information block and the comment prompt.
    unsigned char *text = static_cast<unsigned char *>(malloc(0xffff));
    memset(text, 0, 0xffff);
    char *cursor = reinterpret_cast<char *>(text + 12);
    *reinterpret_cast<unsigned int *>(text) = 0x52455355; // "USER"
    text[8] = 0;
    cursor += sprintf(cursor, "\x93\x8c\x95\xfb\x92\x6e\x97\xec\x93\x61\x20\x83\x8a\x83\x76\x83\x8c\x83\x43\x83\x74\x83\x40\x83\x43\x83\x8b\x8f\xee\x95\xf1\r\n");
    cursor += sprintf(cursor, "Version %s\r\n", "1.00a");
    cursor += sprintf(cursor, "Name %s\r\n", HeaderOf(state)->name);
    tm *date = _localtime64(&HeaderOf(state)->timestamp);
    cursor += sprintf(cursor, "Date %.2d/%.2d/%.2d %.2d:%.2d\r\n", date->tm_year % 100,
        date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min);
    cursor += sprintf(cursor, "Chara %s\r\n",
        g_ReplayCharacterNames[HeaderOf(state)->character * 3 + HeaderOf(state)->shot]);
    cursor += sprintf(cursor, "Rank %s\r\n", g_ReplayDifficultyNames[HeaderOf(state)->difficulty]);
    int printed;
    if (HeaderOf(state)->stage > 7)
    {
        if (firstStage == 7)
            printed = sprintf(cursor, "Extra Stage Clear\r\n");
        else
            printed = sprintf(cursor, "Stage All Clear\r\n");
    }
    else if (firstStage == lastStage)
    {
        if (firstStage == 7)
            printed = sprintf(cursor, "Extra Stage\r\n");
        else
            printed = sprintf(cursor, "Stage %d\r\n", firstStage);
    }
    else
        printed = sprintf(cursor, "Stage %d \x81\x60 %d\r\n", firstStage, lastStage);
    cursor += printed;
    cursor += sprintf(cursor, "Score %d\r\n", HeaderOf(state)->score);
    cursor += sprintf(cursor, "Slow Rate %2.2f\r\n", HeaderOf(state)->slowRate) + 1;
    if ((cursor - reinterpret_cast<char *>(text)) % 4)
        cursor += 4 - (cursor - reinterpret_cast<char *>(text)) % 4;
    unsigned int size = static_cast<unsigned int>(cursor - reinterpret_cast<char *>(text));
    *reinterpret_cast<unsigned int *>(text + 4) = size;
    WriteReplaySection(text, size);

    memset(text, 0, 0xffff);
    *reinterpret_cast<unsigned int *>(text) = 0x52455355; // "USER"
    text[8] = 1;
    cursor = reinterpret_cast<char *>(text + 12);
    cursor += sprintf(cursor, "\x83\x52\x83\x81\x83\x93\x83\x67\x82\xf0\x8f\x91\x82\xaf\x82\xdc\x82\xb7") + 1;
    if ((cursor - reinterpret_cast<char *>(text)) % 4)
        cursor += 4 - (cursor - reinterpret_cast<char *>(text)) % 4;
    size = static_cast<unsigned int>(cursor - reinterpret_cast<char *>(text));
    *reinterpret_cast<unsigned int *>(text + 4) = size;
    WriteReplaySection(text, size);
    free(text);
    CloseSharedFile();
    state->flags |= 1;
    return 0;
}
