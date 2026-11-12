#include "ChainRegistration.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "Random.hpp"
#include <new>
#include <string.h>

// The replay writer owns structurally equivalent types in ReplayWriter.cpp;
// these are narrowly scoped physical views of the original initializing TU.
struct ReplayWriteState;
struct ReplayWriteSegmentNode;
ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment(ReplayWriteState *, int);

struct ReplayInitFileHeaderView { unsigned char bytes[0x24]; };
struct ReplayInitHeaderView { unsigned char bytes[0x70]; };
struct ReplayInitStageView { unsigned char bytes[0x90]; };
struct ReplayInitStreamView { void *first, *middle, *last; };
struct ReplayInitStateView
{
    unsigned char unknown000[8];
    ChainElement *update, *draw;
    int mode;
    ReplayInitFileHeaderView *fileHeader;
    ReplayInitHeaderView *header;
    ReplayInitStageView *stages[8];
    unsigned char unknown03c[4];
    ReplayInitStreamView streams[8];
    int segmentCount;
    unsigned char unknown0a4[0x1d0 - 0xa4];
    ChainElement *secondaryUpdate;
    int recordingStage;
    unsigned int flags;
};
typedef char ReplayInitStageStride[(sizeof(ReplayInitStageView) == 0x90) ? 1 : -1];
typedef char ReplayInitStateHeaderOffset[(offsetof(ReplayInitStateView, header) == 0x18) ? 1 : -1];
typedef char ReplayInitStateStreamsOffset[(offsetof(ReplayInitStateView, streams) == 0x40) ? 1 : -1];
typedef char ReplayInitStateLastStream[(offsetof(ReplayInitStateView, streams) + 7 * sizeof(ReplayInitStreamView) + 8 == 0x9c) ? 1 : -1];
typedef char ReplayInitStateRecordingStage[(offsetof(ReplayInitStateView, recordingStage) == 0x1d4) ? 1 : -1];

// These data roots/callbacks are directly observed in the Japanese PE.
// Their actual class/TU names and ownership have not been established.
extern ReplayInitStateView *g_SecondaryReplayCurrent;
extern unsigned int g_ReplayModeFlags, g_ReplayStatusMask;
extern unsigned int g_ReplayStageCharacter, g_ReplayStageScore, g_ReplayStageExtra;
extern unsigned int g_ReplayStageDifficult, g_ReplayStageFlags, g_ReplayStageBonus;
extern unsigned int g_ReplayStageValue, g_ReplayStageLife, g_ReplayStageLives;
extern unsigned int g_ReplayStageBombs, g_ReplayStageItemCount;
extern unsigned int g_ReplayStageBoost, g_ReplayStageExtraScore;
extern unsigned int g_ReplayStageEndScore;
extern RandomStateView g_TextRandomState;
extern unsigned int g_StageVariant;

struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage); // 00437420
void __stdcall InitializeReplayHeaderStorage(ReplayInitHeaderView *header); // 00420D80
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source); // 00436B60
int __fastcall ReplayRecordUpdate(void *context); // 00436D20
int __fastcall ReplayPlaybackUpdate(void *context); // 00436D30
int __fastcall ReplayAuxiliaryUpdate(void *context); // 00436D40
int __fastcall ReplayDrawCallback(void *context); // 00436D80

