#include "TitleMenuLifecycle.hpp"
extern "C" unsigned int __cdecl TitleMenuThreadProbe()
{
    return TitleMenuThread();
}
