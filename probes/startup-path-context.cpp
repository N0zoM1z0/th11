// Non-runnable startup call context; both complete bodies are retained.
#include "../src/StartupPath.hpp"

extern "C" int __stdcall StartupPathProbe()
{
    return CheckStartupPath();
}
