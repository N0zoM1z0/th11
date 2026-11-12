#include "ThreadInfLifecycle.hpp"

// Original 0x459410..0x45941F: non-deleting 16-byte virtual destructor,
// referenced by the native EH unwind routines. With the actual
// StopRuntimeThread producer, VC9 retains ESI as the worker receiver.
ThreadInfView::~ThreadInfView()
{
    StopRuntimeThread(reinterpret_cast<RuntimeThreadView *>(this));
}

// A natural dynamic caller keeps the member destructor in its own complete
// public primary-TU contribution for the bounded /GL comparison.
void __stdcall DestroyThreadInfForUnwind(ThreadInfView *self)
{
    self->~ThreadInfView();
}
