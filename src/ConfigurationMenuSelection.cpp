#include "ConfigurationAudioMenu.hpp"

namespace
{
// Complete physical storage used by the reviewed selection operations. The
// original class identity and the menu's other embedded fields remain open.
struct SelectionStorageView
{
    int current, previous, count;
    int savedCurrent[16], savedCount[16];
    int depth;
    int excluded[16];
    int wrapEnabled, excludedCount;
};
typedef char SelectionStorageSize[(sizeof(SelectionStorageView) == sizeof(SecondaryDebugSelectionView)) ? 1 : -1];
typedef char SelectionDepthOffset[(offsetof(SelectionStorageView, depth) == 0x8c) ? 1 : -1];
typedef char SelectionExcludedOffset[(offsetof(SelectionStorageView, excluded) == 0x90) ? 1 : -1];
}

int __stdcall MoveSecondaryDebugSelection(SecondaryDebugSelectionView *selection, int amount)
{
    SelectionStorageView *self = reinterpret_cast<SelectionStorageView *>(selection);
    int count = self->count;
    if (count > 0)
    {
        int excludedCount = self->excludedCount;
    Advance:
        self->current = static_cast<int>(static_cast<unsigned int>(self->current) + amount);
        while (self->current >= count)
        {
            if (self->wrapEnabled) self->current -= count;
            else self->current = count - 1;
        }
        while (self->current < 0)
        {
            if (self->wrapEnabled) self->current += count;
            else self->current = 0;
        }
        for (int i = 0; i < excludedCount; ++i)
            if (self->excluded[i] == self->current) goto Advance;
    }
    return self->current;
}

void __stdcall PushConfigurationSelection(SecondaryDebugSelectionView *selection)
{
    SelectionStorageView *self = reinterpret_cast<SelectionStorageView *>(selection);
    self->savedCurrent[self->depth] = self->current;
    self->savedCount[self->depth] = self->count;
    ++self->depth;
    self->excludedCount = 0;
    if (self->depth >= 16) self->depth = 15;
}

void __stdcall PopConfigurationSelection(SecondaryDebugSelectionView *selection)
{
    SelectionStorageView *self = reinterpret_cast<SelectionStorageView *>(selection);
    --self->depth;
    if (self->depth < 0) self->depth = 0;
    self->current = self->savedCurrent[self->depth];
    self->count = self->savedCount[self->depth];
    self->excludedCount = 0;
}
