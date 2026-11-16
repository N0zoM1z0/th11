#include "../src/StageScript.hpp"

// Independent linked-LTCG diagnostic context for the 249-byte stage VM setup.
// Keep the real function as the producer rather than hard-coding its register ABI.
void __cdecl InitializePrimaryStageScripts();
extern "C" __declspec(dllexport) void __stdcall StageScriptInitializationProbe()
{
    InitializePrimaryStageScripts();
}
