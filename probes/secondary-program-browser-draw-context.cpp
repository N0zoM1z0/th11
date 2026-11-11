#include "SecondaryProgramBrowser.hpp"
extern "C" int __stdcall SecondaryProgramBrowserDrawProbe(SecondaryProgramBrowserView *browser)
{
    return DrawSecondaryProgramBrowser(browser);
}
