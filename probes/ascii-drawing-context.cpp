// Non-runnable link context; all objects, strings and selectors are independent.
#include "AsciiDrawing.hpp"
#include "AsciiFormatting.hpp"

extern "C" int __stdcall AsciiDrawingProbe(AsciiManagerObject *first,
    AsciiManagerObject *second, int mode, AnimationVmView *vm, AnimationSpriteView *sprite)
{
    SetAnimationSpriteUv(vm, sprite);
    return DrawAsciiManager(first, mode) + DrawAsciiOverlay(second);
}

extern "C" __declspec(dllexport) void __stdcall AsciiPrintProbe(AsciiManagerView *self,
    const float *position, const char *format, int first, int second)
{
    AsciiManagerPrint(self, position, format, first, second);
    AsciiManagerPrintSmall(self, position, format, second, first);
}

extern "C" __declspec(dllexport) int __stdcall AsciiSpriteBindingProbe(
    AnimationDocumentView *file, int index, AnimationVmView *vm)
{
    return BindAnimationSprite(file, index, vm);
}


extern "C" __declspec(dllexport) void __stdcall AsciiFlushProbe(RenderBatchView *renderer)
{
    FlushRenderBatch(renderer);
}

extern "C" __declspec(dllexport) void __stdcall AsciiCameraProbe(PerspectiveCameraView *camera)
{
    ApplyPerspectiveCamera(camera);
}
