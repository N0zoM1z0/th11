#pragma once

#include "AsciiManager.hpp"
#include "StageResource.hpp"

struct AnimationVmView;

// Lifecycle view of the same 0x184BC-byte object as AsciiManagerView.
// The virtual destructor and two actual embedded VM lifetimes are observed
// independently of the caller's LTCG register assignments.
struct AsciiManagerObject
{
    unsigned int flags, unknown008;
    ChainElement *updateChain, *drawChain;
    StageEmbeddedVm textVm, overlayVm;
    AsciiStringRecord strings[320];
    int stringCount;
    unsigned int color;
    float scale[2];
    unsigned int unknown1848c, unknown18490, unknown18494;
    unsigned int unknown18498, unknown1849c;
    int stringLifetime;
    unsigned int drawMode;
    int frameCounter;
    AnimationDocumentView *animation;
    unsigned int unknown184b0, unknown184b4;
    ChainElement *overlayChain;

    AsciiManagerObject();
    virtual ~AsciiManagerObject();
};

typedef char AsciiManagerObjectSize[(sizeof(AsciiManagerObject) == 0x184bc) ? 1 : -1];
typedef char AsciiManagerTextVmOffset[(offsetof(AsciiManagerObject, textVm) == 0x14) ? 1 : -1];
typedef char AsciiManagerOverlayVmOffset[(offsetof(AsciiManagerObject, overlayVm) == 0x448) ? 1 : -1];
typedef char AsciiManagerStringsOffset[(offsetof(AsciiManagerObject, strings) == 0x87c) ? 1 : -1];
typedef char AsciiManagerAnimationOffset[(offsetof(AsciiManagerObject, animation) == 0x184ac) ? 1 : -1];
typedef char AsciiManagerOverlayChainOffset[(offsetof(AsciiManagerObject, overlayChain) == 0x184b8) ? 1 : -1];

extern AsciiManagerObject *g_AsciiManager;
extern const char g_AsciiAnimationFilename[], g_AsciiAnimationLoadError[];
int __stdcall InitializeAsciiManager(AsciiManagerObject *self);
int __fastcall AsciiManagerDrawCallback(AsciiManagerObject *self);
int __fastcall AsciiManagerOverlayCallback(AsciiManagerObject *self);

// Reviewed target call boundaries; drawing bodies remain external.
int __stdcall DrawAsciiManager(AsciiManagerObject *self, int mode);
int __stdcall DrawAsciiOverlay(AsciiManagerObject *self);
