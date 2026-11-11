#include "PauseContinueMenu.hpp"
#include "MenuController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"
#include <string.h>

int __stdcall InsertPauseContinueScore(unsigned char *scoreBlock);

namespace
{
__forceinline void ChooseRecordItem(int value, SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        selection->current = value >= selection->count ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) :
            value < 0 ? 0 : value;
    else
        selection->current = value;
}
}

// 0042D850: temporarily select the extra-stage score bank for a saved run,
// then initialize the record-name keyboard if the new score enters the table.
void __stdcall PreparePauseContinueRecords(PauseMenuView *self)
{
    if (g_StageVariant == 7 && self->saveMode)
    {
        g_SecondaryGameplaySelectionEC8 =
            reinterpret_cast<SecondaryGameplaySelectionView *>(g_MenuStageSelections + 0x40);
        g_StageVariant = 8;
        g_MenuPreviousVariant = 8;
    }

    const unsigned int character = static_cast<unsigned int>(g_MeshEffectSelection0) * 3u +
        g_MeshEffectSelection1;
    int place = InsertPauseContinueScore(
        g_SecondaryGameplayStatistics + character * 0x68d4u + 8);

    // The insertion call can change globals: this is intentionally a second
    // test, not an unconditional restoration of the initial condition.
    if (g_StageVariant == 7 && self->saveMode)
    {
        g_SecondaryGameplaySelectionEC8 =
            reinterpret_cast<SecondaryGameplaySelectionView *>(g_MenuStageSelections);
        g_StageVariant = 7;
        g_MenuPreviousVariant = 7;
    }

    if (place < 0)
    {
        self->unknown1f8 = 1;
        return;
    }

    self->selection.count = 25;
    self->selection.wrapEnabled = 1;
    ChooseRecordItem(place, &self->selection);
    ChooseRecordItem(0, &self->keyboard);
    self->keyboard.count = 91;
    self->keyboard.wrapEnabled = 1;

    strcpy(self->name, reinterpret_cast<char *>(g_SecondaryGameplayStatistics) + 0x2dde0);
    self->namePosition = 0;
    if (strcmp(self->name, "        "))
        MoveSecondaryDebugSelection(&self->keyboard, -1);

    int length = 8;
    while (length > 0 && self->name[length - 1] == ' ')
        --length;
    self->namePosition = length;
    self->unknown1f8 = 0;
}
