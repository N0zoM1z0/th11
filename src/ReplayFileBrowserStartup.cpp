#include "ReplayFileBrowserLifetime.hpp"
#include "StageResource.hpp"
#include <new>
#include <string.h>

// Native 0x437F60..0x437FD4, 117 bytes. StageEmbeddedVm has a separate
// exact 101-byte native constructor at 0x402270; the owner also resets 13
// 0x40-stride flags before its final whole-object clear and singleton publish.
ReplayFileBrowserView *__cdecl CreateReplayFileBrowser()
{
    ReplayFileBrowserView *self = static_cast<ReplayFileBrowserView *>(
        ::operator new(sizeof(ReplayFileBrowserView)));
    if (self)
    {
        new (reinterpret_cast<unsigned char *>(self) + 0x18) StageEmbeddedVm;
        for (int i = 0; i <= 12; ++i)
            *reinterpret_cast<unsigned int *>(
                reinterpret_cast<unsigned char *>(self) + 0x478 + i * 0x40) &= ~1u;
        memset(self, 0, sizeof(*self));
        self->flags |= 2u;
        g_ReplayFileBrowser = self;
    }

    if (InitializeReplayFileBrowser(self))
    {
        if (self)
        {
            DestroyReplayFileBrowser(self);
            ::operator delete(self);
        }
        return 0;
    }
    return self;
}
