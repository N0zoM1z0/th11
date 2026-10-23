#include "AnimationVm.hpp"
#include "RenderBatch.hpp"
#include <string.h>

void __stdcall ResetAnimationVm(AnimationVmView *self)
{
    AnimationPersistentState saved = self->persistent3e8;
    unsigned int saved020 = self->persistent020;
    memset(self, 0, sizeof(*self));
    self->scaleX = 1.0f;
    self->scaleY = 1.0f;
    self->persistent020 = saved020;
    self->persistent3e8 = saved;
    self->spriteIndex = -1;

    D3DMATRIX &m = self->transform;
    m._12 = m._13 = m._14 = m._21 = m._23 = m._24 =
    m._31 = m._32 = m._34 = m._41 = m._42 = m._43 = 0.0f;
    m._11 = m._22 = m._33 = m._44 = 1.0f;
    self->flags.halves.low = 7;
    self->time.ResetFields();
    self->reset0d4 = 0;
    self->reset120 = 0;
    self->reset14c = 0;
    self->reset198 = 0;
    self->reset1d4 = 0;
    self->reset220 = 0;
    self->reset24c = 0;
    self->reset278 = 0;
    self->reset2a4 = 0;
    self->updateNode.Reset(self);
    self->drawNode.Reset(self);
}

void __stdcall BindAnimationScript(AnimationFileView *file, int scriptId, AnimationVmView *self)
{
    if (file->scripts[scriptId] && !file->bindingDisabled)
    {
        ResetAnimationVm(self);
        self->scriptId = static_cast<unsigned short>(scriptId);
        self->flags.value &= ~0x600u;
        self->fileId = file->id;
        self->file = file;
        self->scriptStart = file->scripts[scriptId];
        self->instruction = self->scriptStart;
        self->time.SetFrame(-1);
        self->flags.value &= ~1u;
        ExecuteAnimationVm(self);
        ++*reinterpret_cast<unsigned int *>(g_RenderBatch->unknown000 + 0xa0);
        return;
    }
    memset(self, 0, sizeof(*self));
}
