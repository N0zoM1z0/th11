// Independent renderer/layer inputs retain the general public boundaries.
#include "AnimationRendererDispatch.hpp"
extern "C" int __stdcall AnimationRendererDispatchProbe(int kind,
    AnimationRenderer *renderer, int layer)
{
    switch (kind)
    {
    case 0: return UpdateAnimationRenderer(renderer);
    case 1: return UpdateOtherAnimationRenderer(renderer);
    case 2: return DrawAnimationRendererLayer(renderer, layer);
    case 3: return reinterpret_cast<int>(AllocateSceneAnimationVm());
    }
    return 0;
}
