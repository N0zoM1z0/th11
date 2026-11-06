#include "SecondaryGameplay.hpp"

namespace
{
__forceinline SecondaryGameplayHudView *Hud()
{ return reinterpret_cast<SecondaryGameplayHudView *>(g_SecondaryActorHud); }
__forceinline int &RuntimeMode()
{ return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558); }
__forceinline void PauseRequiredChains(SecondaryGameplayChainsView *self)
{
    self->update->flags |= 2;
    self->draw->flags |= 2;
}
}

int __stdcall UpdateSecondaryGameplay(SecondaryGameplayView *self)
{
    if ((Hud()->flags & 0x200) && Hud()->transitionFrame < 120) return 1;
    if (self->time.current == 0)
    {
        ResetSecondaryGameplayHud();
        if (self->flags & 8)
        {
            StopRuntimeThread(&FrameRuntime().worker);
            RuntimeMode() = ((~(g_MainRuntime.flags >> 13)) & 1) | 2;
            return 1;
        }
        ResetSecondaryGameplayStage();
        if (g_StageSecondary)
        {
            StartSecondaryGameplayStageTransition();
            ResetSecondaryGameplayStageTimer();
            self->flags |= 0x800;
            SignalSecondaryAnimation(&Hud()->signal4370);
        }
        else
        {
            self->flags &= ~0x800u;
            ResetSecondaryGameplayProjectiles();
            ResetSecondaryGameplayPlayer();
            memset(reinterpret_cast<unsigned char *>(g_SecondaryAttractionItems) + 0x14, 0, 0x265e50);
            ResetSecondaryGameplayActors(g_SecondaryMaster);
            ReleaseSecondaryGameplayObjects();
            g_SecondaryGameplayFrame0 = 0;
            g_SecondaryGameplayFrame1 = 0;
            ResetSecondaryGameplayMode();
            SecondaryHostSpawnArguments arguments;
            memset(&arguments, 0, sizeof(arguments));
            CreateSecondaryHostActor("main", &arguments);
            InitializeSecondaryGameplayHud();
            PauseSecondaryGameplayBoss();
            PauseSecondaryGameplayPlayer();
            PauseSecondaryGameplayObject(g_SceneClearPool);
            PauseSecondaryGameplayActors();
            PauseSecondaryGameplayObject(g_SecondaryAttractionItems);
            PauseSecondaryGameplayObject(g_SecondaryClearManager);
            PauseRequiredChains(g_SecondaryGameplayChainsD74);
            PauseSecondaryGameplayObject(g_MeshEffectEnemy);
            PauseRequiredChains(g_SecondaryGameplayChainsEC0);
            PauseSecondaryGameplayItems();
            PauseSecondaryGameplaySceneEffect();
            if (!(g_StageResourceFlags & 0x20))
            {
                if (g_StageVariant != 6)
                    StartSecondaryGameplayAudio(0, g_SecondaryGameplaySelectionEC8->value);
                else
                {
                    StartSecondaryGameplayAudio(0, g_SecondaryGameplaySelectionEC8->value);
                    StartSecondaryGameplayExtraAudio();
                }
            }
            SignalSecondaryAnimation(&g_AsciiManager->unknown184b0);
            ClearSecondaryGameplayAsciiOverlay();
        }
    }
    else if (self->time.current == 30 && (self->flags & 0x800))
    {
        self->flags &= ~0x800u;
        ResetSecondaryGameplayProjectiles();
        ResetSecondaryGameplayPlayer();
        memset(reinterpret_cast<unsigned char *>(g_SecondaryAttractionItems) + 0x14, 0, 0x265e50);
        ResetSecondaryGameplayActors(g_SecondaryMaster);
        ReleaseSecondaryGameplayObjects();
        g_SecondaryGameplayFrame0 = 0;
        g_SecondaryGameplayFrame1 = 0;
        ResetSecondaryGameplayMode();
        SecondaryHostSpawnArguments arguments;
        memset(&arguments, 0, sizeof(arguments));
        CreateSecondaryHostActor("main", &arguments);
        InitializeSecondaryGameplayHud();
        PauseSecondaryGameplayBoss();
        PauseSecondaryGameplayPlayer();
        PauseSecondaryGameplayObject(g_SceneClearPool);
        PauseSecondaryGameplayActors();
        PauseSecondaryGameplayObject(g_SecondaryAttractionItems);
        PauseSecondaryGameplayObject(g_SecondaryClearManager);
        PauseRequiredChains(g_SecondaryGameplayChainsD74);
        PauseSecondaryGameplayObject(g_MeshEffectEnemy);
        PauseRequiredChains(g_SecondaryGameplayChainsEC0);
        PauseSecondaryGameplayItems();
        PauseSecondaryGameplaySceneEffect();
        ResetSecondaryGameplayAudio();
        if (g_StageVariant != 6)
            StartSecondaryGameplayAudio(0, g_SecondaryGameplaySelectionEC8->value);
        else
        {
            StartSecondaryGameplayAudio(0, g_SecondaryGameplaySelectionEC8->value);
            StartSecondaryGameplayExtraAudio();
        }
        ClearSecondaryGameplayAsciiOverlay();
        SetStageTimer(&self->time, 0);
    }
    if (self->time.current == 5) g_SecondaryGameplayAudioState = 2;
    StageResourceView *stage = g_StageSecondary;
    if (stage && (stage->flags2ff0 & 8)) delete stage;
    if (self->flags & 4)
    {
        self->flags |= 0x80;
        return 1;
    }
    StopRuntimeThread(&FrameRuntime().worker);
    if (g_StageResourceFlags & 0x20)
    {
        if ((g_InputState.current & 0x80103) || (self->flags & 0x70))
            RuntimeMode() = (g_MainRuntime.flags & 0x2000) ? 2 : 4;
        if (self->time.current == 3540)
            CreateSecondaryHostScreenEffect(5, 60, 0, 0, 0x39);
        else if (self->time.current == 3600)
            SetSecondaryGameplayRuntimeMode(&g_MainRuntime, 4);
    }
    else if (g_StageVariant == 6 && self->time.current == 300)
        AdvanceSecondaryGameplayExtraAudio();
    UpdateSecondaryGameplayHud();
    if (self->flags & 0x10) return 3;
    if (self->flags & 0x20) return 3;
    if (self->flags & 0x40) return 3;
    if (g_SecondaryControllerMode->mode != 1)
    {
        unsigned int index = (static_cast<unsigned int>(g_MeshEffectSelection0) * 3u +
            static_cast<unsigned int>(g_MeshEffectSelection1)) * 0x68d4u;
        int *record = reinterpret_cast<int *>(g_SecondaryGameplayStatistics + index + 0x594);
        if (*record < 215999999) ++*record;
    }
    if (!Hud()->dialogue && self->time.current >= 90)
        UpdateSecondaryGameplayGauges(&g_SecondaryHostSelectionState);
    ++g_SecondaryGameplayFrame0;
    ++g_SecondaryGameplayFrame1;
    AdvanceStageTimer(&self->time);
    return 1;
}
