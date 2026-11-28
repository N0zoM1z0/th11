#include "AnimationDocument.hpp"
#include "StageResource.hpp"

int __cdecl LoadFrontDocuments();
int __cdecl ReleaseFrontDocuments();

// Diagnostic only: call the front document helpers and the shared document
// functions with generic arguments.
extern "C" int __stdcall FrontProbe(int slot, AnimationDocumentView *file)
{
    int result = LoadFrontDocuments() + ReleaseFrontDocuments();
    if (slot >= 0 && slot < 32)
        result += AcquireAnimationDocument(slot, "") != 0;
    DestroyAnimationDocument(file);
    return result;
}
