#include "ChainRegistration.hpp"
#include "SecondaryDialogue.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "Random.hpp"
#include "SecondaryReplayState.hpp"
#include "ReplayHeaderStorageInitialization.hpp"
#include "ReplayRecordFactories.hpp"
#include <stddef.h>
#include <string.h>

extern unsigned int g_ReplayModeFlags, g_ReplayStatusMask;
extern unsigned int g_ReplayStageCharacter, g_ReplayStageFlags, g_ReplayStageBonus;
extern unsigned int g_ReplayStageBombs, g_ReplayStageExtraScore;
extern unsigned int g_StageVariant;
extern int g_SecondaryActorScore, g_SecondaryPatternPower, g_SecondaryPlayerValue;
extern int g_SecondaryPatternMaximumPower;
extern int g_SecondaryCollisionCount;
extern RandomStateView g_TextRandomState;
struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source);
int __fastcall ReplayRecordUpdate(void *context);
int __fastcall ReplayPlaybackUpdate(void *context);
int __fastcall ReplayAuxiliaryUpdate(void *context);
int __fastcall ReplayDrawCallback(void *context);

namespace
{
struct ReplayInitFileHeader
{
    unsigned int magic;
    unsigned short version;
    unsigned char unknown006[0x10 - 6];
    unsigned int flags;
    unsigned char unknown014[0x24 - 0x14];
    ReplayInitFileHeader()
    {
        memset(this, 0, sizeof(*this));
        magic = 0x72313174;
        version = 4;
        flags = 0x100;
    }
};
struct ReplayInitStorageMember
{
    ReplayHeaderStorageView storage;
    ReplayInitStorageMember() { InitializeReplayHeaderStorage(&storage); }
};
struct ReplayInitHeader
{
    unsigned char unknown000[0xa];
    unsigned char recordedFromArchive : 1;
    unsigned char unknown00a_bits : 7;
    unsigned char unknown00b[0x18 - 0xb];
    ReplayInitStorageMember settings;
    unsigned char unknown054[0x5c - 0x54];
    int character, shot, difficulty;
    unsigned char unknown068[4];
    unsigned int bonus;
    ReplayInitHeader() { memset(this, 0, sizeof(*this)); }
};
typedef char ReplayInitHeaderSize[(sizeof(ReplayInitHeader) == 0x70) ? 1 : -1];
struct ReplayInitStage
{
    short stage;
    unsigned short seed;
    unsigned char unknown004[0xc - 4];
    int score;
    unsigned short power;
    unsigned char unknown012[2];
    int value;
    unsigned short lives, bombs;
    unsigned int flags;
    int positionX, positionY;
    unsigned int bonus, extraScore;
    unsigned int focus, collisions, special;
    unsigned int keys[20];
    unsigned int fromReplay : 1;
    unsigned int unknown08c_bits : 31;
    ReplayInitStage() { memset(this, 0, sizeof(*this)); }
};
typedef char ReplayInitStageSize[(sizeof(ReplayInitStage) == 0x90) ? 1 : -1];
struct ReplayInitGateView
{
    unsigned char unknown000[0x24];
    ReplayHeaderStorageView storage;
};

struct ReplayChainElement : ChainElement
{
    __forceinline ReplayChainElement()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline ReplayChainElement *MakeReplayChain(ChainCallback callback, ReplayStateView *state)
{
    ReplayChainElement *element = new ReplayChainElement;
    element->SetCallback(callback);
    element->context = state;
    element->flags = (element->flags & ~2u) | 1;
    return element;
}

}

// 004356E0. Mode 0 starts a recording, mode 1 loads a replay for playback and
// restores the stage state, mode 2 only loads the replay.
int ReplayRecordRegistryView::InitializeRecord(TitleMenuRecordView *record, int mode)
{
    ReplayStateView *state = reinterpret_cast<ReplayStateView *>(record);
    state->mode = mode;
    if (mode == 0)
    {
        g_SecondaryReplayCurrent = state;
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(state), g_StageVariant);
        state->streams[7].last = AppendReplayWriteSegment(state, g_StageVariant);
        state->fileHeader = new ReplayInitFileHeader;
        state->header = new ReplayInitHeader;
        state->stages[g_StageVariant] = new ReplayInitStage;
        ReplayInitStage *stage = static_cast<ReplayInitStage *>(state->stages[g_StageVariant]);
        ReplayInitHeader *header = static_cast<ReplayInitHeader *>(state->header);
        static_cast<ReplayInitHeader *>(state->header)->character = g_MeshEffectSelection0;
        static_cast<ReplayInitHeader *>(state->header)->shot = g_MeshEffectSelection1;
        static_cast<ReplayInitHeader *>(state->header)->difficulty = g_ReplayStageCharacter;
        static_cast<ReplayInitHeader *>(state->header)->recordedFromArchive = g_ReplayStatusMask >> 4;
        if (g_SecondaryDialogueSession)
            static_cast<ReplayInitHeader *>(state->header)->settings.storage =
                reinterpret_cast<ReplayInitGateView *>(g_SecondaryDialogueSession)->storage;
        stage->stage = static_cast<short>(g_StageVariant);
        stage->seed = g_TextRandomState.seed;
        g_TextRandomState.calls = 0;
        stage->fromReplay = g_ReplayModeFlags;
        if (g_ReplayModeFlags)
        {
            stage->positionX = 0;
            stage->positionY = 0;
        }
        stage->score = g_SecondaryActorScore;
        stage->power = static_cast<unsigned short>(g_SecondaryPatternPower);
        stage->value = g_SecondaryPlayerValue / 100 - g_SecondaryPlayerValue / 100 % 10;
        stage->lives = static_cast<unsigned short>(g_SecondaryPlayerLives);
        stage->bombs = static_cast<unsigned short>(g_ReplayStageBombs);
        stage->flags = g_ReplayStageFlags;
        stage->bonus = g_ReplayStageBonus;
        stage->collisions = g_SecondaryCollisionCount;
        for (int i = 0; i < 20; ++i)
            stage->keys[i] = i * 0xdeaddead;
        static_cast<ReplayInitHeader *>(state->header)->bonus = g_ReplayStageBonus;
        (void)header;
        ReplayChainElement *element = MakeReplayChain(ReplayRecordUpdate, state);
        AddToUpdateChain(element, 0xb);
        state->update = element;
        element = MakeReplayChain(ReplayAuxiliaryUpdate, state);
        AddToUpdateChain(element, 0x1b);
        state->secondaryUpdate = element;
        element = MakeReplayChain(ReplayDrawCallback, state);
        AddToDrawChain(element, 0x38);
        state->draw = element;
        state->recordingStage = g_StageVariant;
        return 0;
    }
    if (mode == 1)
    {
        g_SecondaryReplayCurrent = state;
        if (LoadReplayStageData(state, this))
            return -1;
        reinterpret_cast<ReplayInitGateView *>(g_SecondaryDialogueSession)->storage =
            static_cast<ReplayInitHeader *>(state->header)->settings.storage;
        ReplayRecordStageLink *link = &state->stageLinks[g_StageVariant];
        link->eventsBeginCopy = link->eventsBegin;
        link->eventsEndCopy = link->eventsEnd;
        ReplayInitStage *stage = static_cast<ReplayInitStage *>(link->stageHeader);
        link->unknown014 = 0;
        g_MeshEffectSelection0 = static_cast<ReplayInitHeader *>(state->header)->character;
        g_MeshEffectSelection1 = static_cast<ReplayInitHeader *>(state->header)->shot;
        g_ReplayStageCharacter = static_cast<ReplayInitHeader *>(state->header)->difficulty;
        g_TextRandomState.seed = stage->seed;
        g_TextRandomState.calls = 0;
        g_SecondaryActorScore = stage->score;
        g_SecondaryPatternPower = static_cast<short>(stage->power);
        if (g_SecondaryPatternPower > g_SecondaryPatternMaximumPower)
            g_SecondaryPatternPower = g_SecondaryPatternMaximumPower;
        g_SecondaryPlayerValue = stage->value * 100;
        g_SecondaryPlayerLives = static_cast<short>(stage->lives);
        g_ReplayStageBombs = static_cast<short>(stage->bombs);
        g_ReplayStageFlags = stage->flags;
        g_ReplayStageBonus = stage->bonus;
        g_ReplayStageExtraScore =
            static_cast<ReplayInitStage *>(state->stageLinks[g_StageVariant].stageHeader)->extraScore;
        g_SecondaryCollisionCount = stage->collisions;
        ReplayChainElement *element = MakeReplayChain(ReplayPlaybackUpdate, state);
        AddToUpdateChain(element, 0xb);
        state->update = element;
        element = MakeReplayChain(ReplayAuxiliaryUpdate, state);
        AddToUpdateChain(element, 0x1b);
        state->secondaryUpdate = element;
        element = MakeReplayChain(ReplayDrawCallback, state);
        AddToDrawChain(element, 0x38);
        state->draw = element;
        state->recordingStage = -1;
        return 0;
    }
    if (mode == 2 && LoadReplayStageData(state, this))
        return -1;
    return 0;
}
