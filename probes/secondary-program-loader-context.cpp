#include "SecondaryProgramBrowser.hpp"
// Independent, non-runnable registration/call probe: keep both original
// browser loader functions materialized under pinned VC9 /GL. Only the
// 8-byte callback has an exact acceptance unit.
extern "C" int __stdcall SecondaryProgramLoaderProbe(SecondaryProgramBrowserView *self)
{
    return LoadSecondaryProgramSelection() + SecondaryProgramBrowserLoadThread();
}
