#pragma once

#include "MenuController.hpp"
#include "InputBackend.hpp"
#include "VmBindings.hpp"

// Physical overlay of the same menu; its three adjacent selection objects are
// distinct. No complete original class or record allocation is claimed.
struct MenuPlayerDataView
{
    unsigned char unknown000[0x24];
    SecondaryDebugSelectionView character, difficulty, recordPage;
    int displayedRows;
    unsigned char unknown2b0[0x5cc - 0x2b0];
    AnimationVmId rowVmIds[10];
};

struct MenuSpellRecordView
{
    char name[0x84]; // Observed text-to-counter span; original capacity unknown.
    int firstCount, secondCount;
    unsigned int unknown08c;
};

typedef char MenuPlayerDataCharacterOffset[(offsetof(MenuPlayerDataView, character) == 0x24) ? 1 : -1];
typedef char MenuPlayerDataDifficultyOffset[(offsetof(MenuPlayerDataView, difficulty) == 0xfc) ? 1 : -1];
typedef char MenuPlayerDataPageOffset[(offsetof(MenuPlayerDataView, recordPage) == 0x1d4) ? 1 : -1];
typedef char MenuPlayerDataRowsOffset[(offsetof(MenuPlayerDataView, displayedRows) == 0x2ac) ? 1 : -1];
typedef char MenuPlayerDataIdsOffset[(offsetof(MenuPlayerDataView, rowVmIds) == 0x5cc) ? 1 : -1];
typedef char MenuSpellRecordStride[(sizeof(MenuSpellRecordView) == 0x90) ? 1 : -1];
typedef char MenuSpellRecordFirstOffset[(offsetof(MenuSpellRecordView, firstCount) == 0x84) ? 1 : -1];

extern signed char g_MenuSpellDifficulties[175];
extern unsigned char g_MenuPlayerDataKeyboardCurrent[256];
extern unsigned char g_MenuPlayerDataKeyboardPrevious[256];
extern unsigned char g_MenuPlayerDataKeyboardPressed[256];
extern unsigned int g_MenuPlayerDataSequencePosition;
extern int g_MenuPlayerDataSequenceFrames;
extern const int g_MenuPlayerDataSequenceKeys[12];
extern const char g_MenuPlayerDataHiddenRowFormat[];
// Shared text animation file, also used for music-room comments.
extern AnimationFileView *g_MenuMusicCommentFile;

int __stdcall RefreshMenuPlayerDataRecords(MenuControllerView *self);
int __stdcall CountMenuSpellsForDifficulty(int difficulty);
void __cdecl UnlockMenuPlayerDataEntries();
int __stdcall ReadMenuKeyboardSnapshot(unsigned char *keys);

static __forceinline AnimationVmView *FindMenuPlayerDataVm(
    BoundVmListsView *lists, unsigned int id)
{
    if (!id) return 0;
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}
