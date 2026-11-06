#include "MenuController.hpp"
#include "VmBindings.hpp"

void __stdcall MarkMenuFileAnimations(RenderBatchView *renderer, AnimationFileView *file)
{
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(renderer);
    for (BoundVmNodeView *node = lists->first; node; )
    {
        BoundVmNodeView *next = node->next;
        AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(node->vm);
        if (vm->file == file) vm->flags.value |= 0x4000000;
        node = next;
    }
    for (BoundVmNodeView *node = lists->second; node; )
    {
        BoundVmNodeView *next = node->next;
        AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(node->vm);
        if (vm->file == file) vm->flags.value |= 0x4000000;
        node = next;
    }
}
