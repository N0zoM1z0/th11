#include "SecondaryGameplay.hpp"
#include <stdlib.h>
#include <string.h>

namespace
{
struct DebugInputBackupView
{
    unsigned char unknown000[0x118];
    unsigned int current, previous, unknown120, pressed;
};
__forceinline void ResumeChains(void *object)
{
    SecondaryGameplayChainsView *self = reinterpret_cast<SecondaryGameplayChainsView *>(object);
    if (self->update) self->update->flags &= ~2u;
    if (self->draw) self->draw->flags &= ~2u;
}
__forceinline void ResetSelection(SecondaryDebugSelectionView *self, int count)
{
    self->count = count;
    self->wrapEnabled = 1;
    // Preserve the observed zero/negative-count reset rule.
    if (self->count == 0) self->current = 0;
    else if (self->count < 1)
        self->current = SecondaryHostInstructions::AddInteger(self->count, -1);
    else self->current = 0;
}
}

int __fastcall UpdateSecondaryEclDebug(SecondaryEclDebugView *self)
{
    switch (self->state)
    {
    case 0:
    {
        ResetSecondaryDebugSession();
        WIN32_FIND_DATAA data;
        int count = 0;
        HANDLE find = FindFirstFileA("../../data/*.ecl", &data);
        if (find != INVALID_HANDLE_VALUE)
        {
            do { count = SecondaryHostInstructions::AddInteger(count, 1); }
            while (FindNextFileA(find, &data));
        }
        FindClose(find);
        self->fileCount = count;
        if (count != 0)
        {
            self->filenames = static_cast<char **>(malloc(static_cast<unsigned int>(count) * sizeof(char *)));
            find = FindFirstFileA("../../data/*.ecl", &data);
            for (int index = 0; index < count; ++index)
            {
                self->filenames[index] = static_cast<char *>(malloc(strlen(data.cFileName) + 1));
                strcpy(self->filenames[index], data.cFileName);
                if (!FindNextFileA(find, &data)) break;
            }
            FindClose(find);
        }
        ResetSelection(&self->selections[0], 3);
        ResetSelection(&self->selections[1], count);
        ResetSelection(&self->selections[2], 1);
        self->state = 1;
        self->value6fc = 0.0f;
        self->value708 = 1000;
        self->value700 = 32.0f;
        self->value704 = 0.0f;
        break;
    }
    case 1:
    {
        SecondaryDebugSelectionView *selection = &self->selections[0];
        selection->previous = selection->current;
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(selection, 1);
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(selection, -1);
        switch (selection->current)
        {
        case 0:
            selection = &self->selections[1];
            selection->previous = selection->current;
            if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
                MoveSecondaryDebugSelection(selection, 1);
            if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
                MoveSecondaryDebugSelection(selection, -1);
            if (g_InputState.pressed & 0x80001)
            {
                DeleteSecondaryDebugHud();
                DeleteSecondaryDebugPlayer();
                DeleteSecondaryDebugProjectiles();
                DeleteSecondaryDebugEnemy();
                DeleteSecondaryDebugItems();
                DeleteSecondaryDebugChainsEC0();
                DeleteSecondaryMaster();
                self->flags &= ~2u;
                StartSecondaryDebugThread(&self->worker, LoadSecondaryDebugScript, self);
                self->state = 2;
            }
            break;
        case 1:
            if (g_SecondaryMaster)
            {
                selection = &self->selections[2];
                selection->previous = selection->current;
                if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
                    MoveSecondaryDebugSelection(selection, 1);
                if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
                    MoveSecondaryDebugSelection(selection, -1);
                if (g_InputState.released & 0x80001)
                {
                    PauseSecondaryGameplayPlayer();
                    ResetSecondaryGameplayPlayer();
                    PauseSecondaryGameplayObject(g_SceneClearPool);
                    ResetSecondaryGameplayProjectiles();
                    PauseSecondaryGameplayActors();
                    ResetSecondaryGameplayActors(g_SecondaryMaster);
                    InitializeSecondaryGameplayHud();
                    PauseSecondaryGameplayObject(g_SecondaryAttractionItems);
                    ResetSecondaryGameplayItems();
                    SecondaryHostSpawnArguments arguments;
                    memset(&arguments, 0, sizeof(arguments));
                    arguments.position.x = 0.0f;
                    arguments.position.y = 0.0f;
                    arguments.position.z = 0.0f;
                    arguments.value014 = 10000;
                    const char *script = g_SecondaryMaster->program->subroutines[selection->current].name;
                    CreateSecondaryHostActor(script, &arguments);
                    self->state = 4;
                }
            }
            break;
        case 2:
            if (g_InputState.pressed & 0x80001)
                SetSecondaryGameplayRuntimeMode(&g_MainRuntime, 3);
            break;
        }
        break;
    }
    case 4:
    {
        DebugInputBackupView &backup = *reinterpret_cast<DebugInputBackupView *>(&g_InputState);
        unsigned int previous = backup.current;
        backup.current = g_InputState.current;
        backup.previous = previous;
        UpdateSecondaryDebugInput(&g_InputState);
        backup.pressed = g_InputState.pressed;
        if (g_InputState.pressed & 0x100)
        {
            ResumeChains(g_SecondaryActorReferences);
            ResetSecondaryGameplayPlayer();
            ResumeChains(g_SceneClearPool);
            ResetSecondaryGameplayProjectiles();
            SecondaryMasterView *master = g_SecondaryMaster;
            ResumeChains(master);
            ResetSecondaryGameplayActors(master);
            SecondaryAttractionManagerView *items = g_SecondaryAttractionItems;
            ResumeChains(items);
            memset(reinterpret_cast<unsigned char *>(items) + 0x14, 0, 0x265e50);
            self->state = 1;
        }
        break;
    }
    }
    return 1;
}
