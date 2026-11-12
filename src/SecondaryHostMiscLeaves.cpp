#include "SecondaryHostMiscLeaves.hpp"
#include "SecondaryActorRuntime.hpp"
#include <string.h>

extern const float g_SecondaryHostEffectDefaultAngle;

// The original 0x4107B0 owner raises bit 4 of the spell's +0x8E0 status,
// releases its +0x884 animation ID and clears that slot afterwards.
void __stdcall ReleaseSecondaryHostSpellAnimation()
{
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    spell->flags |= 0x10u;
    unsigned int &id = *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(spell) + 0x884);
    ReleaseSecondaryAnimation(AnimationVmId(id));
    id = 0;
}

// The original 0x410880 owner clears exactly 0x204 bytes, then restores a
// floating-point value at +0x2C from original readonly data 0x497FDC.
struct SecondaryHostEffectSnapshot
{
    unsigned char unknown000[0x2c];
    float initialAngle;
    unsigned char unknown030[0x204 - 0x30];
};
SecondaryHostEffectSnapshot *__fastcall ResetSecondaryHostEffectSnapshot(
    SecondaryHostEffectSnapshot *self)
{
    memset(self, 0, sizeof(*self));
    self->initialAngle = g_SecondaryHostEffectDefaultAngle;
    return self;
}

// The original 0x4108A0 copies a 3-float vector from the instruction's
// source to the native host effect storage at +0x44C without validation.
struct SecondaryHostVectorStorage
{
    unsigned char unknown000[0x44c];
    CameraVector position;
};
void __fastcall CopySecondaryHostVector(SecondaryHostVectorStorage *self,
    const CameraVector *position)
{
    self->position = *position;
}

typedef char SecondaryHostEffectSnapshotSize[
    (sizeof(SecondaryHostEffectSnapshot) == 0x204) ? 1 : -1];
typedef char SecondaryHostCopyVectorOffset[
    (offsetof(SecondaryHostVectorStorage, position) == 0x44c) ? 1 : -1];
