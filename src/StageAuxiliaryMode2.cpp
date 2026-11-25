#include "StageAuxiliary.hpp"
#include "StageMesh.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include <new>

// These are real, independently bounded TH11 functions; the original
// compiler's private register allocations are not source-level ABIs.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
void __stdcall EndSecondarySpecialState(int value);
int __stdcall UpdateStageAuxiliaryMode2Effect(StageAuxiliaryView *self);

// Full original 0x407D10..0x407FA8, including radial screen-space distortion,
// the mode-specific cleanup, and the VM/player position publication.
int __stdcall UpdateSecondarySpecialMode2(StageAuxiliaryView *self)
{
    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->animationId040));
    if (!vm)
        self->animationId040 = 0;
    EndSecondarySpecialState(40);

    if (!vm)
    {
        VmBindingBlockView *block = self->bindings;
        if (block)
        {
            ReleaseVmBindingBlock(block);
            operator delete(block);
        }
        self->bindings = 0;
        return -1;
    }

    const CameraVector &playerPosition = g_SecondaryPlayerControllerInstance->position;
    self->origin = playerPosition;
    PositionSecondaryAnimationVm(vm, &playerPosition);
    UpdateStageAuxiliaryMode2Effect(self);

    const float vmRadius = vm->scale.y;
    self->radius = vmRadius;
    StageMeshView *mesh = reinterpret_cast<StageMeshView *>(self->bindings);
    CameraVector *position = mesh->positions;

    // The native x87 owner computes radius + 0.5*radius before rounding to
    // float, then builds a square whose side is twice that radius plus 40.
    float radius = static_cast<float>(static_cast<double>(vmRadius) * 0.5 + vmRadius);
    CameraVector center = {
        static_cast<float>(static_cast<double>(self->origin.x) + 32.0 + 192.0),
        static_cast<float>(static_cast<double>(self->origin.y) + 16.0),
        self->origin.z
    };
    float diameter = static_cast<float>(radius + radius + 40.0);
    float left = static_cast<float>((self->origin.x - radius) - 20.0);
    float top = static_cast<float>((self->origin.y - radius) - 20.0);
    ResetStageMesh(mesh, left, top, diameter, diameter);

    RenderVertex *vertex = mesh->vertices;
    for (int row = 0; row < mesh->rows; ++row)
    {
        const double squaredRadius = static_cast<double>(radius) * radius;
        for (int column = 0; column < mesh->columns; ++column, ++position, ++vertex)
        {
            CameraVector direction = {
                static_cast<float>(position->x - center.x),
                static_cast<float>(position->y - center.y),
                static_cast<float>(position->z - center.z)
            };
            const float xySquared = direction.x * direction.x +
                direction.y * direction.y;
            const float remainder = static_cast<float>(squaredRadius - xySquared);
            // Native FCOM treats unordered/NaN as outside, too.
            if (remainder >= 0.0f)
            {
                const float weight = static_cast<float>(remainder / squaredRadius);
                vertex->color = 0xffff4040;
                const float magnitude = static_cast<float>(weight * 64.0);
                D3DXVec3Normalize(&direction, &direction);
                direction.x *= magnitude;
                direction.y *= magnitude;
                direction.z *= magnitude;
                vertex->x = static_cast<float>(vertex->x + direction.x);
                vertex->y = static_cast<float>(vertex->y + direction.y);
                vertex->z = 0.0f;
                position->z = 0.0f;
            }
            else
                reinterpret_cast<unsigned char *>(&vertex->color)[3] = 0;
        }
    }
    return 0;
}
