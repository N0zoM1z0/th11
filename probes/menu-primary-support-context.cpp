#include "MenuPrimary.hpp"

// Compile-only arbitrary inputs preserve the shared helpers' real interfaces.
extern "C" __declspec(dllexport) int __stdcall MenuPrimarySupportProbe(
    MenuControllerView *menu, int script, int value,
    SecondaryDebugSelectionView *selection)
{
    StartMenuAnimation(menu, script);
    StopMenuAnimation(menu, script);
    SignalLockedMenuEntry(menu, script);
    return SelectMenuItem(value, selection) + IsMenuExtraUnlocked();
}
