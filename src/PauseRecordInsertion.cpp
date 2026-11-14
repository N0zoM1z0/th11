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
#pragma pack(push, 4)
struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
#pragma pack(pop)
typedef char PauseRecordTimingElapsedOffset[(offsetof(PauseRecordTimingView, elapsed) == 0x24) ? 1 : -1];
typedef char PauseRecordTimingLimitOffset[(offsetof(PauseRecordTimingView, limit) == 0x2c) ? 1 : -1];
extern PauseRecordTimingView *g_PauseRecordTiming;

struct PauseHighScoreBlock
{
    unsigned char unknown000[0x10];
    PauseHighScoreRecord records[][10];
};

// 0042BA80. Insert the current score in descending order; return its place
// or -1 when it does not qualify.
int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock)
{
    PauseHighScoreBlock *block = reinterpret_cast<PauseHighScoreBlock *>(scoreBlock);
    int place;
    for (place = 0; place < 10; ++place)
        if (block->records[g_SecondaryActorDifficulty][place].score <= g_SecondaryActorScore)
            break;
    if (place >= 10)
        return -1;
    for (int i = 9; i > place; --i)
        block->records[g_SecondaryActorDifficulty][i] = block->records[g_SecondaryActorDifficulty][i - 1];
    block->records[g_SecondaryActorDifficulty][place].score = g_SecondaryActorScore;
    block->records[g_SecondaryActorDifficulty][place].shot = g_PauseRecordShotType;
    block->records[g_SecondaryActorDifficulty][place].stage = static_cast<unsigned char>(g_StageVariant);
    _time64(&block->records[g_SecondaryActorDifficulty][place].timestamp);
    strcpy(block->records[g_SecondaryActorDifficulty][place].name, "        ");
    float rate = static_cast<float>(g_PauseRecordTiming->elapsed / g_PauseRecordTiming->limit);
    float completion = static_cast<float>(100.0 - rate * 100.0);
    block->records[g_SecondaryActorDifficulty][place].completion = completion;
    return place;
}
