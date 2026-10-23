#include "ChainRegistration.hpp"
#include "MainRuntime.hpp"

namespace
{
__forceinline void EnterChain()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
}

__forceinline void LeaveChain()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

int __stdcall AddToUpdateChain(ChainElement *element, int priority)
{
    ChainManagerView *manager = g_ChainManager;
    int result = 0;
    if (element->unknown0c)
    {
        result = element->unknown0c(element->context);
        element->unknown0c = 0;
    }
    EnterChain();
    element->priority = priority;
    ChainNode *current = &manager->updateHead.link;
    while (current->next)
    {
        if (current->next->element->priority >= priority)
            break;
        current = current->next;
    }
    ChainNode *node = &element->link;
    if (current->next)
    {
        node->next = current->next;
        current->next->previous = node;
    }
    current->next = node;
    node->previous = current;
    LeaveChain();
    return result;
}

int __stdcall AddToDrawChain(ChainElement *element, int priority)
{
    ChainManagerView *manager = g_ChainManager;
    int result = 0;
    if (element->unknown0c)
    {
        result = element->unknown0c(element->context);
        element->unknown0c = 0;
    }
    EnterChain();
    element->priority = priority;
    ChainNode *current = &manager->drawHead.link;
    while (current->next)
    {
        if (current->next->element->priority >= priority)
            break;
        current = current->next;
    }
    ChainNode *node = &element->link;
    if (current->next)
    {
        node->next = current->next;
        current->next->previous = node;
    }
    current->next = node;
    node->previous = current;
    LeaveChain();
    return result;
}
