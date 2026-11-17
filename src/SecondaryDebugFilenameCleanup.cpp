#include "SecondaryGameplay.hpp"
#include <stdlib.h>

// Native 0x40CFE0..0x40D032: the secondary ECL debug owner keeps an
// array of separately allocated script/file strings at +0x34.
// All entries are released before the array itself, without inventing
// a clear/reallocation step or a nonnegative-count fallback.
void __stdcall ReleaseSecondaryDebugFilenames(SecondaryEclDebugView *self)
{
    if (self->filenames)
    {
        for (int index = 0; index < self->fileCount; ++index)
        {
            char *filename = self->filenames[index];
            if (filename)
            {
                free(filename);
                self->filenames[index] = 0;
            }
        }
        if (self->filenames)
        {
            free(self->filenames);
            self->filenames = 0;
        }
        self->filenames = 0;
    }
}
