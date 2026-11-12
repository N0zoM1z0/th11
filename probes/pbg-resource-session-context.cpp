#include "PbgResourceSession.hpp"
// All branches consume real dynamic arguments; these are compile-only owner
// producers, not substitutes for the original game or its native Pbg archive.
extern "C" unsigned int __stdcall PbgResourceSessionProbe(
    PbgResourceSessionView *self, const char *filename, int mode)
{
    if (mode == 1) return OpenPbgFileSession(self, filename);
    if (mode == 2) return OpenPbgResFileSession(self, filename);
    self->~PbgResourceSessionView();
    return 0;
}
