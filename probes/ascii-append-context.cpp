// Non-runnable compiler context. Independent calls keep all three inputs
// unknown while VC9 chooses its private interprocedural calling convention.
#include "../src/AsciiAppend.hpp"

extern "C" void __stdcall AsciiAppendProbe(AsciiManagerView *a, const float *positionA,
                                          const char *textA, AsciiManagerView *b,
                                          const float *positionB, const char *textB)
{
    AsciiManagerAppendString(a, positionA, textA);
    AsciiManagerAppendString(b, positionB, textB);
}
