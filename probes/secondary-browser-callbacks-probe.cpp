#include "SecondaryProgramBrowser.hpp"
int __fastcall SecondaryProgramBrowserDrawCallback(SecondaryProgramBrowserView *);
unsigned int __cdecl SecondaryProgramBrowserStorageSize();
extern "C" unsigned int __stdcall SecondaryProgramBrowserCallbacksProbe(
    SecondaryProgramBrowserView *browser, int branch)
{
    return branch ? SecondaryProgramBrowserDrawCallback(browser) : SecondaryProgramBrowserStorageSize();
}
