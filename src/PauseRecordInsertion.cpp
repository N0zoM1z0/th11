#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"
#include <string.h>
#include <time.h>

// The score list stores ten 28-byte records for each difficulty.
// The initial 16 bytes of the character's score block precede record zero.
#pragma pack(push, 4)
struct PauseHighScoreRecord
{
    int score;
    unsigned char stage, shot;
    char name[9];
    unsigned char unknown00f;
    __time64_t timestamp;
    float completion;
};
#pragma pack(pop)
typedef char PauseHighScoreRecordStride[(sizeof(PauseHighScoreRecord) == 0x1c) ? 1 : -1];
typedef char PauseHighScoreRecordTime[(offsetof(PauseHighScoreRecord, timestamp) == 0x10) ? 1 : -1];
typedef char PauseHighScoreRecordCompletion[(offsetof(PauseHighScoreRecord, completion) == 0x18) ? 1 : -1];

// Unnamed live globals observed at 004A573C and 004A8D80 respectively.
extern unsigned char g_PauseRecordShotType;
struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
extern PauseRecordTimingView *g_PauseRecordTiming;

// 0042BA80: insert the current score in descending order, preserving records
// below the insertion point and leaving the list unchanged if none qualifies.
int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock)
{
    PauseHighScoreRecord *records = reinterpret_cast<PauseHighScoreRecord *>(
        scoreBlock + 0x10 + g_SecondaryActorDifficulty * 0x118);

    int place = 0;
    while (place < 10 && records[place].score > g_SecondaryActorScore)
        ++place;
    if (place >= 10) return -1;

    for (int index = 9; index > place; --index)
        records[index] = records[index - 1];

    PauseHighScoreRecord &entry = records[place];
    entry.score = g_SecondaryActorScore;
    entry.shot = g_PauseRecordShotType;
    entry.stage = static_cast<unsigned char>(g_StageVariant);
    _time64(&entry.timestamp);
    memcpy(entry.name, "        ", 9);
    const float progress = static_cast<float>(
        g_PauseRecordTiming->elapsed / g_PauseRecordTiming->limit);
    entry.completion = static_cast<float>(100.0 - progress * 100.0);
    return place;
}
