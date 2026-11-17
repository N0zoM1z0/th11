#include "StageDrawing.hpp"
#include "FrameLoop.hpp"

// Native 0x405E80..0x405EBD. The runtime owns cameras spaced 0x118 apart,
// beginning at offset 0x204. The private ESI/EBX call ABI is deliberately
// left to the original VC9 /GL optimizer, not implemented with assembly.
void __stdcall SelectRuntimeCamera(MainRuntimeView *self, int index)
{
    CameraView *camera = reinterpret_cast<CameraView *>(
        reinterpret_cast<unsigned char *>(self) + 0x204 + index * 0x118);
    FrameRuntimeView &runtime = *reinterpret_cast<FrameRuntimeView *>(self);
    runtime.activeCamera = camera;
    ApplyCamera(camera);
    static_cast<IDirect3DDevice9 *>(self->d3dDevice)->SetViewport(&camera->viewport);
    runtime.activeCameraIndex = index;
}
