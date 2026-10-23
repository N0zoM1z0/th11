#pragma once

#include "RenderBatch.hpp"
#include <stddef.h>

struct BoundVmView;
struct BoundVmNodeView
{
    BoundVmView *vm;
    BoundVmNodeView *next;
};

struct BoundVmView
{
    unsigned int id;
    unsigned char unknown004[0x14 - 4];
    BoundVmNodeView *related;
    void *marker018;
    unsigned char unknown01c[0x404 - 0x1c];
    unsigned int flags;
};

struct BoundVmListsView
{
    unsigned char unknown000[0x7b562c];
    BoundVmNodeView *first;
    unsigned int unknown7b5630;
    BoundVmNodeView *second;
};

struct VmBindingBlockView
{
    int count;
    unsigned int unknown004;
    unsigned int *ids;
    void *storage00c;
    void *storage010;
    void *storage014;
};

typedef char BoundVmFlagsOffset[(offsetof(BoundVmView, flags) == 0x404) ? 1 : -1];
typedef char BoundVmFirstListOffset[(offsetof(BoundVmListsView, first) == 0x7b562c) ? 1 : -1];
typedef char BoundVmSecondListOffset[(offsetof(BoundVmListsView, second) == 0x7b5634) ? 1 : -1];

// Both renderer lists are searched in order. Node payloads are not nullable.
static __forceinline BoundVmView *FindBoundVm(unsigned int id)
{
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id)
            return node->vm;
    lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id)
            return node->vm;
    return 0;
}

void __stdcall ReleaseVmBindingBlock(VmBindingBlockView *self);
