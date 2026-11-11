#include "SecondaryProgramBrowser.hpp"

// Native 004390B0: the ECL browser's draw-chain adapter preserves ESI while
// passing the callback receiver on to the full debug-text drawing owner.
int __fastcall SecondaryProgramBrowserDrawCallback(SecondaryProgramBrowserView *browser)
{
    return DrawSecondaryProgramBrowser(browser);
}

// Native 004390D0: returns the observed size of the surrounding debug object.
// The object's nominal type and constructor remain independent questions.
unsigned int __cdecl SecondaryProgramBrowserStorageSize()
{
    return 0x5b94;
}
