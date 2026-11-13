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

// These shared original .rdata constants are referenced by the native leaf.
extern const char g_PauseRecordEmptyName[9] = "        ";
extern const double g_PauseRecordHundred = 100.0;

// 0042BA80: insert the current score in descending order, preserving records
// below the insertion point and leaving the list unchanged if none qualifies.
int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock)
{
    int place = 0;
    int *scan = reinterpret_cast<int *>(
        scoreBlock + 0x10 + g_SecondaryActorDifficulty * 0x118);
    do
    {
        if (*scan <= g_SecondaryActorScore)
        {
            if (place < 10)
            {
                int index = 9;
                if (place < 9)
                {
                    do
                    {
                        unsigned char *source = scoreBlock + 0x10 +
                            (index - 1 + g_SecondaryActorDifficulty * 10) * 0x1c;
                        unsigned char *destination = scoreBlock + 0x10 +
                            (index + g_SecondaryActorDifficulty * 10) * 0x1c;
                        memcpy(destination, source, 0x1c);
                        --index;
                    } while (place < index);
                }

                *reinterpret_cast<int *>(scoreBlock + 0x10 +
                    (place + g_SecondaryActorDifficulty * 10) * 0x1c) =
                    g_SecondaryActorScore;
                *reinterpret_cast<unsigned char *>(scoreBlock + 0x15 +
                    (place + g_SecondaryActorDifficulty * 10) * 0x1c) =
                    g_PauseRecordShotType;
                *reinterpret_cast<unsigned char *>(scoreBlock + 0x14 +
                    (place + g_SecondaryActorDifficulty * 10) * 0x1c) =
                    static_cast<unsigned char>(g_StageVariant);
                _time64(reinterpret_cast<__time64_t *>(scoreBlock + 0x20 +
                    (place + g_SecondaryActorDifficulty * 10) * 0x1c));

                int row = place + g_SecondaryActorDifficulty * 10;
                unsigned char *name = scoreBlock + 0x16 + row * 0x1c;
                memcpy(name, g_PauseRecordEmptyName, sizeof(g_PauseRecordEmptyName));
                const float progress = static_cast<float>(
                    g_PauseRecordTiming->elapsed / g_PauseRecordTiming->limit);
                *reinterpret_cast<float *>(scoreBlock + 0x28 +
                    (place + g_SecondaryActorDifficulty * 10) * 0x1c) =
                    static_cast<float>(g_PauseRecordHundred - progress * g_PauseRecordHundred);
                return place;
            }
            else
            {
                return -1;
            }
        }
        ++place;
        scan += 7;
    } while (place < 10);
    return -1;
}
