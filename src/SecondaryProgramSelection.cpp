#include "SecondaryProgramBrowser.hpp"
#include "ResourceFile.hpp"
#include <stdio.h>
#include <stdlib.h>

// Original 0x438910..0x438A24: the browser's deferred file loader. A script
// program is rebuilt after releasing the old manager and prior input blob.
int __cdecl LoadSecondaryProgramSelection()
{
    SecondaryProgramBrowserView *self = g_SecondaryProgramBrowser;
    char filename[MAX_PATH];
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
    int selected = 0;
    if (count == 0)
    {
        // The native empty-program branch keeps the initial zero index.
    }
    else if (count > 0)
    {
        // Select the first subroutine when the program is nonempty.
    }
    else
        selected = static_cast<int>(static_cast<unsigned int>(count) - 1u);
    self->subroutineSelection.current = selected;
    self->state = 1;
    return 0;
}
