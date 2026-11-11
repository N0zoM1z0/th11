#include "SecondaryProgramBrowser.hpp"

// Native TH11 0045DB90..0045DBDC: constructs the browser's secondary
// script manager, binds its program and resolves its initial subroutine.
// The real VM current-pointer lifetime remains the original class contract.
SecondaryManager *__stdcall CreateSecondaryManager(
    SecondaryProgramView *program, const char *subroutineName)
{
    SecondaryManager *manager = new SecondaryManager;
    manager->program = program;
    SecondaryInstruction *instruction = FindSecondarySubroutine(program, subroutineName);
    manager->current->instruction = instruction;
    manager->current->time = 0.0f;
    return manager;
}
