#include "StageAuxiliary.hpp"
#include "VmBindings.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"

// Five runtime script IDs stored at 0x004A3DFC: original image initially
// contains 48, 50, 52, 54 and 56. Keep the table externally writable.
extern int g_StageAuxiliaryMode4ScriptIds[5];

struct StageMode4VmPlacement
{
    unsigned char unknown000[0x3dc];
    CameraVector local;
    CameraVector position;
    CameraVector translation;
};

typedef char StageMode4LocalOffset[
    (offsetof(StageMode4VmPlacement, local) == 0x3dc) ? 1 : -1];
typedef char StageMode4PositionOffset[
    (offsetof(StageMode4VmPlacement, position) == 0x3e8) ? 1 : -1];
typedef char StageMode4TranslationOffset[
    (offsetof(StageMode4VmPlacement, translation) == 0x3f4) ? 1 : -1];

// Complete 390-byte native 0x4073B0..0x407535 effect. The original walks
// both renderer ID lists and starts each related-node traversal at the VM's
// embedded drawNode, not at its unrelated +0x14 binding-view pointer.
int __stdcall UpdateStageAuxiliaryMode4Effect(StageAuxiliaryView *self)
{
    for (int scriptIndex = 0; scriptIndex < 5; ++scriptIndex)
    {
        const unsigned int id = self->animationId040;
        AnimationVmView *root = id ?
            reinterpret_cast<AnimationVmView *>(FindBoundVm(id)) : 0;
        if (!root)
            self->animationId040 = 0;

        // Native forms root+0x10 unconditionally. Do not insert a new null
        // fallback or replace this with the unrelated bound-VM child list.
        for (AnimationVmNode *node = &root->drawNode; node; node = node->next)
        {
            AnimationVmView *child = node->vm;
            if (static_cast<short>(child->scriptId) !=
                g_StageAuxiliaryMode4ScriptIds[scriptIndex])
                continue;
            if (child)
            {
                const StageMode4VmPlacement &placement =
                    *reinterpret_cast<const StageMode4VmPlacement *>(child);
                CameraVector part = {
                    static_cast<float>(placement.local.x + placement.position.x),
                    static_cast<float>(placement.local.y + placement.position.y),
                    static_cast<float>(placement.local.z + placement.position.z)
                };
                CameraVector center = {
                    static_cast<float>(part.x + placement.translation.x),
                    static_cast<float>(part.y + placement.translation.y),
                    static_cast<float>(part.z + placement.translation.z)
                };
                center.x = static_cast<float>(center.x - 224.0);
                center.y = static_cast<float>(center.y - 16.0);

                const unsigned int mask = (~SecondaryActorSpell()->flags) & 1u;
                ClearSceneProjectilesInRadius(&center, child->scaleY,
                    static_cast<int>(mask), 1);
                const unsigned int nextMask = (~SecondaryActorSpell()->flags) & 1u;
                ClearSecondaryObjectsInRadius(&center, child->scaleY,
                    static_cast<int>(nextMask), 1);
            }
            break;
        }
    }
    return 0;
}
