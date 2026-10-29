#include "SecondaryActorState.hpp"

void __stdcall ReleaseSecondaryAuxiliaryThreads(SecondaryManager *self)
{
    SecondaryThread *node = self->threads.next;
    while (node)
    {
        SecondaryThread *next = node->next;
        delete node->vm;
        delete node;
        node = next;
    }
}

void __stdcall ResetSecondaryMainThread(SecondaryManager *self)
{
    self->main.flags &= ~1u;
    self->main.time = 0.0f;
    self->main.instruction = 0;
    self->main.owner = self;
    self->main.id = -1;
    self->main.unknown1018 = 0;
    self->current = &self->main;
    self->threads.vm = &self->main;
    self->threads.next = 0;
    self->threads.previous = 0;
}
