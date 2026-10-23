// Non-runnable repeated context; instances remain independent of global storage.
#include "../src/WindowInitialization.hpp"

extern "C" int __stdcall WindowInitializationProbe(HINSTANCE first, HINSTANCE second)
{
    int result = InitializeMainWindow(first);
    return result + InitializeMainWindow(second);
}
