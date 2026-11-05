#pragma once
#include "SecondaryActorInstructionControl.hpp"

struct SecondaryHostPattern
{
    float real0, real1;
    int value2, value3, value4, value5;
};
struct SecondaryHostPatterns { SecondaryHostPattern values[18]; };
struct SecondaryHostRecord
{
    short type, subtype;
    CameraVector position;
    float angle, angle2, speed, speed2, value20;
    SecondaryHostPatterns patterns;
    unsigned char unknown1d4[0x1f8 - 0x1d4];
    short count0, count1, kind, unknown1fe;
    unsigned int flags;
    int value204, value208;
    float unknown20c, unknown210;
};
struct SecondaryHostBasicPacket
{
    CameraVector position;
    float angle, speed1, speed0, angle1, value1c, value20;
    short type, subtype;
    unsigned int flags;
    SecondaryHostPatterns patterns;
    int value1dc, value1e0;
};
struct SecondaryHostAdvancedPacket
{
    CameraVector position;
    unsigned char unknown00c[0x18 - 0xc];
    float angle, unknown01c, speed0, speed1, value28, value2c;
    int value30, value34, value38, value3c, value40, value44, value48;
    short type, subtype;
    unsigned int flags;
    SecondaryHostPatterns patterns;
};
struct SecondaryHostObjectView : SecondaryClearObjectView
{
    unsigned char unknown010[0x3c - 0x10];
    CameraVector position;
    unsigned char unknown048[0x54 - 0x48];
    float value054;
    unsigned char unknown058[4];
    float value05c, value060;
    unsigned char unknown064[0x6c - 0x64];
    int id;
    unsigned char unknown070[0x45c - 0x70];
    float value45c;
};

int __stdcall EmitSecondaryHostRecord(SecondaryHostRecord *record);
SecondaryHostAdvancedPacket *__stdcall ResetSecondaryHostAdvancedPacket(SecondaryHostAdvancedPacket *packet);
int __stdcall CreateSecondaryHostObject(const void *packet, int kind);
SecondaryHostObjectView *__stdcall FindSecondaryHostObject(int id);
void __stdcall SetSecondaryHostObjectTarget(SecondaryHostObjectView *self, const CameraVector *position);
SecondaryVector2 *__stdcall SecondaryHostPolarVector(SecondaryVector2 *result, float angle, float magnitude);
float __stdcall SecondaryAngleMath(float y, float x);
void __stdcall DestroySecondaryHostMesh(StageMeshView *mesh);
void __stdcall SelectSecondaryHostStageLabel(int label);
void __stdcall ShowSecondaryHostScore(int score, const CameraVector *position, int value);
extern int (__fastcall *g_SecondaryHostCallbacks[])(SecondaryActorStateView *self);
extern int (__fastcall *g_SecondaryHostUpdateCallbacks[])(SecondaryActorStateView *self);
extern void (__fastcall *g_SecondaryHostDrawCallbacks[])(SecondaryActorStateView *self);

typedef char SecondaryHostPatternSize[(sizeof(SecondaryHostPattern) == 0x18) ? 1 : -1];
typedef char SecondaryHostPatternsSize[(sizeof(SecondaryHostPatterns) == 0x1b0) ? 1 : -1];
typedef char SecondaryHostRecordSize[(sizeof(SecondaryHostRecord) == 0x214) ? 1 : -1];
typedef char SecondaryHostRecordPatternsOffset[(offsetof(SecondaryHostRecord, patterns) == 0x24) ? 1 : -1];
typedef char SecondaryHostRecordCountsOffset[(offsetof(SecondaryHostRecord, count0) == 0x1f8) ? 1 : -1];
typedef char SecondaryHostRecordFlagsOffset[(offsetof(SecondaryHostRecord, flags) == 0x200) ? 1 : -1];
typedef char SecondaryHostBasicPacketSize[(sizeof(SecondaryHostBasicPacket) == 0x1e4) ? 1 : -1];
typedef char SecondaryHostBasicPatternsOffset[(offsetof(SecondaryHostBasicPacket, patterns) == 0x2c) ? 1 : -1];
typedef char SecondaryHostBasicFlagsOffset[(offsetof(SecondaryHostBasicPacket, flags) == 0x28) ? 1 : -1];
typedef char SecondaryHostAdvancedPacketSize[(sizeof(SecondaryHostAdvancedPacket) == 0x204) ? 1 : -1];
typedef char SecondaryHostAdvancedPatternsOffset[(offsetof(SecondaryHostAdvancedPacket, patterns) == 0x54) ? 1 : -1];
typedef char SecondaryHostAdvancedFlagsOffset[(offsetof(SecondaryHostAdvancedPacket, flags) == 0x50) ? 1 : -1];
typedef char SecondaryHostObjectPositionOffset[(offsetof(SecondaryHostObjectView, position) == 0x3c) ? 1 : -1];
typedef char SecondaryHostObjectIdOffset[(offsetof(SecondaryHostObjectView, id) == 0x6c) ? 1 : -1];
typedef char SecondaryHostObjectScalarOffset[(offsetof(SecondaryHostObjectView, value45c) == 0x45c) ? 1 : -1];

namespace SecondaryHostInstructions
{
__forceinline SecondaryHostRecord *Record(SecondaryActorStateView *self, int index)
{ return reinterpret_cast<SecondaryHostRecord *>(&self->records[index]); }
__forceinline CameraVector &RecordLocal(SecondaryActorStateView *self, int index)
{ return reinterpret_cast<CameraVector *>(self->unknown1414)[index]; }
__forceinline CameraVector &RecordOffset(SecondaryActorStateView *self, int index)
{ return reinterpret_cast<CameraVector *>(self->unknown1414 + 0x60)[index]; }
__forceinline CameraVector RecordPosition(SecondaryActorStateView *self, int index)
{
    const CameraVector &local = RecordLocal(self, index);
    const CameraVector &offset = RecordOffset(self, index);
    CameraVector result;
    if (offset.z > 0.89999997615814209)
    {
        result.x = offset.x + local.x;
        result.y = offset.y + local.y;
        result.z = offset.z + local.z;
        result.z = 0.0f;
    }
    else
    {
        result.x = local.x + self->world.position.x;
        result.y = local.y + self->world.position.y;
        result.z = local.z + self->world.position.z;
    }
    return result;
}
__forceinline float &ProjectileScale()
{ return *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(g_SceneClearPool) + 0x60); }
__forceinline int DifficultyArgument(int first)
{
    if (g_SecondaryActorDifficulty == 0) return first;
    if (g_SecondaryActorDifficulty == 1) return first + 1;
    if (g_SecondaryActorDifficulty == 2) return first + 2;
    return first + 3;
}
__forceinline int InterpolatedInteger(int lower, int upper)
{
    int product = static_cast<int>((static_cast<unsigned int>(upper) - lower) *
        (static_cast<unsigned int>(g_SecondaryHostSelectionValue) + 1024u));
    return AddInteger(product / 2048, lower);
}
}
