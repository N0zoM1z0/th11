#include "MenuController.hpp"
#include "AsciiLifecycle.hpp"
#include "MainRuntime.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SoundQueue.hpp"
#include <stdlib.h>
#include <string.h>

namespace
{
__forceinline int Add(int value, int amount)
{
    return static_cast<int>(static_cast<unsigned int>(value) + amount);
}
__forceinline int &RuntimeMode()
{
    return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558);
}
__forceinline void PauseDraw(MenuControllerView *self)
{
    ChainElement *draw = *reinterpret_cast<ChainElement **>(
        reinterpret_cast<unsigned char *>(self) + 0x10);
    draw->flags |= 2;
}
__forceinline AnimationFileView *MenuFile(RenderBatchView *renderer, unsigned int offset)
{
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(renderer) + offset);
}
__forceinline void SelectCurrent(SecondaryDebugSelectionView *selection, int value)
{
    if (selection->count)
        selection->current = selection->count <= value ? Add(selection->count, -1) : value;
    else selection->current = value;
}
__forceinline void EnterPrimaryPage(MenuControllerView *self)
{
    SetConfigurationMenuPage(&self->page, 1);
    PauseDraw(self);
    UpdateMenuPage01(self);
}
__forceinline void AdvanceTime(StageTimerView *time)
{
    int previous = time->current;
    time->previous = previous;
    if (*time->scale > static_cast<double>(0.99f) && *time->scale < 1.01f)
    {
        time->current = Add(previous, 1);
        time->fraction = static_cast<float>(time->fraction + 1.0);
    }
    else
    {
        float elapsed = *time->scale + time->fraction;
        time->fraction = elapsed;
        time->current = static_cast<int>(elapsed);
    }
}
}

int __stdcall UpdateMenuController(MenuControllerView *self)
{
    if (self->page.page == 1)
    {
        int input = static_cast<unsigned short>(g_InputState.current);
        g_MenuIdleFrames = Add(g_MenuIdleFrames, 1);
        if (input) g_MenuIdleFrames = 0;
        else if (g_MenuIdleFrames >= 1800)
        {
            const char *source = g_MenuDemoPaths[g_MenuDemoIndex];
            g_StageResourceFlags = (g_StageResourceFlags & ~0x40u) | 0x20;
            strcpy(g_MenuReplayPath, source);
            MenuReplayView *replay = ReadMenuReplay(g_MenuReplayPath);
            g_MenuDemoIndex = Add(g_MenuDemoIndex, 1) % 4;
            int stage;
            for (stage = 0; stage < 8; ++stage)
                if (replay->stages[stage].marker) break;
            g_StageVariant = stage;
            g_MenuPreviousVariant = stage;
            RuntimeMode() = 13;
            MenuReplayHeaderView *header = replay->header;
            g_MeshEffectSelection0 = header->selection0;
            g_SecondaryGameplaySelectionEC8 = reinterpret_cast<SecondaryGameplaySelectionView *>(
                g_MenuStageSelections + stage * 0x40);
            g_MeshEffectSelection1 = header->selection1;
            g_MenuPreviousDifficulty = g_SecondaryActorDifficulty;
            g_SecondaryActorDifficulty = header->difficulty;
            DestroyMenuReplayContents(replay);
            free(replay);
            g_MenuEntryMode = 1;
            g_MenuIdleFrames = 0;
        }
    }

    int entryMode = g_MenuEntryMode;
    if ((self->flags & 1) && (self->musicDelay = Add(self->musicDelay, 1)) >= 5)
    {
        EnqueueMenuMusicLoad(0, "bgm/th11_00.wav");
        if (reinterpret_cast<const unsigned char *>(&g_MainRuntime)[0x200] & 0x10)
            EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
        EnqueueSoundCommand(&g_SoundManagerQueue, 2, 0, "dummy");
        entryMode = g_MenuEntryMode;
        g_SecondaryGameplayStatistics[0x2ddfa] = 1;
        self->flags &= ~1u;
        self->musicDelay = 0;
    }

    switch (self->page.page)
    {
    case 0:
    {
        RenderBatchView *renderer = g_RenderBatch;
        MarkMenuFileAnimations(renderer, MenuFile(renderer, 0x4350d4));
        MarkMenuFileAnimations(renderer, MenuFile(renderer, 0x4350d8));
        MarkMenuFileAnimations(renderer, MenuFile(renderer, 0x4350c8));
        MarkMenuFileAnimations(renderer, MenuFile(renderer, 0x4350c0));
        unsigned int *overlay = &g_AsciiManager->unknown184b4;
        SignalSecondaryAnimation(overlay);
        *overlay = 0;
        if (entryMode == 3)
        {
            self->page.selection.count = 10;
            SelectCurrent(&self->page.selection, 0);
            PushConfigurationSelection(&self->page.selection);
            SetConfigurationMenuPage(&self->page, 14);
            PauseDraw(self);
            g_MenuEntryMode = 1;
            goto Page0E;
        }
        if (!(g_StageResourceFlags & 0x20))
        {
            self->flags |= 1;
            self->musicDelay = 0;
        }
        else
        {
            self->flags &= ~1u;
            g_SecondaryActorDifficulty = g_MenuPreviousDifficulty;
        }
        g_StageResourceFlags &= ~0x20u;
        if (!entryMode)
        {
            self->flags |= 2;
            SetConfigurationMenuPage(&self->page, 1);
            g_MenuEntryMode = 1;
        }
        else
        {
            self->flags &= ~2u;
            if (entryMode == 1)
            {
                if (g_SecondaryActorDifficulty == 4)
                {
                    int count = self->page.selection.count;
                    if (count)
                    {
                        if (count <= 1)
                        {
                            self->page.selection.current = Add(count, -1);
                            EnterPrimaryPage(self);
                            break;
                        }
                        self->page.selection.current = 1;
                        EnterPrimaryPage(self);
                        break;
                    }
                    self->page.selection.current = 1;
                }
                EnterPrimaryPage(self);
                break;
            }
            if (entryMode == 2)
            {
                self->page.selection.count = 10;
                SelectCurrent(&self->page.selection, 3);
                PushConfigurationSelection(&self->page.selection);
                SetConfigurationMenuPage(&self->page, 11);
                PauseDraw(self);
                g_MenuEntryMode = 1;
                goto Page0B;
            }
        }
        goto Page01;
    }
    case 1:
    Page01:
        PauseDraw(self); UpdateMenuPage01(self); break;
    case 2: RuntimeMode() = (g_MainRuntime.flags & 0x2000) ? 2 : 3;
        // Fall through to the shared audio-reset operation.
    case 9:
    case 12: ResetSecondaryGameplayAudio(); break;
    case 3: UpdateConfigurationAudioMenu(&self->page); break;
    case 4: UpdateMenuPage04(self); break;
    case 5: UpdateMenuPage05(self); break;
    case 6: UpdateMenuPage06(self); break;
    case 7: UpdateMenuPage07(self); break;
    case 8: UpdateMenuPage08(self); break;
    case 10: UpdateMenuPage0A(self); break;
    case 11:
    Page0B:
        UpdateMenuPage0B(self); break;
    case 13: UpdateMenuPage0D(self); break;
    case 14:
    Page0E:
        UpdateMenuPage0E(self); break;
    case 15: UpdateMenuPage0F(self); break;
    }
    AdvanceTime(&self->page.time);
    return 1;
}
