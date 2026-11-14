#include "PauseContinueMenu.hpp"
#include "MenuController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"
#include <string.h>

int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock);

namespace
{
inline void SetPauseSelection(SecondaryDebugSelectionView *selection, int value)
{
    if (!selection->count)
        selection->current = value;
    else
    {
        if (value >= selection->count)
            value = selection->count - 1;
        selection->current = value;
    }
}
inline void ResetPauseSelection(SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        selection->current = selection->count <= 0 ? selection->count - 1 : 0;
    else
        selection->current = 0;
}
}

// 0042D850. Record the score; on success open the name entry with the
// saved player name and the cursor after its last non-space character.
void __stdcall PreparePauseContinueRecords(PauseMenuView *self)
{
    if (g_StageVariant == 7 && self->saveMode)
    {
        g_SecondaryGameplaySelectionEC8 =
            reinterpret_cast<SecondaryGameplaySelectionView *>(g_MenuStageSelections + 0x40);
        g_StageVariant = 8;
        g_MenuPreviousVariant = 8;
    }
    unsigned char *scores = g_SecondaryGameplayStatistics + 8 +
        (g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1) * 0x68d4;
    int place = InsertPauseContinueScore(scores);
    if (g_StageVariant == 7 && self->saveMode)
    {
        g_SecondaryGameplaySelectionEC8 =
            reinterpret_cast<SecondaryGameplaySelectionView *>(g_MenuStageSelections);
        g_StageVariant = 7;
        g_MenuPreviousVariant = 7;
    }
    if (place >= 0)
    {
        self->selection.count = 25;
        self->selection.wrapEnabled = 1;
        SetPauseSelection(&self->selection, place);
        ResetPauseSelection(&self->keyboard);
        self->keyboard.count = 91;
        self->keyboard.wrapEnabled = 1;
        strcpy(self->name, reinterpret_cast<char *>(g_SecondaryGameplayStatistics + 0x2dde0));
        self->namePosition = 0;
        if (strcmp(self->name, "        "))
            MoveSecondaryDebugSelection(&self->keyboard, -1);
        int length = 8;
        do
        {
            if (self->name[length - 1] != ' ')
                break;
        } while (--length > 0);
        self->namePosition = length;
        self->unknown1f8 = 0;
    }
    else
        self->unknown1f8 = 1;
}
