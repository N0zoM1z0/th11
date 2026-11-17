#include "StageAuxiliary.hpp"
#include "StageMesh.hpp"
#include "Random.hpp"
#include "SecondaryActorRuntime.hpp"
#include <new>

// The 212-byte game-specific collision/effect postpass at 0x406F90 has its
// own separate owner. Its private register ABI is not declared in C++.
int __stdcall UpdateStageAuxiliaryMode3Effect(StageAuxiliaryView *self);

namespace
{
// The target uses FILD of the random 32-bit integer, adds 2^32 for a
// negative signed sample, then computes (2 * sample/2^32 - 1) before
// rounding once to float. A second float store rounds the 8x jitter.
static __forceinline float NextMeshOffset()
{
    const unsigned int sample = RandomNext32(&g_TextRandomState);
    const float centered = static_cast<float>(
        static_cast<double>(sample) * (2.0 / 4294967296.0) - 1.0);
    return static_cast<float>(centered * 8.0);
}
}

// Native 0x406D10..0x406E73 (356 bytes): live-VM gate, two independently
// jittered adjacent rows in the mesh, then a separate collision postpass.
int __stdcall UpdateSecondarySpecialMode3(StageAuxiliaryView *self)
{
    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->animationId040));
    if (!vm)
    {
        self->animationId040 = 0;
        return -1;
    }

    if (self->timer014.current > 90)
    {
        SignalSecondaryAnimation(&self->animationId040);
        VmBindingBlockView *block = self->bindings;
        if (block)
        {
            ReleaseVmBindingBlock(block);
            operator delete(block);
        }
        self->bindings = 0;
        return -1;
    }

    StageMeshView *mesh = reinterpret_cast<StageMeshView *>(self->bindings);
    RenderVertex *vertex = mesh->vertices;
    for (int column = 0; column < mesh->columns - 1; ++column, ++vertex)
    {
        const int count = mesh->columns;
        const CameraVector *source = mesh->positions;
        // Target updates x alone, including the two independent RNG draws.
        vertex[count].x = static_cast<float>(
            source[count + column].x + NextMeshOffset());

        const int otherCount = mesh->columns;
        source = mesh->positions;
        vertex[2 * otherCount].x = static_cast<float>(
            source[2 * otherCount + column].x + NextMeshOffset());
    }
    UpdateStageAuxiliaryMode3Effect(self);
    return 0;
}
