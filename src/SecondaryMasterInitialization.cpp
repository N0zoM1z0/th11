#include "SecondaryActorState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryProgramExtension.hpp"
#include "ChainRegistration.hpp"
#include "StageResource.hpp"
#include <new>

int __fastcall SecondaryMasterUpdateCallback(void *context);
int __fastcall SecondaryMasterDrawCallback(void *context);

namespace
{
struct MasterChainElementWithConstructor : ChainElement
{
    MasterChainElementWithConstructor()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};
__forceinline ChainElement *CreateMasterChainElement(SecondaryMasterView *master,
    ChainCallback callback, int priority, bool update)
{
    ChainElement *element = new MasterChainElementWithConstructor;
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = master;
    element->flags = (element->flags & ~2u) | 1u;
    if (update) AddToUpdateChain(element, priority);
    else AddToDrawChain(element, priority);
    return element;
}
}

// Native complete 0x410BE0..0x410D1D, 318-byte secondary master setup.
// Program constructor/extension owner and the 0x24-byte chain element are
// real C++ objects; original class/TU ownership remains indeterminate.
int __stdcall InitializeSecondaryHost(SecondaryMasterView *self,
    const char *scriptFilename)
{
    *reinterpret_cast<AnimationFileView **>(reinterpret_cast<unsigned char *>(self) + 0x40) =
        g_SecondaryActorFiles->file;
    SecondaryProgramWithExtensions *program = new SecondaryProgramWithExtensions;
    self->program = program;
    program->LoadFile(scriptFilename);
    *reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(self) + 8) =
        CreateMasterChainElement(self, SecondaryMasterUpdateCallback, 18, true);
    *reinterpret_cast<ChainElement **>(reinterpret_cast<unsigned char *>(self) + 12) =
        CreateMasterChainElement(self, SecondaryMasterDrawCallback, 20, false);
    StageTimerView *time = reinterpret_cast<StageTimerView *>(
        reinterpret_cast<unsigned char *>(self) + 0x50);
    time->SetFrame(-1);
    return 0;
}
