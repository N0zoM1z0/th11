#include "StageAuxiliary.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"

// Only the first of the six mode updaters has maintained source at present.
// Each call has an independently observed native destination; the others
// deliberately remain external until their own complete bodies are recovered.
int __stdcall UpdateSecondaryEffect(MeshEffectControllerView *self);
int __stdcall UpdateSecondarySpecialMode1(StageAuxiliaryView *self);
int __stdcall UpdateSecondarySpecialMode2(StageAuxiliaryView *self);
int __stdcall UpdateSecondarySpecialMode3(StageAuxiliaryView *self);
int __stdcall UpdateSecondarySpecialMode4(StageAuxiliaryView *self);
int __stdcall UpdateSecondarySpecialMode5(StageAuxiliaryView *self);

// Native 0x406620..0x406707: 208 instruction bytes and a six-entry
// compiler-generated jump table. This source has no instruction-byte data.
int __stdcall UpdateStageAuxiliary(StageAuxiliaryView *self)
{
    if (!self->effectActive)
        return 1;

    if (self->effectActive == 1)
    {
        int finished = 0;
        const int mode = static_cast<int>(
            static_cast<unsigned int>(g_MeshEffectSelection0) * 3u +
            static_cast<unsigned int>(g_MeshEffectSelection1));
        switch (mode)
        {
        case 0:
            finished = UpdateSecondaryEffect(reinterpret_cast<MeshEffectControllerView *>(self));
            break;
        case 1:
            finished = UpdateSecondarySpecialMode1(self);
            break;
        case 2:
            finished = UpdateSecondarySpecialMode2(self);
            break;
        case 3:
            finished = UpdateSecondarySpecialMode3(self);
            break;
        case 4:
            finished = UpdateSecondarySpecialMode4(self);
            break;
        case 5:
            finished = UpdateSecondarySpecialMode5(self);
            break;
        }

        if (finished)
        {
            self->effectActive = 0;
            return 1;
        }
    }
    AdvanceStageTimer(&self->timer014);
    return 1;
}
