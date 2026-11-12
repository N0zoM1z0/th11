#include "TitleMenuLifecycle.hpp"
extern "C" int __cdecl TitleMenuResourcesProbe()
{
    return InitializeTitleMenuResources();
}
