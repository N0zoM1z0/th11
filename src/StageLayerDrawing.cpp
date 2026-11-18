#include "StageLayerDrawing.hpp"
#include "StageDrawing.hpp"
#include "AnimationImmediateDraw.hpp"
#include "FrameLoop.hpp"

// Native stage+2FE4/+2FE8/+2FEC are integer counters, although some other
// owners currently see the same twelve-byte region as a physical vector.
struct StageDrawCounters
{
    int acceptedObjects;
    int rejectedObjects;
    int drawnPrimitives;
};

struct StageLayerRendererFlag
{
    unsigned char unknown000[0x4355c4];
    unsigned char enabled;
};

struct StageSpriteDimensions
{
    unsigned char unknown000[0x34];
    float height, width;
};

static __forceinline StageDrawCounters &DrawCounters(StageResourceView *stage)
{
    return *reinterpret_cast<StageDrawCounters *>(&stage->drawOffset);
}

static __forceinline void SetLayerRenderState(D3DRENDERSTATETYPE state, DWORD value)
{
    FlushRenderBatch(g_RenderBatch);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(state, value);
}

// Complete target owner at 0x404190, including the variable-length primitive
// stream and the independent fog/Z state transitions.
int __stdcall DrawStageLayer(StageResourceView *stage, int layer)
{
    StageLayerRecord *entry = reinterpret_cast<StageLayerRecord *>(stage->section004);
    int zWritesEnabled = 1;

    FrameRuntime().activeCamera = reinterpret_cast<CameraView *>(
        reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x434);
    PerspectiveCameraView *camera = reinterpret_cast<PerspectiveCameraView *>(
        reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x434);
    ApplyPerspectiveCamera(camera);
    // Perspective application can replace the active camera. The native
    // reloads the published camera before choosing the viewport.
    PerspectiveCameraView *activeCamera = reinterpret_cast<PerspectiveCameraView *>(
        FrameRuntime().activeCamera);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetViewport(&activeCamera->viewport);
    FrameRuntime().activeCameraIndex = 2;
    reinterpret_cast<StageLayerRendererFlag *>(g_RenderBatch)->enabled = 1;

    for (; entry->objectIndex >= 0; ++entry)
    {
        StageObjectDrawView *object = reinterpret_cast<StageObjectDrawView *>(
            stage->objects[entry->objectIndex]);
        if (object->layer != layer)
            continue;

        // Native dispatch snapshots the layer offset into a local vector;
        // the culling helper observes the copy rather than the record.
        CameraVector offset;
        offset.x = entry->offset.x;
        offset.y = entry->offset.y;
        offset.z = entry->offset.z;
        if (CullStageObject(camera, object, &offset, stage->distanceSquared))
        {
            ++DrawCounters(stage).rejectedObjects;
            entry->flags &= ~1u;
            continue;
        }

        object->flags |= 2;
        StagePrimitiveDrawView *command = reinterpret_cast<StagePrimitiveDrawView *>(
            object->commands);
        while (command->type >= 0)
        {
            AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
                reinterpret_cast<unsigned char *>(stage->vms) +
                command->vmIndex * static_cast<int>(sizeof(AnimationVmView)));
            if (command->type == 0)
            {
                unsigned int vmFlags = vm->flags.value;
                if ((vmFlags & 0x03c00000) >= 0x01000000)
                {
                    float *position = reinterpret_cast<float *>(&vm->persistent3e8);
                    position[0] = static_cast<float>(command->offsetX + entry->offset.x);
                    position[1] = static_cast<float>(command->offsetY + entry->offset.y);
                    position[2] = static_cast<float>(command->offsetZ + entry->offset.z);

                    StageSpriteDimensions *sprite =
                        reinterpret_cast<StageSpriteDimensions *>(vm->unknown3ac);
                    if (command->scaleX != 0.0f)
                    {
                        const float scaleX = command->scaleX / sprite->width;
                        vmFlags |= 8;
                        vm->flags.value = vmFlags;
                        vm->scaleX = scaleX;
                    }
                    if (command->scaleY != 0.0f)
                    {
                        const float scaleY = command->scaleY / sprite->height;
                        vm->flags.value |= 8;
                        vm->scaleY = scaleY;
                    }
                }

                // The native splits enable and disable into separate gates,
                // each of which samples the current runtime fog state.
                if ((vm->flags.value & 0x03c00000) == 0x02000000)
                {
                    if (FrameRuntime().fogEnabled != 1)
                    {
                        FlushRenderBatch(g_RenderBatch);
                        FrameRuntime().fogEnabled = 1;
                        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->
                            SetRenderState(D3DRS_FOGENABLE, 1);
                    }
                }
                else if (FrameRuntime().fogEnabled != 0)
                {
                    FlushRenderBatch(g_RenderBatch);
                    FrameRuntime().fogEnabled = 0;
                    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->
                        SetRenderState(D3DRS_FOGENABLE, 0);
                }
                // The native extracts bit 11 with a full DWORD shift/mask,
                // keeping the result integer-valued until the Z state gates.
                const unsigned int disableZ = (vm->flags.value >> 11) & 1u;
                if (disableZ && zWritesEnabled)
                {
                    SetLayerRenderState(D3DRS_ZWRITEENABLE, FALSE);
                    zWritesEnabled = 0;
                }
                else if (!disableZ && !zWritesEnabled)
                {
                    SetLayerRenderState(D3DRS_ZWRITEENABLE, TRUE);
                    zWritesEnabled = 1;
                }
                DrawAnimationVmByMode(g_RenderBatch, vm);
                ++DrawCounters(stage).drawnPrimitives;
            }
            command = reinterpret_cast<StagePrimitiveDrawView *>(
                reinterpret_cast<unsigned char *>(command) + command->byteSize);
        }
        entry->flags |= 1;
        ++DrawCounters(stage).acceptedObjects;
    }

    if (!zWritesEnabled)
        SetLayerRenderState(D3DRS_ZWRITEENABLE, TRUE);
    return 0;
}
