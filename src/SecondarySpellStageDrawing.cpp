#include "SecondarySpellStage.hpp"
#include "SecondaryActorRuntime.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AsciiLifecycle.hpp"
#include "AsciiFormatting.hpp"

// Reuse the mutable native record database at 0x4A8EBC; only the
// record fields read by this draw owner have identified offsets.
extern unsigned char *g_StageSpellRecordData;  // 0x004A8EBC

// Complete native 0x40C4A0..0x40C611 drawing owner. The original
// compiler passes self in EAX; a natural C++ signature leaves that
// private /GL ABI to a whole-caller producer.
int __stdcall DrawSecondarySpellStage(SecondarySpellStageView *self)
{
    if (!(self->flags & 1u))
        return 1;

    DrawAnimationVmByMode(g_RenderBatch, &self->vm0);
    DrawAnimationVmByMode(g_RenderBatch, &self->vm1);

    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->followerIds[2]));
    if (!vm)
    {
        self->followerIds[2] = 0;
        return 1;
    }

    AsciiManagerView *ascii = reinterpret_cast<AsciiManagerView *>(g_AsciiManager);
    ascii->unknown1849c = 1;
    ascii->unknown18498 = 2;
    reinterpret_cast<unsigned char *>(&ascii->color)[3] =
        reinterpret_cast<const unsigned char *>(&vm->spriteIndex)[3];

    float position[3] = {0.0f, 51.0f, 0.0f};
    if (self->flags & 2u)
    {
        position[0] = 265.0f;
        AsciiManagerPrint(ascii, position, "%.8d", self->score);
    }
    else
    {
        position[0] = 279.0f;
        AsciiManagerPrint(ascii, position, "$");
    }

    position[0] = 360.0f;
    const int category = g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1;
    unsigned char *record = g_StageSpellRecordData +
        category * 0x68d4 + self->spellIndex * 0x90;
    const int first = *reinterpret_cast<const int *>(record + 0x6ec);
    const int second = *reinterpret_cast<const int *>(record + 0x6f0);
    AsciiManagerPrint(ascii, position, "%.2d/%.2d", first, second);

    ascii = reinterpret_cast<AsciiManagerView *>(g_AsciiManager);
    ascii->unknown18498 = 0;
    ascii->unknown1849c = 0;
    reinterpret_cast<unsigned char *>(&ascii->color)[3] = 0xff;
    return 1;
}
