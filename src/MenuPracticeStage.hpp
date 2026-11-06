#pragma once

#include "MenuController.hpp"

struct MenuPracticeStageView
{
    unsigned char unknown000[0x24];
    SecondaryDebugSelectionView selection;
};
typedef char MenuPracticeSelectionOffset[(offsetof(MenuPracticeStageView, selection) == 0x24) ? 1 : -1];

extern int g_MenuPracticeStageSelection;
extern int g_MenuStageShortcut;
extern unsigned char g_MenuStageShortcutKeys[256];

void __stdcall StartMenuTransitionOverlay(float x, float y);
int __stdcall EnqueueMenuMusicFade(float duration);

// Reviewed complete call boundary at455A70; its body remains source-open.
AnimationVmId __stdcall CreateDirectPositionAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script, int layer);
