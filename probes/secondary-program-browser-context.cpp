#include "SecondaryProgramBrowser.hpp"
// Non-runnable diagnostic with independent pointer values from an unknown
// callback owner. Avoid baking in assumed menu producer code.
extern "C" int __stdcall SecondaryProgramBrowserProbe(SecondaryProgramBrowserView *self)
{
    int result = UpdateSecondaryProgramBrowser(self);
    ClearSecondaryProgramBrowserFiles(self);
    return result;
}
