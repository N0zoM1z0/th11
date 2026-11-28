#include "AsciiLifecycle.hpp"
#include "ChainRegistration.hpp"
#include <new>

AsciiManagerObject *__cdecl CreateAsciiManager();

// Creates and deletes the manager the way the game does (0x4013F0 and the
// teardown's inlined delete) and calls the chain helpers with generic values.
extern "C" int __stdcall AsciiLifecycleProbe(AsciiManagerObject *first,
    AsciiManagerObject *second, ChainElement *element, int priority)
{
    int result = CreateAsciiManager() != 0;
    delete g_AsciiManager;
    if (first == second) result += CreateAsciiManager() != 0;
    return result + AddToUpdateChain(element, priority) + AddToDrawChain(element, priority);
}


#include "AnimationSpriteBinding.hpp"
#include "ErrorLog.hpp"

extern "C" __declspec(dllexport) AnimationDocumentView *__stdcall AsciiDocumentProbe(int index, const char *filename)
{
    return AcquireAnimationDocument(index, filename);
}


extern "C" __declspec(dllexport) int __stdcall AsciiSpriteProbe(AnimationDocumentView *file, int sprite, AnimationVmView *vm)
{
    return BindAnimationSprite(file, sprite, vm);
}
