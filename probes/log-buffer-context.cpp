// Non-runnable formatting callers with unknown destinations and arguments.
#include "../src/LogBuffer.hpp"

extern "C" const char *__stdcall LogBufferProbe(LogBufferView *a, const char *formatA,
                                               LogBufferView *b, const char *formatB,
                                               unsigned int value)
{
    AppendLog(a, formatA, value);
    return AppendLog(b, formatB, value);
}
