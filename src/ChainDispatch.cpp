#include "Chain.hpp"
#include <windows.h>

extern unsigned int g_InputRuntimeFlags;
extern CRITICAL_SECTION g_ChainCriticalSection;
extern unsigned char g_ChainCriticalDepth;

namespace
{
__forceinline void EnterChain()
{
    if (g_InputRuntimeFlags & 0x8000)
    {
        EnterCriticalSection(&g_ChainCriticalSection);
        ++g_ChainCriticalDepth;
    }
}

__forceinline void LeaveChain()
{
    if (g_InputRuntimeFlags & 0x8000)
    {
        LeaveCriticalSection(&g_ChainCriticalSection);
        --g_ChainCriticalDepth;
    }
}
}

int ExecuteDrawChain()
{
    ChainManagerView *manager = g_ChainManager;
    EnterChain();
    int count = 0;
    ChainNode *node = manager->drawHead.link.next;
    while (node)
    {
        ChainNode *next = node->next;
        ChainElement *element = node->element;
        if (element->callback)
        {
executeAgain:
            if (element->flags & 2)
            {
                LeaveChain();
                int result = element->callback(element->context);
                EnterChain();
                switch (result)
                {
                case 0:
                    UnregisterChainElement(element, manager);
                    break;
                case 2:
                    goto executeAgain;
                case 4:
                    count = 0;
                    goto loopExit;
                case 3:
                    count = 1;
                    goto loopExit;
                case 5:
                    count = -1;
                    goto loopExit;
                }
            }
            ++count;
        }
        node = next;
    }
loopExit:
    LeaveChain();
    return count;
}

// The target's internal entry keeps the manager in EBX. This ordinary member
// declaration remains a source view until its linked calling context is proved.
int ChainManagerView::ExecuteUpdateChain()
{
    ChainNode *node;
    ChainNode *next;
    ChainElement *element;
    int count;
    int result;

    EnterChain();
restart:
    count = 0;
    node = updateHead.link.next;
    while (node)
    {
        next = node->next;
        element = node->element;
        if (element->callback)
        {
executeAgain:
            if (element->flags & 2)
            {
                if (updatesPaused)
                    goto auxiliary;
                LeaveChain();
                result = element->callback(element->context);
                EnterChain();
                switch (result)
                {
                case 0:
                    UnregisterChainElement(element, this);
                    ++count;
                    node = next;
                    continue;
                case 2:
                    goto executeAgain;
                case 4:
                case 8:
                    count = 0;
                    goto loopExit;
                case 3:
                    count = 1;
                    goto loopExit;
                case 5:
                    count = -1;
                    goto loopExit;
                case 6:
                    goto restart;
                case 7:
auxiliary:
                    if (element->auxiliaryCallback)
                        element->auxiliaryCallback(element->context);
                    break;
                }
            }
            ++count;
        }
        node = next;
    }
loopExit:
    LeaveChain();
    return count;
}
