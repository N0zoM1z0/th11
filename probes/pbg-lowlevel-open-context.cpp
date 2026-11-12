#include "PbgLowLevelFile.hpp"
// Bounded dynamic caller: preserve a real File owner and distinct standalone
// pathname-resolution call without specializing the original mode branches.
extern "C" void __stdcall PbgLowLevelOpenProbe(PbgLowLevelFileView *stream,
    const char *filename, const char *mode, char *resolved, int usePath)
{
    stream->Open(filename, mode);
    if (usePath) ResolvePbgDiskPath(resolved, filename);
}
