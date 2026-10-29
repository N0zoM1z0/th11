#pragma once
#include "SecondaryActor.hpp"
#include "SecondaryCounter.hpp"
#include "StageScript.hpp"

struct SecondaryActorManagerView;
struct SecondaryActorNode
{
    SecondaryActorManagerView *actor;
    SecondaryActorNode *next, *previous;
};

// Physical motion record shared by the four observed state blocks.
struct SecondaryMotionView
{
    CameraVector position, velocity;
    float angle, angularVelocity, radius, radialVelocity, tilt, ellipseRatio;
    unsigned int flags;
};

struct SecondaryPairTween
{
    SecondaryVector2 initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};

struct SecondaryActorRecord
{
    unsigned char unknown000[0x208];
    int sentinel;
    unsigned char unknown20c[8];
    SecondaryActorRecord() { memset(this, 0, sizeof(*this)); sentinel = -1; }
};

struct SecondaryActorEffectView
{
    int id;
    unsigned char unknown004[0x34 - 4];
    float width, height;
};

struct SecondaryActorSlot
{
    int first, second, value;
    unsigned int unknown00c;
};

// Observed 0x163c-byte state allocation. Unknown storage is retained without
// assigning original class names, ownership or behavior to those bytes.
struct SecondaryActorStateView
{
    SecondaryActorStateView();
    SecondaryMotionView snapshot, world, local, offset;
    float extentX, extentY, extentZ, unknown0dc;
    unsigned int animationIds[10];
    unsigned char unknown108[0x114 - 0x108];
    int scriptBase, direction, unknown11c, enabled;
    unsigned char unknown124[0x154 - 0x124];
    StageTimerView frame;
    SecondaryActorNode node;
    StageVectorTween localTween, offsetTween;
    SecondaryPairTween pairTweens[6];
    SecondaryActorRecord records[8];
    unsigned char unknown1414[0x14dc - 0x1414];
    float boundX, boundY, boundWidth, boundHeight;
    unsigned int unknown14ec;
    SecondaryCounterView counter;
    SecondaryActorEffectView effect;
    unsigned char unknown1544[0x1558 - 0x1544];
    StageTimerView timer1558, timer156c;
    unsigned int flags;
    int script1584, script1588, index, unknown1590;
    SecondaryActorSlot slots[8];
    SecondaryActorManagerView *owner;
    StageMeshView *mesh;
    unsigned int unknown161c;
    float meshProgress, mesh1624, mesh1628, mesh162c;
    void *hook1630, *hook1634, *hook1638;
};

struct SecondaryActorManagerView : SecondaryManager
{
    SecondaryActorManagerView(const char *name);
    virtual ~SecondaryActorManagerView();
    virtual int ExecuteInstruction();
    virtual int ReadGlobalInt(int reference);
    virtual int *GlobalIntPointer(int reference);
    virtual float ReadGlobalFloat(int reference);
    virtual float *GlobalFloatPointer(int reference);
    SecondaryActorStateView state;
};

struct SecondaryMasterView
{
    unsigned char unknown000[0x1c];
    unsigned char indexed01c[0x64 - 0x1c];
    SecondaryProgramView *program;
    SecondaryActorNode *first, *last;
    int count;
    unsigned int unknown074;
    // The target indexes from 0x1c; the slot count and overlapping views are open.
    __forceinline SecondaryActorManagerView *&Slot(int index)
    { return reinterpret_cast<SecondaryActorManagerView **>(indexed01c)[index]; }
};

struct SecondaryActorReference
{
    SecondaryActorManagerView *actor;
    unsigned char unknown004[0x6c - 4];
};

struct SecondaryActorReferencesView
{
    unsigned char unknown000[0x9c0];
    SecondaryActorReference records[256];
    unsigned char unknown75c0[0x7c94 - 0x75c0];
    SecondaryActorManagerView *tracked;
    unsigned char trackingLocked;
    unsigned char unknown7c99[0x8bb4 - 0x7c99];
    int status;
    unsigned char unknown8bb8[0x8bc4 - 0x8bb8];
    unsigned int flags;
};

extern SecondaryMasterView *g_SecondaryMaster;
extern SecondaryActorReferencesView *g_SecondaryActorReferences;

void __stdcall DetachSecondaryActor(SecondaryActorManagerView *self);
void __stdcall UpdateSecondaryActorPosition(SecondaryActorStateView *self);
void __stdcall ComposeSecondaryMotion(SecondaryMotionView *self);
void __fastcall TrackSecondaryActor(SecondaryActorManagerView *self);
int __stdcall SecondaryActorReferenceStatus();
// Native writes the pair through its result buffer and returns that address.
// This physical contract leaves the original vector class's return ABI open.
SecondaryVector2 *__stdcall StepSecondaryPairTween(SecondaryVector2 *result, SecondaryPairTween *self);
float __stdcall SecondaryPairTweenFactor(SecondaryPairTween *self);
void __stdcall ReleaseSecondaryAuxiliaryThreads(SecondaryManager *self);
void __stdcall ResetSecondaryMainThread(SecondaryManager *self);

typedef char SecondaryMotionSize[(sizeof(SecondaryMotionView) == 0x34) ? 1 : -1];
typedef char SecondaryPairTweenSize[(sizeof(SecondaryPairTween) == 0x3c) ? 1 : -1];
typedef char SecondaryActorRecordSize[(sizeof(SecondaryActorRecord) == 0x214) ? 1 : -1];
typedef char SecondaryActorStateSize[(sizeof(SecondaryActorStateView) == 0x163c) ? 1 : -1];
typedef char SecondaryActorSize[(sizeof(SecondaryActorManagerView) == 0x2678) ? 1 : -1];
typedef char SecondaryActorFrameOffset[(offsetof(SecondaryActorStateView, frame) == 0x154) ? 1 : -1];
typedef char SecondaryActorNodeOffset[(offsetof(SecondaryActorStateView, node) == 0x168) ? 1 : -1];
typedef char SecondaryActorTweenOffset[(offsetof(SecondaryActorStateView, localTween) == 0x174) ? 1 : -1];
typedef char SecondaryActorRecordOffset[(offsetof(SecondaryActorStateView, records) == 0x374) ? 1 : -1];
typedef char SecondaryActorCounterOffset[(offsetof(SecondaryActorStateView, counter) == 0x14f0) ? 1 : -1];
typedef char SecondaryActorTimerOffset[(offsetof(SecondaryActorStateView, timer1558) == 0x1558) ? 1 : -1];
typedef char SecondaryActorFlagsOffset[(offsetof(SecondaryActorStateView, flags) == 0x1580) ? 1 : -1];
typedef char SecondaryActorOwnerOffset[(offsetof(SecondaryActorStateView, owner) == 0x1614) ? 1 : -1];
typedef char SecondaryMasterProgramOffset[(offsetof(SecondaryMasterView, program) == 0x64) ? 1 : -1];
typedef char SecondaryMasterSize[(sizeof(SecondaryMasterView) == 0x78) ? 1 : -1];
typedef char SecondaryActorTrackedOffset[(offsetof(SecondaryActorReferencesView, tracked) == 0x7c94) ? 1 : -1];
typedef char SecondaryActorStatusOffset[(offsetof(SecondaryActorReferencesView, status) == 0x8bb4) ? 1 : -1];
