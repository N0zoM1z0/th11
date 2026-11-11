#include "SecondaryGameplay.hpp"
#include "FrameLoop.hpp"
#include <process.h>

// Native 004594A0..004594D7: stop the previous worker before starting a new
// CRT thread with a user-supplied callback and browser/session context.
// The native callback also lives in the extra +0x18 worker field.
void __stdcall StartSecondaryDebugThread(RuntimeThreadView *thread,
    SecondaryDebugThreadCallback callback, void *context)
{
    StopRuntimeThread(thread);
    thread->unknown18 = reinterpret_cast<unsigned int>(callback);
    thread->pending = 1;
    thread->stopRequested = 0;
    thread->thread = reinterpret_cast<HANDLE>(_beginthreadex(
        0, 0, reinterpret_cast<unsigned (__stdcall *)(void *)>(callback),
        context, 0, &thread->unknown08));
}
