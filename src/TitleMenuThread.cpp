#include "TitleMenuLifecycle.hpp"
#include "MenuController.hpp"
#include "AnimationDocument.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"
#include <new>

// Physical loading/scene object: only the completion counter is observed.
struct TitleMenuLoadingView
{
    unsigned char unknown000[0x474];
    int progress;
};
extern TitleMenuLoadingView *g_TitleMenuLoading;
extern unsigned int g_TitleMenuStartupErrorFlags;
extern int g_TitleMenuThreadState;
extern TitleMenuView *g_TitleMenu;

// Native 0x4394F0..0x439594: initialize chains/assets, wait for the runtime
// load milestone unless graphics/device shutdown interrupts, then release
// the loader-only animation document and enable the update callback.
unsigned int __stdcall TitleMenuThread(void *context)
{
    if (InitializeTitleMenuResources() != 0)
    {
        g_TitleMenuStartupErrorFlags = ((~(g_MainRuntime.flags >> 13)) & 1u) | 2u;
        return 0;
    }
    if (g_TitleMenuLoading)
    {
        while (g_TitleMenuLoading->progress < 180 && !(g_MainRuntime.flags & 0x180))
            Sleep(16);
        AnimationDocumentSlotsView *slots =
            reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
        AnimationDocumentView *file = slots->files[1];
        if (file)
        {
            DestroyAnimationDocument(file);
            ::operator delete(file);
            slots->files[1] = 0;
        }
    }
    g_TitleMenu->updateChain->flags |= 2;
    g_TitleMenuThreadState = 1;
    return 0;
}
