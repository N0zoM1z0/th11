#include "StageResource.hpp"
#include "MenuController.hpp"

// Compile-only direct caller for the full native animation-document release
// owner; the diagnostic linked image is not an executable TH11 replacement.
extern "C" void __stdcall AnimationDocumentDestructorProbe(AnimationDocumentView *file)
{
    DestroyAnimationDocument(file);
}

// The original marker has multiple producer paths; retain an independent
// renderer-driven caller so LTCG cannot specialize its receiver to the
// global singleton used by this one document-destroy caller.
extern "C" void __stdcall AnimationDocumentMarkerGenericProbe(
    RenderBatchView *renderer, AnimationFileView *file)
{
    MarkMenuFileAnimations(renderer, file);
}
