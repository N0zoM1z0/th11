#pragma once

#include "MenuController.hpp"

static __forceinline AnimationVmId &MenuDifficultyFooterId(MenuControllerView *self)
{
    return *reinterpret_cast<AnimationVmId *>(
        reinterpret_cast<unsigned char *>(self) + 0x5c8);
}

void __stdcall ExecuteMenuAnimationEvent3(MenuControllerView *self, int script);
