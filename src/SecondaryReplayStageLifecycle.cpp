#include "SecondaryReplayState.hpp"
#include "SecondaryReplayStageLifecycle.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "SecondaryDamage.hpp"

struct ReplayCleanupState;
struct ReplayWriteSegmentNode;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment(ReplayWriteState *state, int stage);
void __fastcall SetSecondaryIntegerPlayerPosition(const int *position);

extern unsigned int g_StageVariant;
extern unsigned int g_ReplayModeFlags;
extern unsigned int g_ReplayStageBombs, g_ReplayStageFlags, g_ReplayStageBonus;
extern unsigned int g_ReplayStageExtraScore;

namespace
{
template<typename T> __forceinline T &ReplayStageField(ReplayStageHeaderView *stage,
    unsigned int offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<unsigned char *>(stage) + offset);
}

__forceinline void CaptureReplayStageStatistics(ReplayStageHeaderView *stage)
{
    ReplayStageField<int>(stage, 0x0c) = g_SecondaryActorScore;
    ReplayStageField<short>(stage, 0x10) = static_cast<short>(g_SecondaryPatternPower);
    const int score = g_SecondaryPlayerValue / 100;
    ReplayStageField<int>(stage, 0x14) = score - score % 10;
    ReplayStageField<short>(stage, 0x18) = static_cast<short>(g_SecondaryPlayerLives);
    ReplayStageField<short>(stage, 0x1a) = static_cast<short>(g_ReplayStageBombs);
    ReplayStageField<unsigned int>(stage, 0x1c) = g_ReplayStageFlags;
    ReplayStageField<unsigned int>(stage, 0x28) = g_ReplayStageBonus;
    ReplayStageField<unsigned int>(stage, 0x34) =
        static_cast<unsigned int>(g_SecondaryCollisionCount);
}
}

// Complete native 00436DA0..00436F23 (388 bytes). This is the stage-boundary
// half of replay playback/recording, separate from the 1380-byte initializer:
// retire the three chain nodes, rotate the active stream, capture/restore the
// stage snapshot, and reset the frame counter on every path.
void __stdcall UpdateSecondaryReplayStage()
{
    ReplayStateView *state = g_SecondaryReplayCurrent;
    if (state->update) state->update->flags |= 2;
    if (state->secondaryUpdate) state->secondaryUpdate->flags |= 2;
    if (state->draw) state->draw->flags |= 2;

    const unsigned int stageIndex = g_StageVariant;
    if (state->mode == 0)
    {
        ReplayStageHeaderView *stage =
            reinterpret_cast<ReplayStageHeaderView *>(state->stages[stageIndex]);
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(state),
            static_cast<int>(stageIndex));
        state->streams[7].last = AppendReplayWriteSegment(state,
            static_cast<int>(stageIndex));
        if (!g_ReplayModeFlags)
            CaptureReplayStageStatistics(stage);

        SecondaryPatternOwnerView *controller =
            reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryControllerMode);
        ReplayStageField<int>(stage, 0x20) = controller->x;
        ReplayStageField<int>(stage, 0x24) = controller->y;
        ReplayStageField<unsigned int>(stage, 0x2c) = g_ReplayStageExtraScore;
        ReplayStageField<int>(stage, 0x38) = controller->multiplier;
        state->recordingStage = static_cast<int>(stageIndex);
        ReplayStageHeaderView *selected =
            reinterpret_cast<ReplayStageHeaderView *>(state->stages[stageIndex]);
        ReplayStageField<int>(selected, 0x30) = controller->focused;
    }
    else if (state->mode == 1)
    {
        ReplayStageHeaderView *stage = reinterpret_cast<ReplayStageHeaderView *>(
            state->stageLinks[stageIndex].stageHeader);
        state->recordingStage = static_cast<int>(stageIndex);
        SetSecondaryIntegerPlayerPosition(reinterpret_cast<const int *>(
            reinterpret_cast<unsigned char *>(stage) + 0x20));
        SecondaryPatternOwnerView *controller =
            reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryControllerMode);
        controller->focused = ReplayStageField<int>(stage, 0x30);
        controller->multiplier = ReplayStageField<int>(stage, 0x38);
        if (controller->multiplier != 0)
        {
            SecondaryDamageReferencesView *damage =
                reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryControllerMode);
            damage->patternSlot = 0;
            ResetSecondaryPlayerEmitters(damage);
            damage = reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryControllerMode);
            ResetSecondaryPlayerEmitters(damage);
        }
    }
    state->frame = 0;
}
