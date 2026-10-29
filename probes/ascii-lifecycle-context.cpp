#include "AsciiLifecycle.hpp"
#include "ChainRegistration.hpp"
#include <new>

extern "C" int __stdcall AsciiLifecycleProbe(AsciiManagerObject *first,
    AsciiManagerObject *second, ChainElement *element, int priority)
{
    new (first) AsciiManagerObject;
    new (second) AsciiManagerObject;
    int result = InitializeAsciiManager(first) + InitializeAsciiManager(second);
    first->AsciiManagerObject::~AsciiManagerObject();
    second->AsciiManagerObject::~AsciiManagerObject();
    return result + AddToUpdateChain(element, priority) + AddToDrawChain(element, priority);
}


#include "AnimationSpriteBinding.hpp"
#include "ErrorLog.hpp"

extern "C" __declspec(dllexport) AnimationDocumentView *__stdcall AsciiDocumentProbe(int index, const char *filename)
{
    return AcquireAnimationDocument(index, filename);
}

extern "C" __declspec(dllexport) const char *__stdcall AsciiErrorProbe(ErrorLogBufferView *log, const char *format, int value)
{
    return AppendErrorLog(log, format, value);
}

extern "C" __declspec(dllexport) int __stdcall AsciiSpriteProbe(AnimationDocumentView *file, int sprite, AnimationVmView *vm)
{
    return BindAnimationSprite(file, sprite, vm);
}
