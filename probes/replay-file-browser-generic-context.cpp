#include "ReplayFileBrowserLifetime.hpp"
#include "ChainRegistration.hpp"

// Real dynamic-priority registration caller. Keeps original chain registration
// functions generic when inlining the independent 230-byte replay browser setup.
extern "C" int __stdcall ReplayFileBrowserGenericProbe(
    ReplayFileBrowserView *owner, ChainElement *outside, int priority)
{
    int result = InitializeReplayFileBrowser(owner);
    if (outside) result += AddToUpdateChain(outside, priority);
    result += AddToDrawChain(outside, priority);
    return result;
}
