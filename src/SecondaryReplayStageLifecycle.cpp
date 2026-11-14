#include "SecondaryReplayState.hpp"
#include "SecondaryReplayStageLifecycle.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "SecondaryDamage.hpp"
#include <stddef.h>

struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
extern unsigned int g_ReplayModeFlags;
extern unsigned int g_ReplayStageBombs, g_ReplayStageFlags;
extern unsigned int g_ReplayStageBonus, g_ReplayStageExtraScore;

namespace
{
// One 0x90-byte stage record of the replay file.
struct ReplayStageRecordView
{
    unsigned char unknown000[0xc];
    int score;
    unsigned short power;
    unsigned char unknown012[2];
    int value;
    unsigned short lives, bombs;
    unsigned int flags;
    int positionX, positionY;
    unsigned int bonus, extraScore;
    unsigned int focus, collisions, special;
};
typedef char ReplayStageRecordSpecialOffset[(offsetof(ReplayStageRecordView, special) == 0x38) ? 1 : -1];

struct ReplayStageControllerView
{
    unsigned char unknown000[0x888];
    int integerX, integerY;
    unsigned char unknown890[0x7c90 - 0x890];
    unsigned int emitterState;
    unsigned char unknown7c94[0x8bac - 0x7c94];
    unsigned int special;
    unsigned char unknown8bb0[0x8d20 - 0x8bb0];
    unsigned int focus;
};
typedef char ReplayStageControllerFocusOffset[(offsetof(ReplayStageControllerView, focus) == 0x8d20) ? 1 : -1];

inline ReplayStageControllerView *ReplayController()
{
    return reinterpret_cast<ReplayStageControllerView *>(g_SecondaryControllerMode);
}
}

// 00436DA0. At a stage boundary hide the replay's chains, then either start
// a new recording segment and snapshot the stage state, or restore the
// player state from the replay's stage record.
void __stdcall UpdateSecondaryReplayStage()
{
    ReplayStateView *state = g_SecondaryReplayCurrent;
    if (state->update) state->update->flags |= 2;
    if (state->secondaryUpdate) state->secondaryUpdate->flags |= 2;
    if (state->draw) state->draw->flags |= 2;
    if (!state->mode)
    {
        ReplayStageRecordView *stage = static_cast<ReplayStageRecordView *>(state->stages[g_StageVariant]);
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(state), g_StageVariant);
        state->streams[7].last = AppendReplayWriteSegment(state, g_StageVariant);
        if (!g_ReplayModeFlags)
        {
            stage->score = g_SecondaryActorScore;
            stage->power = static_cast<unsigned short>(g_SecondaryPatternPower);
            stage->value = g_SecondaryPlayerValue / 100 - g_SecondaryPlayerValue / 100 % 10;
            stage->lives = static_cast<unsigned short>(g_SecondaryPlayerLives);
            stage->bombs = static_cast<unsigned short>(g_ReplayStageBombs);
            stage->flags = g_ReplayStageFlags;
            stage->bonus = g_ReplayStageBonus;
            stage->collisions = g_SecondaryCollisionCount;
        }
        stage->positionX = ReplayController()->integerX;
        stage->positionY = ReplayController()->integerY;
        stage->extraScore = g_ReplayStageExtraScore;
        stage->special = ReplayController()->special;
        state->recordingStage = g_StageVariant;
        static_cast<ReplayStageRecordView *>(state->stages[g_StageVariant])->focus = ReplayController()->focus;
    }
    else if (state->mode == 1)
    {
        state->recordingStage = g_StageVariant;
        ReplayStageRecordView *stage =
            static_cast<ReplayStageRecordView *>(state->stageLinks[g_StageVariant].stageHeader);
        SetSecondaryIntegerPlayerPosition(&stage->positionX);
        ReplayController()->focus = stage->focus;
        ReplayController()->special = stage->special;
        if (stage->special)
        {
            ReplayController()->emitterState = 0;
            ResetSecondaryPlayerEmitters(reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryControllerMode));
            ResetSecondaryPlayerEmitters(reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryControllerMode));
        }
    }
    state->frame = 0;
}
