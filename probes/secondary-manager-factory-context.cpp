#include "SecondaryProgramBrowser.hpp"

// Non-runnable independent caller. The actual ECL browser TU is also linked
// to recover the native private program-in-EDI convention without fake ABI.
extern "C" SecondaryManager *__stdcall SecondaryManagerFactoryProbe(
    SecondaryProgramView *program, const char *name)
{
    return CreateSecondaryManager(program, name);
}
