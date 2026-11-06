#pragma once

#include "MenuController.hpp"

extern int g_MenuSavedPartnerSelection;

// Reviewed mutable-ID lookup is separate from this read-only visibility input.
void __stdcall HideMenuAnimationTree(const AnimationVmId &id);
