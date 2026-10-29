// Independent diagnostic arguments; this image is never executed.
#include "AnimationDocument.hpp"
extern "C" void __stdcall AnimationTraceProbe(const char *message, int value)
{
    TraceAnimationLoad(message, value);
}
