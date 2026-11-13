#include "PbgDirectoryLoader.hpp"
#include "PbgResourceSession.hpp"

// Independent dynamic physical session caller to avoid trivially fixed
// file paths or a synthetic resource table in the original large loader.
extern "C" int __stdcall PbgDirectoryLoaderProbe(PbgResourceSessionView *self,
    const char *filename, int action)
{
    if (action == 0) return LoadPbgSessionDirectory(self,filename);
    if (action == 1) return OpenPbgFileSession(self,filename);
    return OpenPbgResFileSession(self,filename);
}
