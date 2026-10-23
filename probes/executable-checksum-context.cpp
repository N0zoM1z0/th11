// Non-runnable return-value consumer; no runtime/product claim.
#include "../src/ExecutableChecksum.hpp"

extern "C" unsigned int __stdcall ExecutableChecksumProbe()
{
    return ComputeExecutableChecksum();
}
