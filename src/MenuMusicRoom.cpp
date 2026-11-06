#include "MenuMusicRoom.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "AsciiLifecycle.hpp"
#include "FormattedTextureText.hpp"
#include "MainRuntime.hpp"
#include "MeshEffects.hpp"
#include "ResourceFile.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SoundQueue.hpp"
#include "VmBindings.hpp"
#include <math.h>
#include <stdlib.h>

namespace
{
__forceinline void SelectFirst(SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        selection->current = selection->count <= 0 ? selection->count - 1 : 0;
    else selection->current = 0;
}
__forceinline CameraVector *MusicPosition(AnimationVmView *vm)
{
    return reinterpret_cast<CameraVector *>(reinterpret_cast<unsigned char *>(vm) + 0x3dc);
}
__forceinline void SetEvent(AnimationVmView *vm, short event)
{
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
}
__forceinline void SignalTree(AnimationVmView *vm, short event)
{
    if (vm)
    {
        SetEvent(vm, event);
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                SetEvent(reinterpret_cast<AnimationVmView *>(node->vm), event);
    }
}
__forceinline void SetTrackVisibility(MenuMusicRoomView *music, AnimationVmView *vm, int track)
{
    if (track >= music->firstVisibleTrack && track < music->firstVisibleTrack + 10)
        vm->flags.value |= 2;
    else vm->flags.value &= ~2u;
}
__forceinline void RefreshComment(MenuControllerView *self, MenuMusicRoomView *music)
{
    if (self->page.time.current % 2 == 0 && music->nextCommentLine < 8)
    {
        AnimationVmView *vm = ClearMissingAnimationId(
            &music->commentVmIds[music->nextCommentLine].value);
        if (!g_SecondaryGameplayStatistics[0x2ddfa + music->commentTrack] && music->warningPending)
            FormatTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                0x8080ff, 0, 0, 0, 0, g_MenuMusicWarningLines[music->nextCommentLine]);
        else
            FormatTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                0xffffff, 0, 0, 0, 0, music->comments[music->commentTrack][music->nextCommentLine]);
        SetEvent(vm, 2);
        ++music->nextCommentLine;
    }
}
}

