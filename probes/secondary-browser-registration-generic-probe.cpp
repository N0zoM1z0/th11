#include "SecondaryProgramBrowser.hpp"
#include "ChainRegistration.hpp"
// Independent caller with live chain elements and priorities. This models
// real generic users without inventing a private register ABI for the owner.
extern "C" int __stdcall SecondaryBrowserRegistrationGenericProbe(
    SecondaryProgramBrowserView *browser, ChainElement *external, int priority)
{
    int result = RegisterSecondaryProgramBrowserChains(browser);
    if (external)
        result += AddToUpdateChain(external, priority);
    return result + AddToDrawChain(external, priority);
}