namespace
{
template<typename T> __forceinline T &At(void *ptr, unsigned int offset)
{ return *reinterpret_cast<T *>(reinterpret_cast<unsigned char *>(ptr) + offset); }

__forceinline ChainElement *ReplayChain(ReplayInitStateView *state,
    ChainCallback callback, bool draw)
{
    ChainElement *element = new ChainElement;
    if (element)
    {
        element->flags &= ~1u;
        element->callback = 0;
        element->unknown0c = 0;
        element->auxiliaryCallback = 0;
        element->priority = 0;
        element->link.element = element;
        element->link.next = 0;
        element->link.previous = 0;
    }
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = state;
    element->flags = (element->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(element, 0);
    else AddToUpdateChain(element, 0);
    return element;
}

__forceinline ReplayInitFileHeaderView *NewFileHeader()
{
    ReplayInitFileHeaderView *file = new ReplayInitFileHeaderView;
    if (file)
    {
        memset(file, 0, sizeof(*file));
        At<unsigned int>(file, 0) = 0x72313174;
        At<unsigned short>(file, 4) = 4;
        At<unsigned int>(file, 0x10) = 0x100;
    }
    return file;
}

__forceinline ReplayInitHeaderView *NewHeader()
{
    ReplayInitHeaderView *header = new ReplayInitHeaderView;
    if (header)
    {
        InitializeReplayHeaderStorage(header);
        memset(header, 0, sizeof(*header));
    }
    return header;
}

__forceinline ReplayInitStageView *NewStage()
{
    ReplayInitStageView *stage = new ReplayInitStageView;
    if (stage) memset(stage, 0, sizeof(*stage));
    return stage;
}

__forceinline void CaptureReplayStart(ReplayInitStateView *state,
    ReplayInitStageView *stage)
{
    ReplayInitHeaderView *header = state->header;
    At<int>(header, 0x5c) = g_MeshEffectSelection0;
    At<int>(header, 0x60) = g_MeshEffectSelection1;
    At<unsigned int>(header, 0x64) = g_ReplayStageCharacter;
    unsigned char &status = At<unsigned char>(header, 0x0a);
    status = static_cast<unsigned char>((status & ~1u) |
        ((g_ReplayStatusMask >> 4) & 1u));
    if (g_SecondaryControllerMode)
        memcpy(&At<unsigned char>(header, 0x18),
            reinterpret_cast<unsigned char *>(g_SecondaryControllerMode) + 0x24, 15 * 4);

    At<unsigned short>(stage, 0) = static_cast<unsigned short>(g_StageVariant);
    At<unsigned short>(stage, 2) = g_TextRandomState.seed;
    g_TextRandomState.calls = 0;
    unsigned int &stageFlags = At<unsigned int>(stage, 0x8c);
    stageFlags ^= (stageFlags ^ g_ReplayModeFlags) & 1u;
    if (g_ReplayModeFlags)
    {
        At<unsigned int>(stage, 0x20) = 0;
        At<unsigned int>(stage, 0x24) = 0;
    }
    At<unsigned int>(stage, 0x0c) = g_SecondaryActorScore;
    At<short>(stage, 0x10) = static_cast<short>(g_SecondaryPatternPower);
    int score = static_cast<int>(g_ReplayStageValue / 100);
    At<int>(stage, 0x14) = score - score % 10;
    At<short>(stage, 0x18) = static_cast<short>(g_SecondaryPlayerLives);
    At<short>(stage, 0x1a) = static_cast<short>(g_ReplayStageBombs);
    At<unsigned int>(stage, 0x1c) = g_ReplayStageFlags;
    At<unsigned int>(stage, 0x28) = g_ReplayStageBonus;
    At<unsigned int>(stage, 0x34) = g_SecondaryCollisionCount;
    unsigned int value = 0;
    for (int i = 0; i < 20; ++i)
    {
        At<unsigned int>(stage, 0x3c + i * 4) = value;
        value -= 0x21522153u;
    }
    At<unsigned int>(header, 0x6c) = g_ReplayStageBonus;
}

__forceinline void RestoreReplayState(ReplayInitStateView *state)
{
    memcpy(reinterpret_cast<unsigned char *>(g_SecondaryControllerMode) + 0x24,
        &At<unsigned char>(state->header, 0x18), 15 * 4);
    unsigned int selected = g_StageVariant;
    unsigned char *stream = reinterpret_cast<unsigned char *>(state) + 0xa4 + selected * 0x24;
    At<unsigned int>(stream, 8) = At<unsigned int>(stream, 4);
    At<unsigned int>(stream, 0xc) = At<unsigned int>(stream, 8);
    unsigned char *stage = At<unsigned char *>(stream, 0x10);
    At<unsigned int>(stream, 0x14) = 0;
    g_MeshEffectSelection0 = At<int>(state->header, 0x5c);
    g_MeshEffectSelection1 = At<int>(state->header, 0x60);
    g_ReplayStageCharacter = At<unsigned int>(state->header, 0x64);
    g_TextRandomState.seed = At<unsigned short>(stage, 2);
    g_TextRandomState.calls = 0;
    g_SecondaryActorScore = At<int>(stage, 0xc);
    g_SecondaryPatternPower = static_cast<int>(At<short>(stage, 0x10));
    if (g_SecondaryPatternPower > g_SecondaryPatternMaximumPower)
        g_SecondaryPatternPower = g_SecondaryPatternMaximumPower;
    g_ReplayStageValue = At<int>(stage, 0x14) * 100;
    g_SecondaryPlayerLives = At<short>(stage, 0x18);
    g_ReplayStageBombs = At<short>(stage, 0x1a);
    g_ReplayStageFlags = At<unsigned int>(stage, 0x1c);
    g_ReplayStageBonus = At<unsigned int>(stage, 0x28);
    g_ReplayStageExtraScore = At<unsigned int>(state->stages[selected], 0x2c);
    g_SecondaryCollisionCount = At<unsigned int>(stage, 0x34);
}
}

// Native 004356E0..00435C43 (1380 bytes, four exits): mode 0 records a
// fresh replay, mode 1 loads the saved replay and restores stage state,
// and mode 2 checks a replay stream without publishing a new chain.
int __stdcall InitializeSecondaryReplay(ReplayInitStateView *state,
    int mode, void *source)
{
    state->mode = mode;
    if (mode == 0)
    {
        g_SecondaryReplayCurrent = state;
        // The exact 0x437420 owner clears the *selected* stage's stream;
        // AppendReplayWriteSegment also receives the selected stage, while
        // its result is published in the separate +0x9C tail slot.
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(state),
            static_cast<int>(g_StageVariant));
        state->streams[7].last = AppendReplayWriteSegment(
            reinterpret_cast<ReplayWriteState *>(state), static_cast<int>(g_StageVariant));
        state->fileHeader = NewFileHeader();
        state->header = NewHeader();
        state->stages[g_StageVariant] = NewStage();
        CaptureReplayStart(state, state->stages[g_StageVariant]);
        state->update = ReplayChain(state, ReplayRecordUpdate, false);
        state->secondaryUpdate = ReplayChain(state, ReplayAuxiliaryUpdate, false);
        state->draw = ReplayChain(state, ReplayDrawCallback, true);
        state->recordingStage = static_cast<int>(g_StageVariant);
        return 0;
    }
    if (mode == 1)
    {
        g_SecondaryReplayCurrent = state;
        if (LoadReplayStageData(state, source) != 0) return -1;
        RestoreReplayState(state);
        state->update = ReplayChain(state, ReplayPlaybackUpdate, false);
        state->secondaryUpdate = ReplayChain(state, ReplayAuxiliaryUpdate, false);
        state->draw = ReplayChain(state, ReplayDrawCallback, true);
        state->recordingStage = -1;
        return 0;
    }
    if (mode == 2 && LoadReplayStageData(state, source) != 0)
        return -1;
    return 0;
}