int __stdcall UpdateMenuPage0D(MenuControllerView *self)
{
    MenuMusicRoomView *music = reinterpret_cast<MenuMusicRoomView *>(self);
    SecondaryDebugSelectionView *selection = &self->page.selection;
    switch (self->page.state)
    {
    case 0:
        if (self->page.time.current == 1)
        {
            selection->count = 6;
            SelectFirst(selection);
            if (!FindAnimationVmById(g_RenderBatch, MenuAnimationId(self, 0x5d)))
            {
                StartMenuAnimation(self, 0x5d);
                MenuAnimationId(self, 0xc1) = CreateAnimationVmId(
                    reinterpret_cast<AnimationFileView *>(g_AsciiManager->animation), 0x12, 0x16);
            }
            StartMenuAnimation(self, 0x65);
            int remaining;
            int trackCount = 0;
            char *cursor = reinterpret_cast<char *>(LoadResourceFile("musiccmt.txt",
                reinterpret_cast<unsigned int *>(&remaining), 0));
            music->textBuffer = reinterpret_cast<unsigned char *>(cursor);
            if (!cursor) goto Close;
            char *filename = music->filenames[0];
            char *title = music->titles[0];
            char *comment = music->comments[0][0];
            while (remaining > 0)
            {
                if (*cursor == '#' || *cursor != '@')
                    cursor = SkipMenuMusicLine(cursor, &remaining);
                else
                {
                    cursor = CopyMenuMusicLine(cursor + 1, &remaining, filename);
                    cursor = CopyMenuMusicLine(cursor, &remaining, title);
                    for (int line = 0; line < 8; ++line)
                    {
                        cursor = CopyMenuMusicLine(cursor, &remaining, comment);
                        comment += 0x42;
                    }
                    ++trackCount;
                    filename += 0x40;
                    title += 0x42;
                }
            }
            selection->count = trackCount;
            SelectFirst(selection);
            music->trackCount = trackCount;
            music->firstVisibleTrack = 0;
            for (int line = 0; line < 8; ++line)
                music->commentVmIds[line] = CreateAnimationVmId(g_MenuMusicCommentFile, line + 0x29, 0x16);
            music->nextCommentLine = 0;
            music->commentTrack = 0;
            music->warningPending = 0;
        }
        if (self->page.time.current < 10)
        {
            int track = self->page.time.current * 2 - 2;
            CameraVector position;
            position.x = 64.0f;
            position.y = static_cast<float>((96.0 - music->firstVisibleTrack * 20.0) +
                (self->page.time.current * 40 - 40));
            position.z = 0.0f;
            for (; track < self->page.time.current * 2; ++track)
            {
                if (track >= music->trackCount) break;
                music->trackVmIds[track] = CreateAnimationVmId(self->page.file, track + 0xab, 0x16);
                AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, music->trackVmIds[track]);
                if (!vm) music->trackVmIds[track] = AnimationVmId(0);
                if (g_SecondaryGameplayStatistics[0x2ddfa + track])
                    FormatTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                        0xffffff, 0, 0, 0, 0, music->titles[track]);
                else
                    FormatTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                        0xffffff, 0, 0, 0, 0, g_MenuMusicHiddenTitle, track + 1);
                SetTrackVisibility(music, vm, track);
                if (track == selection->current) position.x = static_cast<float>(position.x - 4.0);
                TweenMenuMusicPosition(vm, &position, MusicPosition(vm));
                if (track == selection->current) position.x = static_cast<float>(position.x + 4.0);
                position.y = static_cast<float>(position.y + 20.0);
                SetEvent(vm, static_cast<short>((track != selection->current) + 2));
            }
        }
        if (self->page.time.current >= 10)
        {
            self->page.state = 1;
            self->page.time.SetFrame(-1);
            return 0;
        }
        break;
    case 1:
        RefreshComment(self, music);
        if (self->page.time.current > 4)
        {
            SetConfigurationMenuState(&self->page, 2);
            return 0;
        }
        break;
    case 2:
    {
        RefreshComment(self, music);
        selection->previous = selection->current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(selection, -1);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(selection, 1);
        if (selection->previous != selection->current)
        {
            QueueMeshEffectSound(12);
            if (selection->current < music->firstVisibleTrack)
                music->firstVisibleTrack = selection->current;
            else if (selection->current >= music->firstVisibleTrack + 10)
                music->firstVisibleTrack = selection->current - 9;
            CameraVector position;
            position.x = 64.0f;
            position.y = static_cast<float>(96.0 - music->firstVisibleTrack * 20.0);
            position.z = 0.0f;
            for (int track = 0; track < music->trackCount; ++track)
            {
                AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, music->trackVmIds[track]);
                if (!vm) music->trackVmIds[track] = AnimationVmId(0);
                SetTrackVisibility(music, vm, track);
                if (track == selection->current) position.x = static_cast<float>(position.x - 4.0);
                float delta = MusicPosition(vm)->y - position.y;
                float distance = static_cast<float>(fabs(delta));
                if (distance < 40.0f) TweenMenuMusicPosition(vm, &position, MusicPosition(vm));
                else *MusicPosition(vm) = position;
                if (track == selection->current) position.x = static_cast<float>(position.x + 4.0);
                position.y = static_cast<float>(position.y + 20.0);
                SetEvent(vm, static_cast<short>((track != selection->current) + 2));
            }
            if (music->nextCommentLine >= 8) music->warningPending = 0;
        }
        if (g_InputState.pressed & 0x80001)
        {
            for (int line = 0; line < 8; ++line)
                SignalTree(FindAnimationVmById(g_RenderBatch, music->commentVmIds[line]), 3);
            music->commentTrack = selection->current;
            music->nextCommentLine = 0;
            self->page.time.SetFrame(-1);
            if (!g_SecondaryGameplayStatistics[0x2ddfa + music->commentTrack] && !music->warningPending)
            {
                if (reinterpret_cast<unsigned char *>(&g_MainRuntime)[0x200] & 0x10)
                    EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
                else EnqueueSoundCommand(&g_SoundManagerQueue, 3, 0, "dummy");
                music->warningPending = 1;
                return 0;
            }
            EnqueueMenuMusicLoad(0, music->filenames[selection->current]);
            if (reinterpret_cast<unsigned char *>(&g_MainRuntime)[0x200] & 0x10)
                EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
            EnqueueSoundCommand(&g_SoundManagerQueue, 2, 0, "dummy");
            g_SecondaryGameplayStatistics[0x2ddfa] = 1;
            music->warningPending = 0;
            return 0;
        }
        if (g_InputState.pressed & 0x102)
        {
        Close:
            if (music->textBuffer)
            {
                free(music->textBuffer);
                music->textBuffer = 0;
            }
            music->textBuffer = 0;
            PopConfigurationSelection(selection);
            for (int track = 0; track < music->trackCount; ++track)
                SignalSecondaryAnimation(&music->trackVmIds[track].value);
            for (int line = 0; line < 8; ++line)
            {
                unsigned int id = music->commentVmIds[line].value;
                if (id) SignalTree(reinterpret_cast<AnimationVmView *>(FindBoundVm(id)), 1);
            }
            QueueMeshEffectSound(11);
            self->page.state = 3;
            self->page.time.SetFrame(-1);
        }
        break;
    }
    case 3:
        if (self->page.time.current >= 10)
        {
            StopMenuAnimation(self, 0x65);
            StopMenuAnimation(self, 0x5d);
            SignalSecondaryAnimation(&MenuAnimationId(self, 0xc1).value);
            SetConfigurationMenuPage(&self->page, 1);
            EnqueueMenuMusicLoad(0, "bgm/th11_00.wav");
            EnqueueMenuMusicStart(0, 0);
            PopConfigurationSelection(selection);
        }
        break;
    }
    return 0;
}
