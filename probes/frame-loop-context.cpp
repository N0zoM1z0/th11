// Non-runnable whole-family context; no product startup or simulated callbacks.
#include "../src/FrameLoop.hpp"
#include "../src/Chain.hpp"

extern "C" int __stdcall FrameLoopProbe(FrameLoopView *first, FrameLoopView *second,
    MainRuntimeView *runtime, RuntimeThreadView *thread, ChainManagerView *manager)
{
    StopRuntimeThread(thread);
    int callbacks = manager->ExecuteUpdateChain();
    int a = RunFrame(first);
    int b = RunFrame(second);
    return callbacks + a + b + DisableRuntimeFog(runtime);
}
