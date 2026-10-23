// Non-runnable caller context retaining independent variadic log inputs.
#include "../src/DeviceInitialization.hpp"
#include "../src/ErrorLog.hpp"
#include "../src/RenderBatch.hpp"

extern "C" int __stdcall DeviceInitializationProbe(ErrorLogBufferView *a,
    ErrorLogBufferView *b, const char *format, unsigned int value, RenderBatchView *batch)
{
    AppendErrorLog(a, format, value);
    AppendErrorLog(b, format, value);
    AppendLog(&a->buffer, format, value);
    AppendLog(&b->buffer, format, value);
    FlushRenderBatch(batch);
    return InitializeD3DDevice();
}
