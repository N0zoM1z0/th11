#include "SecondaryProgramBrowser.hpp"

// Separate 8-byte thread entry at 0x438900..0x438907. The callback accepts
// no stack parameters and always returns zero after the selection loader.
unsigned int __cdecl SecondaryProgramBrowserLoadThread()
{
    LoadSecondaryProgramSelection();
    return 0;
}
