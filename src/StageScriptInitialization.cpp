#include "StageScript.hpp"

// One variable-length ANM command in a stage object's local script stream.
// The VM index at +6 is populated as the stage creates its animation VMs.
struct StageObjectVmCommand
{
    short type;
    short byteSize;
    short scriptId;
    short vmIndex;
};

typedef char StageObjectVmCommandSize[(sizeof(StageObjectVmCommand) == 8) ? 1 : -1];

// Prepare the three stage callbacks and the object VM streams before the
// stage interpreter begins. A negative command type terminates each stream.
void __cdecl InitializePrimaryStageScripts()
{
    StageResourceView *stage = g_StagePrimary;
    stage->updateChain->flags |= 2;
    stage->drawChain->flags |= 2;
    stage->overlayChain->flags |= 2;

    int vmIndex = 0;
    for (int objectIndex = 0; objectIndex < stage->data->objectCount; ++objectIndex)
    {
        stage->objects[objectIndex][3] = 1;
        StageObjectVmCommand *command = reinterpret_cast<StageObjectVmCommand *>(
            stage->objects[objectIndex] + 0x1c);
        while (command->type >= 0)
        {
            AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
                reinterpret_cast<unsigned char *>(stage->vms) + vmIndex * sizeof(AnimationVmView));
            int scriptId = command->scriptId;
            AnimationFileView *file = reinterpret_cast<AnimationFileView *>(stage->animation);
            ResetAnimationVm(vm);
            reinterpret_cast<unsigned char *>(vm)[0x41d] = 0x10;
            reinterpret_cast<unsigned char *>(vm)[0x41c] = 0x10;
            BindAnimationScript(file, scriptId, vm);
            command->vmIndex = static_cast<short>(vmIndex);
            ++vmIndex;
            command = reinterpret_cast<StageObjectVmCommand *>(
                reinterpret_cast<unsigned char *>(command) + command->byteSize);
        }
    }
    StageScriptStateView &script = *reinterpret_cast<StageScriptStateView *>(stage);
    script.instruction = reinterpret_cast<StageScriptInstruction *>(stage->section008);
}
