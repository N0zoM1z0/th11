#include "../src/Chain.hpp"

int __cdecl ExecuteDrawChain();

// Non-runnable link context: the draw-chain dispatcher and a direct removal.
extern "C" int __stdcall ChainRemoveProbe(ChainElement *element)
{
    UnregisterChainElement(g_ChainManager, element);
    return ExecuteDrawChain();
}
