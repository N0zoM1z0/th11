#include "AnimationExecution.hpp"
#include "MainRuntime.hpp"

namespace
{
__forceinline AnimationExecutionView *State(AnimationVmView *vm)
{ return reinterpret_cast<AnimationExecutionView *>(vm); }
__forceinline RandomStateView *Random(AnimationVmView *vm)
{ return (vm->flags.value & 0x40000000) ? &g_AnimationSceneRandomState : &g_TextRandomState; }
__forceinline float RuntimeFloat(unsigned int offset)
{ return *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + offset); }
}

float __stdcall ReadAnimationFloat(AnimationVmView *vm, float encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (static_cast<int>(encoded))
    {
    case 10000: return static_cast<float>(self->integerVariables[0]);
    case 10001: return static_cast<float>(self->integerVariables[1]);
    case 10002: return static_cast<float>(self->integerVariables[2]);
    case 10003: return static_cast<float>(self->integerVariables[3]);
    case 10004: return self->floatVariables[0];
    case 10005: return self->floatVariables[1];
    case 10006: return self->floatVariables[2];
    case 10007: return self->floatVariables[3];
    case 10008: return static_cast<float>(self->integer3d4);
    case 10009: return static_cast<float>(self->integer3d8);
    case 10010: return AnimationRandomSignedRange(Random(vm), 3.1415927f);
    case 10011: return AnimationRandomUnit(Random(vm));
    case 10012: return AnimationRandomSignedUnit(Random(vm));
    case 10013: return self->position.x;
    case 10014: return self->position.y;
    case 10015: return self->position.z;
    case 10016: return RuntimeFloat(0x434);
    case 10017: return RuntimeFloat(0x438);
    case 10018: return RuntimeFloat(0x43c);
    case 10019: return RuntimeFloat(0x458);
    case 10020: return RuntimeFloat(0x45c);
    case 10021: return RuntimeFloat(0x460);
    case 10022: return static_cast<float>(RandomNext32(Random(vm)));
    default: return encoded;
    }
}

int __stdcall ReadAnimationInteger(AnimationVmView *vm, int encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (encoded)
    {
    case 10000: return self->integerVariables[0];
    case 10001: return self->integerVariables[1];
    case 10002: return self->integerVariables[2];
    case 10003: return self->integerVariables[3];
    case 10004: return static_cast<int>(self->floatVariables[0]);
    case 10005: return static_cast<int>(self->floatVariables[1]);
    case 10006: return static_cast<int>(self->floatVariables[2]);
    case 10007: return static_cast<int>(self->floatVariables[3]);
    case 10008: return self->integer3d4;
    case 10009: return self->integer3d8;
    case 10022: return static_cast<int>(RandomNext32(Random(vm)));
    default: return encoded;
    }
}

float *__stdcall AnimationFloatDestination(AnimationVmView *vm, float *encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (static_cast<int>(*encoded))
    {
    case 10004: return &self->floatVariables[0];
    case 10005: return &self->floatVariables[1];
    case 10006: return &self->floatVariables[2];
    case 10007: return &self->floatVariables[3];
    case 10013: return &self->position.x;
    case 10014: return &self->position.y;
    case 10015: return &self->position.z;
    default: return encoded;
    }
}

int *__stdcall AnimationIntegerDestination(AnimationVmView *vm, int *encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (*encoded)
    {
    case 10000: return &self->integerVariables[0];
    case 10001: return &self->integerVariables[1];
    case 10002: return &self->integerVariables[2];
    case 10003: return &self->integerVariables[3];
    case 10008: return &self->integer3d4;
    case 10009: return &self->integer3d8;
    default: return encoded;
    }
}

void __stdcall SetAnimationUv(AnimationFloatPair *output, const CameraVector *point)
{
    output->x = static_cast<float>(point->x / 640.0);
    output->y = static_cast<float>(point->y / 480.0);
    if (output->x < 0.0f) output->x = 0.0f;
    if (output->y < 0.0f) output->y = 0.0f;
}
