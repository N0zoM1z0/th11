#pragma once
#include "StageResource.hpp"
#include <stddef.h>

// item.cpp ("item.cpp:60 ItemInf"). The manager at 0x4A8E90 (also
// g_SecondaryAttractionItems) holds 0x896 items of 0x478 bytes. Each item
// starts with its animation VM and has a timer at +0x450; their implicit
// constructor and destructor (0x423220, 0x423240) are what the vector
// iterators call.
struct ItemData
{
    StageEmbeddedVm vm;
    CameraVector position;
    unsigned char unknown440[0x450 - 0x440];
    StageTimerView timer;
    int state;
    unsigned int unknown468;
    int type;
    unsigned char unknown470[0x478 - 0x470];
};

struct ItemInf
{
    ItemInf();
    ~ItemInf();

    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateElement;
    ChainElement *drawElement;
    unsigned int unknown010;
    ItemData items[0x896];
    unsigned char unknown265e64[0x265e70 - 0x265e64];
};

typedef char ItemDataSize[(sizeof(ItemData) == 0x478) ? 1 : -1];
typedef char ItemDataTimer[(offsetof(ItemData, timer) == 0x450) ? 1 : -1];
typedef char ItemDataState[(offsetof(ItemData, state) == 0x464) ? 1 : -1];
typedef char ItemDataType[(offsetof(ItemData, type) == 0x46c) ? 1 : -1];
typedef char ItemInfItems[(offsetof(ItemInf, items) == 0x14) ? 1 : -1];
typedef char ItemInfSize[(sizeof(ItemInf) == 0x265e70) ? 1 : -1];

extern ItemInf *g_ItemManager;

int __stdcall InitializeItemManager(ItemInf *self);
ItemInf *__cdecl CreateItemManager();
void __cdecl DeleteItemManager();
int __fastcall ItemManagerUpdateCallback(ItemInf *self);
// 0x423550, the item update loop (not yet reconstructed).
int __stdcall UpdateItemManager(ItemInf *self);
int __fastcall ItemManagerDrawCallback(ItemInf *self);
int __stdcall DrawItemManager(ItemInf *self);
