#pragma once
#include "Chain.hpp"
#include <stddef.h>

// hint.cpp ("hint.cpp:194 HintInf"), published at 0x4A8E8C. It loads
// hint/hint_auto.txt and hint/hint_user.txt into sixteen lists (eight
// categories each); +0xDC holds ten animation VM IDs.
struct HintNode
{
    void *data;
    HintNode *next;
};

struct HintList
{
    struct HintInf *owner;
    HintNode *first;
    HintNode *last;
};

struct HintInf
{
    HintInf();
    void ResetList(HintList &list)
    {
        list.first = 0;
        list.last = 0;
        list.owner = this;
    }

    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateElement;
    ChainElement *drawElement;
    int unknown010;
    unsigned int unknown014;
    HintList lists[16];
    unsigned int unknown0d8;
    unsigned int vmIds[10];
    unsigned char unknown104[0x1a8 - 0x104];
};
typedef char HintInfLists[(offsetof(HintInf, lists) == 0x18) ? 1 : -1];
typedef char HintInfVmIds[(offsetof(HintInf, vmIds) == 0xdc) ? 1 : -1];
typedef char HintInfSize[(sizeof(HintInf) == 0x1a8) ? 1 : -1];

extern HintInf *g_HintManager;

int __stdcall InitializeHintManager(HintInf *self);
void __stdcall FreeHintLists(HintInf *self);
int __stdcall LoadHints(HintInf *self, const char *path, int user);
int __fastcall HintManagerUpdateCallback(HintInf *self);
int __fastcall HintManagerDrawCallback(HintInf *self);
