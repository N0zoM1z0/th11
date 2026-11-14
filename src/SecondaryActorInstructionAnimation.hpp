#pragma once
#include "SecondaryActorInstructions.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryEmitterAnimation.hpp"
#include "AnimationProjectedQuad.hpp"
#include "MeshEffects.hpp"

struct SecondaryHostSpawnParameters { int values[12]; };
struct SecondaryHostSpawnArguments
{
    CameraVector position;
    int value00c, value010, value014, mirrored;
    unsigned int flags;
    SecondaryHostSpawnParameters parameters;
};
struct SecondaryHostAnimationSelection
{
    int file, savedFile, savedScript;
};
struct SecondaryHostAnimationMetrics
{
    unsigned char unknown000[0x4c];
    float value4c, value50;
};
// The observed first camera starts at renderer+204; the rest of the renderer
// and the original camera-container type remain outside this physical view.
struct SecondaryHostProjectionRenderer
{
    unsigned char unknown000[0x204];
    CameraView camera;
};
extern SecondaryHostProjectionRenderer g_SecondaryHostProjectionRenderer;
extern D3DMATRIX g_SecondaryHostProjectionWorld;

SecondaryActorManagerView *__stdcall CreateSecondaryHostActor(const char *script,
    const SecondaryHostSpawnArguments *arguments);
int __stdcall ReadSecondaryHostIntegerValue(SecondaryActorStateView *self, int index, int value);
float __stdcall ReadSecondaryHostRealValue(SecondaryActorStateView *self, int index, float value);
void __stdcall SelectSecondaryHostCamera(SecondaryHostProjectionRenderer *self, int index);
const CameraVector *__stdcall OffsetSecondaryHostPosition(const CameraVector *position);
void __stdcall ClearSecondaryHostAnimationFlag2(unsigned int *id);
AnimationVmId __stdcall CreateDirectPositionAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script, int layer);
AnimationVmId __stdcall CreatePositionedSecondaryEmitterAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script);
AnimationVmId __stdcall CreateOffsetSecondaryEmitterAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script);
void __stdcall SignalSecondaryAnimationEvent(AnimationVmId id, short event);

typedef char SecondaryHostSpawnSize[(sizeof(SecondaryHostSpawnArguments) == 0x50) ? 1 : -1];
typedef char SecondaryHostSpawnParametersOffset[(offsetof(SecondaryHostSpawnArguments, parameters) == 0x20) ? 1 : -1];
typedef char SecondaryHostSelectionSize[(sizeof(SecondaryHostAnimationSelection) == 0xc) ? 1 : -1];
typedef char SecondaryHostSelectionOffset[(offsetof(SecondaryActorStateView, unknown108) == 0x108) ? 1 : -1];
typedef char SecondaryHostMetricsOffset[(offsetof(SecondaryHostAnimationMetrics, value50) == 0x50) ? 1 : -1];
typedef char SecondaryHostProjectionCameraOffset[(offsetof(SecondaryHostProjectionRenderer, camera) == 0x204) ? 1 : -1];

namespace SecondaryHostInstructions
{
__forceinline int AddInteger(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) + right); }
__forceinline SecondaryHostAnimationSelection &Selection(SecondaryActorStateView *self)
{ return *reinterpret_cast<SecondaryHostAnimationSelection *>(self->unknown108); }
__forceinline AnimationFileView *File(int index)
{ return SecondaryActorFile(0x40u + 4u * static_cast<unsigned int>(index)); }
__forceinline SecondaryActorAnimationView *AnimationState(AnimationVmView *vm)
{ return reinterpret_cast<SecondaryActorAnimationView *>(vm); }
__forceinline const CameraVector &SceneOffset()
{ return *reinterpret_cast<const CameraVector *>(g_SecondaryHostProjectionRenderer.camera.unknown000); }
__forceinline const SecondaryValue *SpawnPayload(SecondaryInstruction *instruction)
{ return reinterpret_cast<const SecondaryValue *>(instruction + 1); }
__forceinline int SpawnWords(SecondaryInstruction *instruction)
{ return AddInteger(SpawnPayload(instruction)[0].integer, 4) / 4; }
__forceinline void SpawnParameters(SecondaryActorStateView *self, SecondaryHostSpawnArguments *arguments)
{
    arguments->parameters = *reinterpret_cast<const SecondaryHostSpawnParameters *>(self->unknown124);
}
__forceinline void SpawnActor(SecondaryInstruction *instruction, const SecondaryHostSpawnArguments *arguments)
{
    const char *script = reinterpret_cast<const char *>(instruction + 1) + 4;
    CreateSecondaryHostActor(script, arguments);
}
__forceinline void HitDimensions(SecondaryActorStateView *self, AnimationVmView *vm)
{
    SecondaryHostAnimationMetrics *metrics = reinterpret_cast<SecondaryHostAnimationMetrics *>(vm);
    self->hitWidth = metrics->value50 * vm->scaleY;
    self->hitHeight = metrics->value4c * vm->scaleX;
}
__forceinline void FinishAnimationReplacement(SecondaryActorStateView *self,
    unsigned int *id, int slot, AnimationVmView *vm)
{
    if (slot == 0) HitDimensions(self, vm);
    if (self->flags & 0x20) ClearSecondaryHostAnimationFlag2(id);
}
}
