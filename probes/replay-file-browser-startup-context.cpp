#include "ReplayFileBrowserLifetime.hpp"
extern "C" ReplayFileBrowserView *__cdecl ReplayFileBrowserStartupProbe()
{
    return CreateReplayFileBrowser();
}
