#include "MenuPlayerData.hpp"
#include "FormattedTextureText.hpp"
#include "SecondaryActorRuntime.hpp"
#include <string.h>

int __stdcall RefreshMenuPlayerDataRecords(MenuControllerView *self)
{
    MenuPlayerDataView *data = reinterpret_cast<MenuPlayerDataView *>(self);
    int spell = 0, skipped = 0;
    while (skipped < data->recordPage.current * 10 - 10)
    {
        if (g_MenuSpellDifficulties[spell] == data->difficulty.current) ++skipped;
        ++spell;
    }
    AnimationVmId *id = data->rowVmIds;
    data->displayedRows = 0;
    int row = 0;
    char name[168];
    for (; row < 10; ++row, ++id)
    {
        while (spell < 175 && g_MenuSpellDifficulties[spell] != data->difficulty.current)
            ++spell;
        if (spell >= 175) break;
        MenuSpellRecordView *record = reinterpret_cast<MenuSpellRecordView *>(
            g_SecondaryGameplayStatistics + 0x27b64 + spell * 0x90);
        if (record->firstCount)
        {
            strcpy(name, record->name);
            int length = static_cast<int>(strlen(name));
            if (length < 42)
            {
                int padding = 42 - length;
                memset(name + length, ' ', padding);
                length += padding;
            }
            const int *counts = reinterpret_cast<const int *>(g_SecondaryGameplayStatistics +
                data->character.current * 0x68d4 + spell * 0x90 + 0x6ec);
            name[length] = 0;
            AnimationVmView *vm = FindMenuPlayerDataVm(
                reinterpret_cast<BoundVmListsView *>(g_RenderBatch), id->value);
            if (!vm) *id = AnimationVmId(0);
            FormatCenteredTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                counts[0] ? 0x00ffff80 : 0x00efefef, 0, 0, 0,
                "No.%3d %s %4d/%4d", ++spell, name, counts[0], counts[1]);
        }
        else
        {
            const int *counts = reinterpret_cast<const int *>(g_SecondaryGameplayStatistics +
                data->character.current * 0x68d4 + spell * 0x90 + 0x6ec);
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, *id);
            if (!vm) *id = AnimationVmId(0);
            FormatCenteredTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
                0x808080, 0, 0, 0, g_MenuPlayerDataHiddenRowFormat,
                ++spell, counts[0], counts[1]);
        }
        ++data->displayedRows;
    }
    for (; row < 10; ++row, ++id)
    {
        AnimationVmView *vm = FindMenuPlayerDataVm(
            reinterpret_cast<BoundVmListsView *>(g_RenderBatch), id->value);
        if (!vm) *id = AnimationVmId(0);
        FormatCenteredTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
            0xffffffff, 0, 0, 0, " ");
    }
    return 0;
}
