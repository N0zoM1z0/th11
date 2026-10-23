// Non-runnable callback address escape, matching WNDCLASSA registration.
#include "../src/MainWindow.hpp"

extern "C" void __stdcall MainWindowProbe(WNDCLASSA *windowClass)
{
    windowClass->lpfnWndProc = MainWindowProc;
}
