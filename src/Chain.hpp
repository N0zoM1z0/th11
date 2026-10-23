#pragma once

typedef int (__fastcall *ChainCallback)(void *context);

struct ChainElement;
struct ChainNode
{
    ChainElement *element;
    ChainNode *next;
    ChainNode *previous;
};

struct ChainElement
{
    int priority;
    unsigned int flags;
    ChainCallback callback;
    ChainCallback unknown0c;
    ChainCallback auxiliaryCallback;
    ChainNode link;
    void *context;
};

struct ChainManagerView
{
    ChainElement updateHead;
    ChainElement drawHead;
    int updatesPaused;

    int ExecuteUpdateChain();
};

typedef char ChainElementSize[(sizeof(ChainElement) == 0x24) ? 1 : -1];
typedef char ChainNodeSize[(sizeof(ChainNode) == 12) ? 1 : -1];

extern ChainManagerView *g_ChainManager;
void __fastcall UnregisterChainElement(ChainElement *element, ChainManagerView *manager);
int ExecuteDrawChain();
