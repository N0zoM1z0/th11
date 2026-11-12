#include "TitleMenuLifecycle.hpp"
extern "C" int __cdecl TitleMenuResourcesProbe()
{
    return InitializeTitleMenuResources();
}
extern "C" TitleMenuView *__cdecl TitleMenuStartupProbe()
{
    return StartTitleMenu();
}
extern "C" void __cdecl TitleMenuReleaseProbe()
{
    DeleteTitleMenu();
}
