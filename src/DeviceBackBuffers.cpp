#include "DeviceInitialization.hpp"

static __forceinline void ClearAndPresentBackBuffer()
{
    DeviceRuntime().device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if (FAILED(DeviceRuntime().device->Present(0, 0, 0, 0)))
        DeviceRuntime().device->Reset(&DeviceRuntime().presentation);
}

void __cdecl PrimeDeviceBackBuffers()
{
    ClearAndPresentBackBuffer();
    ClearAndPresentBackBuffer();
}
