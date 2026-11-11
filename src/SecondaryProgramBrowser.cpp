#include "SecondaryProgramBrowser.hpp"
#include "InputBackend.hpp"
#include "SecondaryGameplay.hpp"
#include <stdlib.h>
#include <string.h>

namespace
{
const char ProgramFilePattern[] = "../../data/*.ecl";

__forceinline void InitializeSelection(SecondaryDebugSelectionView *selection, int count)
{
    selection->count = count;
    selection->wrapEnabled = 1;
    // This is the native zero/negative-count choice, not a bounds clamp.
    if (count == 0) selection->current = 0;
    else if (count < 1) selection->current = count - 1;
    else selection->current = 0;
}

__forceinline void Navigate(SecondaryDebugSelectionView *selection,
    unsigned int increase, unsigned int decrease)
{
    selection->previous = selection->current;
    if ((g_InputState.pressed & increase) || (g_InputState.repeat & increase))
        MoveSecondaryDebugSelection(selection, 1);
    if ((g_InputState.pressed & decrease) || (g_InputState.repeat & decrease))
        MoveSecondaryDebugSelection(selection, -1);
}
}

// Complete native 0x438570..0x4385C2 (83 bytes): release the previous
// enumeration without overwriting fileCount or adding an extra null guard.
void __stdcall ClearSecondaryProgramBrowserFiles(SecondaryProgramBrowserView *self)
{
    if (self->filenames)
    {
        for (int i = 0; i < self->fileCount; ++i)
        {
            if (self->filenames[i])
            {
                free(self->filenames[i]);
                self->filenames[i] = 0;
            }
        }
        if (self->filenames)
        {
            free(self->filenames);
            self->filenames = 0;
        }
        self->filenames = 0;
    }
}

// Complete native 0x438AC0..0x438E65 (934 bytes): ECL browser state machine.
// Preserve enumeration counts, each selection's input bit source, and the
// original management and early-exit order across modes 0, 1 and 4.
int __fastcall UpdateSecondaryProgramBrowser(SecondaryProgramBrowserView *self)
{
    switch (self->state)
    {
    case 0:
    {
        ClearSecondaryProgramBrowserFiles(self);
        WIN32_FIND_DATAA found;
        int count = 0;
        HANDLE handle = FindFirstFileA(ProgramFilePattern, &found);
        if (handle != INVALID_HANDLE_VALUE)
        {
            do { ++count; }
            while (FindNextFileA(handle, &found));
        }
        // The native calls FindClose even if FindFirstFile returned INVALID.
        FindClose(handle);
        self->fileCount = count;
        if (count)
        {
            self->filenames = static_cast<char **>(malloc(count * sizeof(char *)));
            handle = FindFirstFileA(ProgramFilePattern, &found);
            for (int index = 0; index < count; ++index)
            {
                self->filenames[index] = static_cast<char *>(malloc(strlen(found.cFileName) + 1));
                strcpy(self->filenames[index], found.cFileName);
                if (!FindNextFileA(handle, &found)) break;
            }
            FindClose(handle);
        }
        InitializeSelection(&self->category, 3);
        InitializeSelection(&self->fileSelection, count);
        InitializeSelection(&self->subroutineSelection, 1);
        self->state = 1;
        self->displayOrigin = 0.0f;
        self->displayTimer = 1000;
        self->displayScale = 32.0f;
        self->displayOffset = 0.0f;
        break;
    }
    case 1:
        Navigate(&self->category, 0x20, 0x10);
        switch (self->category.current)
        {
        case 0:
            Navigate(&self->fileSelection, 0x80, 0x40);
            if (g_InputState.pressed & 0x80001)
            {
                self->flags &= ~2u;
                StartSecondaryDebugThread(&self->worker, LoadSecondaryDebugScript, self);
                self->state = 2;
            }
            break;
        case 1:
            if (self->program)
            {
                Navigate(&self->subroutineSelection, 0x80, 0x40);
                if (g_InputState.released & 0x80001)
                {
                    if (self->manager)
                    {
                        delete self->manager;
                        self->manager = 0;
                    }
                    const char *name = self->program->subroutines[
                        self->subroutineSelection.current].name;
                    self->manager = CreateSecondaryProgramBrowserManager(self->program, name);
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
    case 4:
        UpdateSecondaryScripts(self->manager, 1.0f);
        if (self->manager)
        {
            if (self->manager->current->instruction)
                break;
            delete self->manager;
        }
        self->manager = 0;
        self->state = 1;
        break;
    }
    return 1;
}
