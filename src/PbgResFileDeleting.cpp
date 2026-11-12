#include "PbgResFileRuntime.hpp"
#include <new>

extern const unsigned int g_PbgIFileVtable[];

// Full native 0x442090..0x4420AE: scalar deleting 16-byte Pbg resource
// stream adapter, with explicit base-vtable lifetime and flag-gated delete.
PbgResFileRuntimeView *PbgResFileRuntimeView::DestroyAndMaybeDelete(unsigned int flags)
{
    vtable = g_PbgIFileVtable;
    if (flags & 1u) ::operator delete(this);
    return this;
}
