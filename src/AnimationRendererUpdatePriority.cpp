#include "AnimationRendererDispatch.hpp"

// Original TH11 draw/update-chain wrapper entries. Their registration
// priority differs from the indexed layer used by draw callbacks.
int __fastcall AnimationRendererUpdatePriority26(void *context)
{
    return UpdateAnimationRenderer(static_cast<AnimationRenderer *>(context));
}

int __fastcall AnimationRendererUpdatePriority08(void *context)
{
    return UpdateOtherAnimationRenderer(static_cast<AnimationRenderer *>(context));
}
