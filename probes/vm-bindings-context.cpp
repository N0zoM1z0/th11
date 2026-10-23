#include "VmBindings.hpp"

extern "C" void __stdcall VmBindingsProbe(VmBindingBlockView *first, VmBindingBlockView *second)
{
    ReleaseVmBindingBlock(first);
    ReleaseVmBindingBlock(second);
}
