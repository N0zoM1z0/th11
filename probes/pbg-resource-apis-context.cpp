#include "PbgResFileRuntime.hpp"

// Compile-only dynamic consumer retaining both mounted-archive and physical
// Pbg disk-file branches without specializing filename or result storage.
extern "C" void *__stdcall PbgResourceApisProbe(const char *filename, int mode)
{
    if (mode == 0) return ReadPbgResourceByName(filename);
    return reinterpret_cast<void *>(GetPbgResourceSizeByName(filename));
}
