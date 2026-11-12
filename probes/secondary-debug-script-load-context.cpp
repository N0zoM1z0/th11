#include "../src/SecondaryGameplay.hpp"
extern unsigned int __cdecl LoadSecondaryDebugSessionBody();
extern "C" unsigned int __stdcall SecondaryDebugScriptLoadProbe()
{
    return LoadSecondaryDebugScript() + LoadSecondaryDebugSessionBody();
}
