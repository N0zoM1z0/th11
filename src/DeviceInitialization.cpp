#include "DeviceInitialization.hpp"
#include "ErrorLog.hpp"
#include <string.h>

int __cdecl InitializeD3DDevice()
{
    DeviceInitializationView &state = DeviceRuntime();
    bool checkTextureFormat = true;
    D3DPRESENT_PARAMETERS presentation;
    memset(&presentation, 0, sizeof(presentation));
    D3DDISPLAYMODE desktop;
    state.d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &desktop);
    state.desktop = desktop;
    if (state.displayMode && desktop.RefreshRate != 60)
    {
        AppendLog(&g_RuntimeLog.buffer, g_DeviceLogRefreshNot60);
        state.presentationFlags &= ~0x10u;
    }
    if (!state.displayMode)
    {
        if (state.compatibilityOptions & 1)
        {
            presentation.BackBufferFormat = D3DFMT_R5G6B5;
            state.colorMode = 1;
        }
        else if (state.colorMode == 0xff)
        {
            presentation.BackBufferFormat = D3DFMT_X8R8G8B8;
            state.colorMode = 0;
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogInitialColor);
        }
        else
            presentation.BackBufferFormat = state.colorMode ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
        if (state.pathMismatch)
            state.disableRefreshChange = 1;
        if (!state.disableRefreshChange)
        {
            presentation.FullScreen_RefreshRateInHz = 60;
            if (state.presentationFlags & 0x10)
                presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            else
                presentation.PresentationInterval = state.timingMode == 3 ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_ONE;
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogTry60);
            presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
        }
        else
        {
            presentation.FullScreen_RefreshRateInHz = 0;
            presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
            presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogTryImmediate);
        }
    }
    else
    {
        presentation.BackBufferFormat = desktop.Format;
        presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
        if ((state.presentationFlags & 0x10) ||
            state.timingMode == 3 || desktop.RefreshRate != 60)
            presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        else
            presentation.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        presentation.Windowed = TRUE;
    }
    state.flags |= 2;
    presentation.BackBufferWidth = 640;
    presentation.BackBufferHeight = 480;
    presentation.EnableAutoDepthStencil = TRUE;
    presentation.AutoDepthStencilFormat = D3DFMT_D16;
    presentation.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    state.refreshChangeActive = 1;
    bool refreshRetried = false;
    for (;;)
    {
        if (!(state.compatibilityOptions & 2))
        {
            if (SUCCEEDED(state.d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                state.window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentation, &state.device)))
            {
                AppendLog(&g_RuntimeLog.buffer, g_DeviceLogHardware);
                state.flags |= 1;
                break;
            }
            if (refreshRetried)
                AppendLog(&g_RuntimeLog.buffer, g_DeviceLogHardwareUnavailable);
            if (SUCCEEDED(state.d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                state.window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &state.device)))
            {
                AppendLog(&g_RuntimeLog.buffer, g_DeviceLogSoftware);
                state.flags &= ~1u;
                break;
            }
            if (refreshRetried)
                AppendLog(&g_RuntimeLog.buffer, g_DeviceLogHalUnavailable);
        }
        if (SUCCEEDED(state.d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
            state.window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &state.device)))
        {
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogReference);
            state.flags &= ~1u;
            checkTextureFormat = false;
            break;
        }
        if (!state.disableRefreshChange)
        {
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogRefreshFailed);
            presentation.FullScreen_RefreshRateInHz = 0;
            state.refreshChangeActive = 0;
            refreshRetried = true;
            continue;
        }
        if (presentation.PresentationInterval == D3DPRESENT_INTERVAL_ONE)
        {
            presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            continue;
        }
        AppendErrorLog(&g_RuntimeLog, g_DeviceLogCreationFailed);
        if (state.d3d)
        {
            state.d3d->Release();
            state.d3d = 0;
        }
        return 1;
    }
    state.presentation = presentation;
    CameraVector eye;
    eye.x = 320.0f;
    eye.y = -240.0f;
    float distance = static_cast<float>(240.0 / CameraTangent(0.2617993950843811f));
    eye.z = -distance;
    CameraVector target = {320.0f, -240.0f, 0.0f};
    CameraVector up = {0.0f, 1.0f, 0.0f};
    D3DXMatrixLookAtLH(&state.view, &eye, &target, &up);
    D3DXMatrixPerspectiveFovLH(&state.projection, 0.5235987901687622f,
        4.0f / 3.0f, 100.0f, 10000.0f);
    state.device->SetTransform(D3DTS_VIEW, &state.view);
    state.device->SetTransform(D3DTS_PROJECTION, &state.projection);
    state.device->GetViewport(&state.viewport);
    state.device->GetDeviceCaps(&state.caps);
    if (!(state.caps.TextureOpCaps & D3DTEXOPCAPS_ADD))
        AppendLog(&g_RuntimeLog.buffer, g_DeviceLogNoAdd);
    if (state.caps.MaxTextureWidth <= 256)
        AppendLog(&g_RuntimeLog.buffer, g_DeviceLogSmallTextures);
    if (!(state.compatibilityOptions & 1) && checkTextureFormat)
    {
        if (state.d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
            presentation.BackBufferFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8) == D3D_OK)
            state.flags |= 4;
        else
        {
            state.flags &= ~4u;
            state.compatibilityOptions |= 1;
            AppendLog(&g_RuntimeLog.buffer, g_DeviceLogReducedColor);
        }
    }
    ResetDeviceRenderStates();
    ResetMainViewport();
    state.resetb0c = 0;
    state.reset584 = 0;
    return 0;
}
