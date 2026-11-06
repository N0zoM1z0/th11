#include "SecondaryDialogue.hpp"
#include "StageMesh.hpp"

char *__stdcall DecodeSecondaryDialogueText(const unsigned char *text)
{
    unsigned char key = 0x77, step = 7;
    unsigned char *output = g_SecondaryDialogueDecodedText;
    unsigned char value;
    do
    {
        value = *text++ ^ key;
        key += step;
        *output++ = value;
        step += 0x10;
    } while (value);
    return reinterpret_cast<char *>(g_SecondaryDialogueDecodedText);
}

AnimationVmId __stdcall FindSecondaryDialogueChild(unsigned int *parent, int script)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*parent));
    if (!vm) *parent = 0;
    for (AnimationVmNode *node = &vm->drawNode; node; node = node->next)
    {
        if (static_cast<short>(node->vm->scriptId) == script)
            return AnimationVmId(node->vm->unknown000);
    }
    return AnimationVmId(0);
}

void __stdcall BindSecondaryDialogueSprite(const AnimationVmId &id,
    AnimationDocumentView *file, int sprite)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm) BindAnimationSprite(file, sprite, vm);
}

void __stdcall ChangeSecondaryDialogueSprite(const AnimationVmId &id, int sprite)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm) BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file), sprite, vm);
}
