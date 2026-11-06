#include "MenuController.hpp"

// Compile-only real callback route and independent renderer/file inputs.
extern "C" int __stdcall MenuControllerProbe(MenuControllerView *self)
{
    return MenuControllerUpdateCallback(self);
}

extern "C" __declspec(dllexport) void __stdcall MenuFileMarkerProbe(
    RenderBatchView *renderer, AnimationFileView *file)
{
    MarkMenuFileAnimations(renderer, file);
}
