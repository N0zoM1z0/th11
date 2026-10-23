// Non-runnable caller context retaining repeated hotkey processing.
#include "../src/Screenshot.hpp"

extern "C" void __stdcall ScreenshotHotkeyProbe()
{
    HandleScreenshotHotkey();
    HandleScreenshotHotkey();
}
