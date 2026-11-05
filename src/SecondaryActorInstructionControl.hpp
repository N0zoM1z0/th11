#pragma once
#include "SecondaryActorInstructionAnimation.hpp"
#include "SecondaryClearing.hpp"

// Additional physical views used by the 412E30 control/effect cases. These
// describe observed offsets, without assigning an original class or capacity.
struct SecondaryHostHudGauge { float fraction; int threshold; };
struct SecondaryHostControlHud
{
    unsigned char unknown000[0x43fc];
    int value43fc;
    SecondaryHostHudGauge gauges[4];
    unsigned char unknown4420[0x4438 - 0x4420];
    void *dialogue;
};
struct SecondaryHostSelectionState
{
    unsigned char unknown000[0x50];
    int selection, unknown054, value058;
};
extern SecondaryHostSelectionState g_SecondaryHostSelectionState;
extern int g_SecondaryHostSelectionValue;

int __stdcall ReadSecondaryHostInteger(SecondaryActorStateView *self, int index);
float __stdcall ReadSecondaryHostReal(SecondaryActorStateView *self, int index);
int *__stdcall SecondaryHostIntegerDestination(SecondaryActorStateView *self);
float *__stdcall SecondaryHostFloatDestination(SecondaryActorStateView *self);
void __stdcall SetSecondaryHostAnimationFlag2(unsigned int *id);
void __stdcall ResetSecondaryStateWithEffect(SecondaryActorEffectView *self,
    const CameraVector *position);
void __stdcall SetSecondaryHostSlot(SecondaryActorManagerView *self, int slot,
    const char *script, int frameLimit);
void __stdcall BeginSecondaryHostSpell(const char *name, int id, int value);
void __stdcall FinishSecondaryHostSpell();
int __stdcall SecondaryHostDialogueStatus();
void __stdcall ReleaseSecondaryHostSpellAnimation();
void __stdcall SetSecondaryHostSelection(SecondaryHostSelectionState *self, int value);
void __stdcall ClearSecondaryHostActors();
void __stdcall ClearSecondaryHostActorsByGroup(int group);
void __stdcall CreateSecondaryHostTransition(int value);
int __stdcall SetSecondaryHostStageMode(int mode);
void *__stdcall CreateSecondaryHostScreenEffect(int mode, int value0,
    int value1, int value2, int kind);
int __stdcall MarkSecondaryHostEffects();

typedef char SecondaryHostHudGaugeSize[(sizeof(SecondaryHostHudGauge) == 8) ? 1 : -1];
typedef char SecondaryHostHudGaugeOffset[(offsetof(SecondaryHostControlHud, gauges) == 0x4400) ? 1 : -1];
typedef char SecondaryHostHudDialogueOffset[(offsetof(SecondaryHostControlHud, dialogue) == 0x4438) ? 1 : -1];
typedef char SecondaryHostSelectionValueOffset[(offsetof(SecondaryHostSelectionState, value058) == 0x58) ? 1 : -1];

namespace SecondaryHostInstructions
{
__forceinline SecondaryHostControlHud *ControlHud()
{ return reinterpret_cast<SecondaryHostControlHud *>(g_SecondaryActorHud); }
__forceinline unsigned int &MasterFlags()
{ return *reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x3c); }
__forceinline int ScaleCounter(int value)
{ return static_cast<int>(static_cast<unsigned int>(value) * 7u); }
__forceinline int EffectIndex(SecondaryActorStateView *self, int index)
{
    if (self->flags & 0x80000)
    {
        if (index == 1) return 10;
        if (index == 4) return 11;
    }
    return index;
}
__forceinline void SetFlag(SecondaryActorStateView *self, int value, unsigned int mask)
{ self->flags = (self->flags & ~mask) | ((value & 1) ? mask : 0u); }
}
