#include "VmBindings.hpp"
#include <stdlib.h>

void __stdcall ReleaseVmBindingBlock(VmBindingBlockView *self)
{
    if (self->storage010)
    {
        free(self->storage010);
        self->storage010 = 0;
    }
    if (self->storage014)
    {
        free(self->storage014);
        self->storage014 = 0;
    }
    for (int i = 0; i < self->count - 1; ++i)
    {
        unsigned int *slot = &self->ids[i];
        if (*slot)
        {
            BoundVmView *vm = FindBoundVm(*slot);
            if (vm)
            {
                vm->flags |= 0x04000000;
                if (!vm->marker018)
                    for (BoundVmNodeView *node = vm->related; node; node = node->next)
                        node->vm->flags |= 0x04000000;
            }
        }
        *slot = 0;
    }
    if (self->ids)
    {
        free(self->ids);
        self->ids = 0;
    }
    if (self->storage00c)
    {
        free(self->storage00c);
        self->storage00c = 0;
    }
}
