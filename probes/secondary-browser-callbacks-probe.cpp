#include "SecondaryProgramBrowser.hpp"
int __fastcall SecondaryProgramBrowserDrawCallback(SecondaryProgramBrowserView *);
int __fastcall SecondaryProgramBrowserUpdateCallback(SecondaryProgramBrowserView *);
unsigned int __cdecl SecondaryProgramBrowserStorageSize();
extern "C" unsigned int __stdcall SecondaryProgramBrowserCallbacksProbe(
    SecondaryProgramBrowserView *browser, int branch)
{
    if (branch == 2) return SecondaryProgramBrowserUpdateCallback(browser);
    return branch ? SecondaryProgramBrowserDrawCallback(browser) : SecondaryProgramBrowserStorageSize();
}
