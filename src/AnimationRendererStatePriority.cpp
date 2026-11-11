#include "AnimationRendererDispatch.hpp"
#include "FrameLoop.hpp"
#include "RenderBatch.hpp"
#include "Camera.hpp"

namespace
{
// The three camera slots are 0x118 apart even though the public CameraView
// occupies fewer bytes. Their root-relative offsets are 204/31C/434.
struct PriorityCameraSlot
{
    CameraView camera;
    unsigned char reserved[0x118 - sizeof(CameraView)];
};
struct PriorityRuntimeView
{
    unsigned char unknown000[0x204];
    PriorityCameraSlot cameras[3];
    CameraView *activeCamera;
    int activeCameraIndex;
    unsigned char unknown554[0x990 - 0x554];
    int fogEnabled;
};
typedef char PriorityCameraSlotSize[(sizeof(PriorityCameraSlot) == 0x118) ? 1 : -1];
typedef char PriorityCameraOffset[(offsetof(PriorityRuntimeView, cameras) == 0x204) ? 1 : -1];
typedef char PriorityCurrentCameraOffset[(offsetof(PriorityRuntimeView, activeCamera) == 0x54c) ? 1 : -1];
typedef char PriorityFogOffset[(offsetof(PriorityRuntimeView, fogEnabled) == 0x990) ? 1 : -1];

__forceinline PriorityRuntimeView *State()
{
    return reinterpret_cast<PriorityRuntimeView *>(&g_MainRuntime);
}
__forceinline IDirect3DDevice9 *Device()
{
    return static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice);
}
__forceinline void UseCamera(int index)
{
    PriorityRuntimeView *runtime = State();
    runtime->activeCamera = &runtime->cameras[index].camera;
    ApplyCamera(&runtime->cameras[index].camera);
    Device()->SetViewport(&runtime->activeCamera->viewport);
    runtime->activeCameraIndex = index;
}
}

// Native 455170: stage camera and conditional fog-cache reset before layer 3.
int __fastcall AnimationRendererDrawPriority10(void *context)
{
    UseCamera(2);
    if (State()->fogEnabled)
    {
        FlushRenderBatch(g_RenderBatch);
        State()->fogEnabled = 0;
        Device()->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    return DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), 3);
}

// Native 455320: clear stage-camera and batch XY offsets before layer 19.
int __fastcall AnimationRendererDrawPriority40(void *context)
{
    State()->cameras[2].camera.offsetX = 0.0f;
    State()->cameras[2].camera.offsetY = 0.0f;
    g_RenderBatch->cameraOffsetX = 0.0f;
    g_RenderBatch->cameraOffsetY = 0.0f;
    return DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), 19);
}

// Native 455350 and 4553A0: screen camera and ordinary draw groups.
int __fastcall AnimationRendererDrawPriority48(void *context)
{
    UseCamera(1);
    return DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), 20);
}
int __fastcall AnimationRendererDrawPriority63(void *context)
{
    UseCamera(1);
    return DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), 29);
}

// Native 4553F0: draw alternate camera group and restore the screen camera.
// The first draw's status survives the second camera setup.
int __fastcall AnimationRendererDrawPriority62(void *context)
{
    UseCamera(0);
    int status = DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), 30);
    UseCamera(1);
    return status;
}
