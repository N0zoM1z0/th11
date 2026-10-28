#include "StageResource.hpp"
#include "ChainRegistration.hpp"
#include <new>

extern "C" int __stdcall StageLifecycleProbe(StageResourceView *first,
    StageResourceView *second, StageEmbeddedVm *vm, const char *filename,
    ChainElement *independentElement, int priority)
{
    new (first) StageResourceView;
    new (second) StageResourceView;
    int result = InitializeStageResource(first, filename);
    result += InitializeStageResource(second, filename);
    first->~StageResourceView();
    second->~StageResourceView();
    new (vm) StageEmbeddedVm;
    vm->~StageEmbeddedVm();
    StageResourceView *created = CreateStageResource(filename);
    delete created;
    result += AddToUpdateChain(independentElement, priority);
    return result;
}
