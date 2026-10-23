#include "WindowInitialization.hpp"
#include <string.h>

int __stdcall InitializeMainWindow(HINSTANCE instance)
{
    WindowInitializationView &state = *reinterpret_cast<WindowInitializationView *>(&g_MainRuntime);
    WNDCLASSA windowClass;
    memset(&windowClass, 0, sizeof(windowClass));
    windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = MainWindowProc;
    state.active = 1;
    state.inactive = 0;
    windowClass.lpszClassName = g_MainWindowClassName;
    RegisterClassA(&windowClass);

    state.flags.bits.mode = state.displayMode;
    state.windowed = (state.flags.value & 0xc) != 0;
    if (!state.frameSkip && state.timingMode == 2)
        state.flags.value |= 0x10;
    else
        state.flags.value &= ~0x10u;
    state.triplets[0].value0 = 15;
    state.triplets[0].value4 = 15;
    state.triplets[0].value8 = 0;
    state.triplets[1].value0 = 12;
    state.triplets[1].value4 = 12;
    state.triplets[1].value8 = 0;
    state.triplets[2].value0 = 12;
    state.triplets[2].value4 = 12;
    state.triplets[2].value8 = 0;
    state.triplets[3].value0 = 8;
    state.triplets[3].value4 = 8;
    state.triplets[3].value8 = 0;
    state.resetb80 = 0;

    if (!state.windowed)
        state.mainWindow = CreateWindowExA(0, g_MainWindowClassName, g_MainWindowTitle,
            WS_POPUP | WS_VISIBLE, 0, 0, 640, 480, 0, 0, instance, 0);
    else
    {
        int width, height;
        unsigned int mode = state.flags.bits.mode;
        if (mode == 3)
        {
            width = GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 1280;
            height = GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + 960;
        }
        else if (mode == 2)
        {
            width = GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 960;
            height = GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + 720;
        }
        else
        {
            width = GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 640;
            height = GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + 480;
        }
        height += GetSystemMetrics(SM_CYCAPTION);
        state.mainWindow = CreateWindowExA(0, g_MainWindowClassName, g_MainWindowTitle,
            WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
            state.windowX, state.windowY, width, height, 0, 0, instance, 0);
    }
    GetWindowRect(state.mainWindow, &state.windowRect);
    state.windowAlias = state.mainWindow;
    if (!state.mainWindow)
        return 1;
    SendMessageA(state.mainWindow, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    Sleep(16);
    SendMessageA(state.mainWindow, WM_SYSCOMMAND, SC_RESTORE, 0);
    return 0;
}
