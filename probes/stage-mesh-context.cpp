#include "StageMesh.hpp"
#include <new>

extern "C" __declspec(dllexport) int __stdcall StageMeshProbe(int kind, void *self,
    int rows, int columns, int flags, unsigned int *id, float x, float y, float width, float height)
{
    StageMeshView *mesh = static_cast<StageMeshView *>(self);
    switch (kind)
    {
    case 0: new (self) StageScriptMesh(rows, columns, flags); break;
    case 1: ResetStageMesh(mesh, x, y, width, height); break;
    case 2: PublishStageMesh(mesh); break;
    case 3: return StageMeshVmCallback(static_cast<AnimationVmView *>(self));
    case 4: return reinterpret_cast<int>(FindAnimationVmById(reinterpret_cast<RenderBatchView *>(self), flags));
    case 5: *id = CreateStageMeshRow(columns).value; break;
    case 6: *id = CreateAlternateStageMeshRow(columns).value; break;
    }
    return 0;
}
