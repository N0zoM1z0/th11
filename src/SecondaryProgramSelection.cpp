#include "SecondaryProgramBrowser.hpp"
#include "ResourceFile.hpp"
#include <stdio.h>
#include <stdlib.h>

// Original 0x438910..0x438A24: the browser's deferred file loader. A script
// program is rebuilt after releasing the old manager and prior input blob.
int __cdecl LoadSecondaryProgramSelection()
{
    SecondaryProgramBrowserView *self = g_SecondaryProgramBrowser;
    char filename[256];
    sprintf(filename, "%s", self->filenames[self->fileSelection.current]);
    if (self->manager)
    {
        delete self->manager;
        self->manager = 0;
    }
    self->manager = 0;
    if (self->loadedData)
    {
        free(self->loadedData);
        self->loadedData = 0;
    }
    self->loadedData = LoadResourceFile(filename, 0, 0);
    self->program = new SecondaryProgramView;
    self->program->Install(self->loadedData);
    int count = self->program->subroutineCount;
    self->subroutineSelection.count = count;
    self->subroutineSelection.current = count < 0 ? count - 1 : 0;
    self->state = 1;
    return 0;
}
