#pragma once

#include "SecondaryGameplay.hpp"
#include "SecondaryScript.hpp"
#include <stddef.h>

// Observed native TH11 ECL browser fields. These offsets describe the real
// physical storage without assuming the original class name or ownership.
struct ChainElement;
struct SecondaryProgramBrowserView
{
    unsigned char unknown000[0x08];
    ChainElement *updateChain, *drawChain;
    RuntimeThreadView worker;
    unsigned int unknown02c;
    int state;
    char **filenames;
    int fileCount;
    SecondaryDebugSelectionView category;
    SecondaryDebugSelectionView fileSelection;
    SecondaryDebugSelectionView subroutineSelection;
    float displayOrigin;
    float displayScale, displayOffset;
    int displayTimer;
    unsigned int flags;
    SecondaryProgramView *program;
    SecondaryManager *manager;
    unsigned char *loadedData;
};
typedef char BrowserUpdateChainOffset[(offsetof(SecondaryProgramBrowserView, updateChain) == 0x08) ? 1 : -1];
typedef char BrowserDrawChainOffset[(offsetof(SecondaryProgramBrowserView, drawChain) == 0x0c) ? 1 : -1];
typedef char BrowserStateOffset[(offsetof(SecondaryProgramBrowserView, state) == 0x30) ? 1 : -1];
typedef char BrowserFilesOffset[(offsetof(SecondaryProgramBrowserView, filenames) == 0x34) ? 1 : -1];
typedef char BrowserCategoriesOffset[(offsetof(SecondaryProgramBrowserView, category) == 0x3c) ? 1 : -1];
typedef char BrowserFileSelectionOffset[(offsetof(SecondaryProgramBrowserView, fileSelection) == 0x114) ? 1 : -1];
typedef char BrowserSubroutineSelectionOffset[(offsetof(SecondaryProgramBrowserView, subroutineSelection) == 0x1ec) ? 1 : -1];
typedef char BrowserDisplayOriginOffset[(offsetof(SecondaryProgramBrowserView, displayOrigin) == 0x2c4) ? 1 : -1];
typedef char BrowserDisplayScaleOffset[(offsetof(SecondaryProgramBrowserView, displayScale) == 0x2c8) ? 1 : -1];
typedef char BrowserFlagsOffset[(offsetof(SecondaryProgramBrowserView, flags) == 0x2d4) ? 1 : -1];
typedef char BrowserProgramOffset[(offsetof(SecondaryProgramBrowserView, program) == 0x2d8) ? 1 : -1];
typedef char BrowserManagerOffset[(offsetof(SecondaryProgramBrowserView, manager) == 0x2dc) ? 1 : -1];
typedef char BrowserLoadedDataOffset[(offsetof(SecondaryProgramBrowserView, loadedData) == 0x2e0) ? 1 : -1];

void __stdcall ClearSecondaryProgramBrowserFiles(SecondaryProgramBrowserView *self);
int __fastcall UpdateSecondaryProgramBrowser(SecondaryProgramBrowserView *self);
int __stdcall RegisterSecondaryProgramBrowserChains(SecondaryProgramBrowserView *self);
int __stdcall DrawSecondaryProgramBrowser(SecondaryProgramBrowserView *self);

// Fully reconstructed 77-byte factory at 0x0045DB90: separate exact owner.
SecondaryManager *__stdcall CreateSecondaryManager(
    SecondaryProgramView *program, const char *subroutineName);

// Native ECL browser loader and dedicated thread callback at 438910/438900.
extern SecondaryProgramBrowserView *g_SecondaryProgramBrowser;
int __cdecl LoadSecondaryProgramSelection();
unsigned int __cdecl SecondaryProgramBrowserLoadThread();
