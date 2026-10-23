#include "RuntimeSurfaces.hpp"

void __stdcall EnsureRuntimeSurfaces(MainRuntimeView *self)
{
    RuntimeSurfaceOwnerView &runtime = *reinterpret_cast<RuntimeSurfaceOwnerView *>(self);
    if (runtime.primarySurface)
        return;
    runtime.animation->textures[2].texture->GetSurfaceLevel(0, &runtime.primarySurface);
    BindAnimationScript(runtime.animation, 81, runtime.primaryVm);
    if (!runtime.primarySurface)
        runtime.cameras[2] = runtime.cameras[0];
    runtime.animation->textures[3].texture->GetSurfaceLevel(0, &runtime.secondarySurface);
    BindAnimationScript(runtime.animation, 82, runtime.secondaryVm);
    BindAnimationScript(runtime.animation, 81, runtime.overlayVm);
    runtime.device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &runtime.backBuffer);
}

void __cdecl ReleaseRuntimeSurfaces()
{
    RuntimeSurfaceOwnerView &runtime = *reinterpret_cast<RuntimeSurfaceOwnerView *>(&g_MainRuntime);
    if (runtime.primarySurface)
    {
        runtime.primarySurface->Release();
        runtime.primarySurface = 0;
    }
    if (runtime.secondarySurface)
    {
        runtime.secondarySurface->Release();
        runtime.secondarySurface = 0;
    }
    if (runtime.backBuffer)
    {
        runtime.backBuffer->Release();
        runtime.backBuffer = 0;
    }
    runtime.primarySurface = 0;
}
