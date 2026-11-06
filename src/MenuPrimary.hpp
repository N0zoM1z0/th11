#pragma once

#include "MenuController.hpp"

// Native indexed ID storage starts at2C4; only observed indices are used by
// maintained callers. This does not declare the original array capacity.
__forceinline AnimationVmId &MenuAnimationId(MenuControllerView *self, int script)
{
    return *reinterpret_cast<AnimationVmId *>(
        reinterpret_cast<unsigned char *>(self) + 0x2c4 + script * 4);
}

extern int g_MenuPreferredDifficulty;
void __stdcall StartMenuAnimation(MenuControllerView *self, int script);
void __stdcall StopMenuAnimation(MenuControllerView *self, int script);
void __stdcall SignalLockedMenuEntry(MenuControllerView *self, int script);
int __cdecl IsMenuExtraUnlocked();
int __fastcall SelectMenuItem(int value, SecondaryDebugSelectionView *selection);
