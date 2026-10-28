#pragma once

#include "AnimationDocument.hpp"
#include "Chain.hpp"
#include "PerspectiveCamera.hpp"
#include "VmBindings.hpp"

extern float g_AnimationTimeScale;

struct StageTimerView
{
    int previous, current;
    float fraction;
    float *scale;
    unsigned int flags;

    StageTimerView() { flags &= ~1u; }
    __forceinline void SetFrame(int value)
    {
        if (!(flags & 1))
        {
            fraction = 0.0f;
            current = 0;
            previous = -999999;
            scale = &g_AnimationTimeScale;
            flags |= 1;
        }
        current = 0;
        fraction = 0.0f;
        previous = value;
    }
};

struct StageEmbeddedVm
{
    unsigned char unknown000[0x5c];
    StageTimerView timer05c;
    unsigned char unknown070[0xc0 - 0x70];
    StageTimerView timer0c0;
    unsigned char unknown0d4[0x10c - 0xd4];
    StageTimerView timer10c;
    unsigned char unknown120[0x138 - 0x120];
    StageTimerView timer138;
    unsigned char unknown14c[0x184 - 0x14c];
    StageTimerView timer184;
    unsigned char unknown198[0x1c0 - 0x198];
    StageTimerView timer1c0;
    unsigned char unknown1d4[0x20c - 0x1d4];
    StageTimerView timer20c;
    unsigned char unknown220[0x238 - 0x220];
    StageTimerView timer238;
    unsigned char unknown24c[0x264 - 0x24c];
    StageTimerView timer264;
    unsigned char unknown278[0x290 - 0x278];
    StageTimerView timer290;
    unsigned char unknown2a4[0x380 - 0x2a4];
    StageTimerView timer380;
    unsigned char unknown394[8];
    short marker39c;
    unsigned char unknown39e[0x400 - 0x39e];
    void *storage400;
    unsigned char unknown404[0x434 - 0x404];

    StageEmbeddedVm();
    ~StageEmbeddedVm();
};

struct StageCameraView
{
    PerspectiveCameraView view;
    CameraVector updateOffset;
    float fogStart, fogEnd;
    unsigned char unknown104[0x114 - 0x104];
    unsigned int fogColor;
};

struct StageBlobHeaderView
{
    short objectCount, vmCount;
    unsigned int section004Offset, section008Offset;
    unsigned int unknown00c;
    char animationFilename[128];
};

struct StageResourceView
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain, *drawChain;
    StageBlobHeaderView *data;
    unsigned char **objects;
    unsigned char *section004, *section008;
    unsigned char unknown020[4];
    StageTimerView timer024, time;
    unsigned char unknown04c[0x80 - 0x4c];
    StageTimerView timer080;
    int reset094;
    unsigned char unknown098[0xcc - 0x98];
    StageTimerView timer0cc;
    int reset0e0;
    unsigned char unknown0e4[0x158 - 0xe4];
    StageTimerView timer158;
    unsigned char unknown16c[0x178 - 0x16c];
    AnimationDocumentView *animation;
    void *vms;
    StageEmbeddedVm vms180[8];
    float distanceSquared;
    unsigned int drawCount;
    unsigned int tint;
    int accent;
    unsigned char unknown2330[0x2348 - 0x2330];
    StageEmbeddedVm vms2348[3];
    CameraVector drawOffset;
    unsigned int flags2ff0;
    StageTimerView timer2ff4;
    unsigned int variant;
    int reset300c;
    VmBindingBlockView *bindings;
    float minimumRadius, radius;
    unsigned int unknown301c;
    float phaseX, phaseY;
    int distortionMode;
    unsigned char unknown302c[0x3034 - 0x302c];
    ChainElement *overlayChain;
    unsigned char *source;
    unsigned int sourceSize;
    StageCameraView camera;

    StageResourceView();
    ~StageResourceView();
};

typedef char StageBlobHeaderSize[(sizeof(StageBlobHeaderView) == 0x90) ? 1 : -1];
typedef char StageResourceAnimationOffset[(offsetof(StageResourceView, animation) == 0x178) ? 1 : -1];
typedef char StageResourceVariantOffset[(offsetof(StageResourceView, variant) == 0x3008) ? 1 : -1];
typedef char StageResourceSourceOffset[(offsetof(StageResourceView, source) == 0x3038) ? 1 : -1];
typedef char StageTimerSize[(sizeof(StageTimerView) == 20) ? 1 : -1];
typedef char StageEmbeddedVmSize[(sizeof(StageEmbeddedVm) == 0x434) ? 1 : -1];
typedef char StageEmbeddedVmStorageOffset[(offsetof(StageEmbeddedVm, storage400) == 0x400) ? 1 : -1];
typedef char StageCameraSize[(sizeof(StageCameraView) == 0x118) ? 1 : -1];
typedef char StageCameraFogOffset[(offsetof(StageCameraView, fogColor) == 0x114) ? 1 : -1];
typedef char StageCameraUpdateOffset[(offsetof(StageCameraView, updateOffset) == 0xf0) ? 1 : -1];
typedef char StageResourceDistortionOffset[(offsetof(StageResourceView, distortionMode) == 0x3028) ? 1 : -1];
typedef char StageResourceTintOffset[(offsetof(StageResourceView, tint) == 0x2328) ? 1 : -1];
typedef char StageResourceDrawOffset[(offsetof(StageResourceView, drawOffset) == 0x2fe4) ? 1 : -1];
typedef char StageResourceSize[(sizeof(StageResourceView) == 0x3158) ? 1 : -1];
typedef char StageResourceVmOffset[(offsetof(StageResourceView, vms2348) == 0x2348) ? 1 : -1];
typedef char StageResourceTimeOffset[(offsetof(StageResourceView, time) == 0x38) ? 1 : -1];
typedef char StageResourceCameraOffset[(offsetof(StageResourceView, camera) == 0x3040) ? 1 : -1];

extern char g_StageResourceName[];
extern const char g_StageAnimationError[];
extern unsigned int g_StageVariant, g_StageResourceFlags;
extern StageResourceView *g_StagePrimary, *g_StageSecondary;
extern const char g_StageLoadError[];
int __stdcall LoadStageResource(StageResourceView *self, const char *filename);
int __stdcall InitializeStageResource(StageResourceView *self, const char *filename);
StageResourceView *__stdcall CreateStageResource(const char *filename);
int __fastcall StageUpdateCallback(void *context);
int __fastcall StageDrawCallback(void *context);
int __fastcall StageOverlayCallback(void *context);
int __stdcall UpdateStage(StageResourceView *self);
int __stdcall DrawStage(StageResourceView *self);
int __stdcall DrawStageOverlay(StageResourceView *self);
void __stdcall DestroyAnimationDocument(AnimationDocumentView *file);
