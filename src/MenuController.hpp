#pragma once

#include "ConfigurationAudioMenu.hpp"

// Partial view of the same menu object used by the audio page. Original class
// identity and complete allocation size remain unresolved.
struct MenuControllerView
{
    ConfigurationAudioMenuView page;
    unsigned char unknown2cc[0x67c - 0x2cc];
    AnimationVmId secondaryRootVmId;
    unsigned char unknown680[0x5b70 - 0x680];
    int musicDelay;
    unsigned int flags;
};

struct MenuReplayHeaderView
{
    unsigned char unknown000[0x5c];
    int selection0, selection1, difficulty;
};
struct MenuReplayStageView
{
    unsigned int marker;
    unsigned char unknown004[0x24 - 4];
};
struct MenuReplayView
{
    unsigned char unknown000[0x18];
    MenuReplayHeaderView *header;
    unsigned char unknown01c[0xb4 - 0x1c];
    MenuReplayStageView stages[8];
};

typedef char MenuControllerPageOffset[(offsetof(MenuControllerView, page) == 0) ? 1 : -1];
typedef char MenuControllerSecondaryVmOffset[(offsetof(MenuControllerView, secondaryRootVmId) == 0x67c) ? 1 : -1];
typedef char MenuControllerDelayOffset[(offsetof(MenuControllerView, musicDelay) == 0x5b70) ? 1 : -1];
typedef char MenuControllerFlagsOffset[(offsetof(MenuControllerView, flags) == 0x5b74) ? 1 : -1];
typedef char MenuReplayHeaderOffset[(offsetof(MenuReplayView, header) == 0x18) ? 1 : -1];
typedef char MenuReplayStagesOffset[(offsetof(MenuReplayView, stages) == 0xb4) ? 1 : -1];
typedef char MenuReplayStageStride[(sizeof(MenuReplayStageView) == 0x24) ? 1 : -1];

extern int g_MenuEntryMode, g_MenuIdleFrames, g_MenuDemoIndex;
extern int g_MenuPreviousDifficulty, g_MenuPreviousVariant;
extern const char *g_MenuDemoPaths[4];
extern char g_MenuReplayPath[];
extern unsigned char g_MenuStageSelections[];

int __stdcall UpdateMenuController(MenuControllerView *self);
int __fastcall MenuControllerUpdateCallback(MenuControllerView *self);
void __stdcall MarkMenuFileAnimations(RenderBatchView *renderer, AnimationFileView *file);

// Reviewed source-open call boundaries. Numeric page names describe the native
// dispatch slots; neither original names nor private register ABIs are claimed.
void __stdcall EnqueueMenuMusicLoad(int slot, const char *filename);
MenuReplayView *__stdcall ReadMenuReplay(const char *filename);
void __stdcall DestroyMenuReplayContents(MenuReplayView *replay);
int __stdcall UpdateMenuPage01(MenuControllerView *self);
int __stdcall UpdateMenuPage04(MenuControllerView *self);
int __stdcall UpdateMenuPage05(MenuControllerView *self);
int __stdcall UpdateMenuPage06(MenuControllerView *self);
int __stdcall UpdateMenuPage07(MenuControllerView *self);
int __stdcall UpdateMenuPage08(MenuControllerView *self);
int __stdcall UpdateMenuPage0A(MenuControllerView *self);
int __stdcall UpdateMenuPage0B(MenuControllerView *self);
int __stdcall UpdateMenuPage0D(MenuControllerView *self);
int __stdcall UpdateMenuPage0E(MenuControllerView *self);
int __stdcall UpdateMenuPage0F(MenuControllerView *self);
