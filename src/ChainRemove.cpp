#include "Chain.hpp"
#include <new>

void __fastcall UnregisterChainElement(ChainElement *element, ChainManagerView *manager)
{
    if (!element)
        return;
    ChainNode *node = &manager->updateHead.link;
    while (node)
    {
        if (node->element == element)
            goto found;
        node = node->next;
    }
    node = &manager->drawHead.link;
    while (node)
    {
        if (node->element == element)
            goto found;
        node = node->next;
    }
    return;
found:
    ChainNode *previous = node->previous;
    if (!previous)
        return;
    if (node->next)
        node->next->previous = previous;
    if (node->previous)
        node->previous->next = node->next;
    node->next = 0;
    node->previous = 0;
    element->callback = 0;
    if (element->flags & 1)
    {
        element->callback = 0;
        element->unknown0c = 0;
        element->auxiliaryCallback = 0;
        operator delete(element);
    }
}
