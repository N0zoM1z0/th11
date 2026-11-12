#pragma once
#include "Chain.hpp"
#include "AnimationDocument.hpp"
#include <stddef.h>

// Physical native 0x78C-byte replay file browser owner. The private original
// class/TU identity is unknown; observed linked callback and blob offsets are
// preserved, without claiming to understand the intervening UI storage.
struct ReplayFileBrowserView
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *update, *draw;
    AnimationDocumentView *animation;
    unsigned char unknown014[0x418 - 0x14];
    void *ownedData;
    unsigned char unknown41c[0x78c - 0x41c];
};
typedef char ReplayFileBrowserSize[(sizeof(ReplayFileBrowserView) == 0x78c) ? 1 : -1];
typedef char ReplayFileBrowserUpdateOffset[(offsetof(ReplayFileBrowserView, update) == 8) ? 1 : -1];
typedef char ReplayFileBrowserBlobOffset[(offsetof(ReplayFileBrowserView, ownedData) == 0x418) ? 1 : -1];

extern ReplayFileBrowserView *g_ReplayFileBrowser;
void __stdcall DestroyReplayFileBrowser(ReplayFileBrowserView *self);
void __cdecl DeleteReplayFileBrowser();
int __stdcall InitializeReplayFileBrowser(ReplayFileBrowserView *self);
ReplayFileBrowserView *__cdecl CreateReplayFileBrowser();
