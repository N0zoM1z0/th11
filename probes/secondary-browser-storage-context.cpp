#include "SecondaryProgramBrowser.hpp"
SecondaryProgramBrowserView *__fastcall ConstructSecondaryProgramBrowserStorage(SecondaryProgramBrowserView *browser);
extern "C" SecondaryProgramBrowserView *__stdcall SecondaryBrowserStorageProbe(
    SecondaryProgramBrowserView *first, SecondaryProgramBrowserView *second)
{
    ConstructSecondaryProgramBrowserStorage(first);
    return ConstructSecondaryProgramBrowserStorage(second);
}
