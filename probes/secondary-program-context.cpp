// Independent program, manager and data/name. The diagnostic image is never run.
#include "SecondaryScript.hpp"
extern "C" int __stdcall SecondaryProgramProbe(int kind, SecondaryProgramView *program,
    SecondaryManager *manager, void *data)
{
    switch (kind)
    {
    case 0: return program->SecondaryProgramView::Install(data);
    case 1: return manager->Initialize(program, static_cast<const char *>(data));
    case 2: return manager->Select(program, static_cast<const char *>(data));
    case 3: return manager->Restart(static_cast<const char *>(data));
    case 4: return program->SecondaryProgramView::ProcessExtension(data);
    }
    return 0;
}
