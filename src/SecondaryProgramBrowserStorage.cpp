#include "SecondaryProgramBrowser.hpp"
#include <string.h>

// 0x4978D0 begins with the original worker vtable/function pointer.
// Its nominal C++ class is not yet known, so preserve the observed address
// dependency as an external data symbol rather than guessing a virtual type.
extern const unsigned int g_SecondaryBrowserWorkerVtable[];
typedef char BrowserStorageSize[(sizeof(SecondaryProgramBrowserView) == 0x2e4) ? 1 : -1];

namespace
{
__forceinline void InitializeBrowserSelection(SecondaryDebugSelectionView *selection)
{
    *reinterpret_cast<unsigned int *>(&selection->unknown00c[0x80]) = 0;
    selection->current = 0;
    selection->excludedCount = 0;
    selection->wrapEnabled = 1;
    selection->count = 999;
}
}

// Native TH11 004385D0..0043865D: construction of the browser's 0x2E4-byte
// storage. In the original, all three embedded selection initializers and
// worker initialization precede the final whole-object memset.
SecondaryProgramBrowserView *__fastcall ConstructSecondaryProgramBrowserStorage(
    SecondaryProgramBrowserView *browser)
{
    browser->worker.unknown00 = reinterpret_cast<unsigned int>(g_SecondaryBrowserWorkerVtable);
    browser->worker.thread = 0;
    browser->worker.unknown08 = 0;
    browser->worker.stopRequested = 0;
    browser->worker.pending = 0;
    InitializeBrowserSelection(&browser->category);
    InitializeBrowserSelection(&browser->fileSelection);
    InitializeBrowserSelection(&browser->subroutineSelection);
    memset(browser, 0, sizeof(*browser));
    *reinterpret_cast<unsigned int *>(browser) |= 2;
    g_SecondaryProgramBrowser = browser;
    return browser;
}
