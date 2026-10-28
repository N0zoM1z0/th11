// Non-runnable context; selector, objects and arguments stay independent.
#include "StageMesh.hpp"
#include "MeshEffects.hpp"
#include <new>

extern "C" __declspec(dllexport) int __stdcall MeshEffectsProbe(int kind, void *self,
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
    case 7: return InitializeColumnMeshEffect(static_cast<MeshEffectControllerView *>(self));
    case 8: return InitializeSquareMeshEffect(static_cast<MeshEffectControllerView *>(self));
    case 9: return SquareMeshVmCallback(static_cast<AnimationVmView *>(self));
    }
    return 0;
}

extern "C" __declspec(dllexport) AnimationVmId __stdcall PositionedVmProbe(
    AnimationFileView *file, const CameraVector *position, int script, int layer)
{
    return CreatePositionedAnimationVmId(file, position, script, layer);
}

extern "C" __declspec(dllexport) unsigned int __stdcall RandomProbe(RandomStateView *a,
                                              RandomStateView *b, int wide)
{
    if (wide)
    {
        unsigned int first = RandomNext32(a);
        return first ^ RandomNext32(b);
    }
    unsigned int first = RandomNext16(a);
    return first ^ RandomNext16(b);
}

extern "C" __declspec(dllexport) void __stdcall SoundQueueProbe(int sound)
{
    QueueMeshEffectSound(sound);
}
