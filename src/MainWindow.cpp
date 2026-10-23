#include "MainWindow.hpp"

static __forceinline WindowRuntimeView &WindowRuntime()
{
    return *reinterpret_cast<WindowRuntimeView *>(&g_MainRuntime);
}

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        g_MainRuntime.flags = (g_MainRuntime.flags & ~0x100u) | 0x80;
        return 1;
    case WM_ERASEBKGND:
        return 1;
    case WM_SIZE:
        if (WindowRuntime().resizeFlags & 1)
        {
            switch (wParam)
            {
            case SIZE_MAXIMIZED:
                WindowRuntime().resizeFlags = (WindowRuntime().resizeFlags & ~0xcu) | 2;
                break;
            }
        }
        break;
    case WM_ACTIVATEAPP:
        WindowRuntime().active = wParam;
        WindowRuntime().inactive = wParam == 0;
        break;
    case WM_SETCURSOR:
        if (!WindowRuntime().windowed)
        {
            if (WindowRuntime().inactive)
            {
                SetCursor(LoadCursorA(0, IDC_ARROW));
                do {} while (ShowCursor(TRUE) < 0);
            }
            else
            {
                while (ShowCursor(FALSE) >= 0) {}
                SetCursor(0);
            }
        }
        else
        {
            SetCursor(LoadCursorA(0, IDC_ARROW));
            ShowCursor(TRUE);
        }
        return 1;
    case WM_SYSCOMMAND:
        switch (wParam & 0xfff0)
        {
        case SC_MOUSEMENU:
        case SC_KEYMENU:
            return 1;
        }
        break;
    case WM_LBUTTONDOWN:
        SetForegroundWindow(window);
        break;
    }
    return DefWindowProcA(window, message, wParam, lParam);
}
