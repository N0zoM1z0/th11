#pragma once

#include "SecondaryGameplay.hpp"
#include "FrameLoop.hpp"
#include "Chain.hpp"
#include "AnimationDocument.hpp"

// Physical 0x5B94-byte title/menu owner. Its historical class/TU identity is
// still open, but the four selection blocks and worker placement are observed.
struct TitleMenuRecordView;
struct TitleMenuView
{
    const void *vtable;
    unsigned int flags004;
    unsigned int unknown008;
    ChainElement *updateChain, *drawChain;
    AnimationDocumentView *file18, *file19;
    unsigned char unknown01c[8];
    SecondaryDebugSelectionView selections[3];
    unsigned char unknown02ac[0x2c0 - 0x2ac];
    unsigned int flags2c0;
    unsigned char unknown2c4[0x5c8 - 0x2c4];
    unsigned int trackedAnimationId;
    unsigned char unknown5cc[0x58ec - 0x5cc];
    SecondaryDebugSelectionView replaySelection;
    unsigned char unknown59c4[0x59dc - 0x59c4];
    TitleMenuRecordView *records[100];
    void *loadedBlob;
    int musicDelay;
    unsigned int flags5b74;
    RuntimeThreadView worker;

    TitleMenuView();
    ~TitleMenuView();
};

typedef char TitleMenuSize[(sizeof(TitleMenuView) == 0x5b94) ? 1 : -1];
typedef char TitleMenuSelectionsOffset[(offsetof(TitleMenuView, selections) == 0x24) ? 1 : -1];
typedef char TitleMenuTrackedOffset[(offsetof(TitleMenuView, trackedAnimationId) == 0x5c8) ? 1 : -1];
typedef char TitleMenuReplaySelectionOffset[(offsetof(TitleMenuView, replaySelection) == 0x58ec) ? 1 : -1];
typedef char TitleMenuRecordsOffset[(offsetof(TitleMenuView, records) == 0x59dc) ? 1 : -1];
typedef char TitleMenuWorkerOffset[(offsetof(TitleMenuView, worker) == 0x5b78) ? 1 : -1];

extern TitleMenuView *g_TitleMenu;
int __cdecl InitializeTitleMenuResources();
TitleMenuView *__cdecl StartTitleMenu();
void __cdecl DeleteTitleMenu();

// Owned downstream records and the background callback need their own
// independently reviewed functions; their lifetimes belong to this owner.
void __stdcall DestroyTitleMenuRecord(TitleMenuRecordView *record);
unsigned int __stdcall TitleMenuThread(void *context);
