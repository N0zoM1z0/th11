#include "TitleMenuRecord.hpp"
// Independent callers make both ChainRemove arguments genuinely dynamic.
// This is a compiler context, not an executable substitute for the game.
extern "C" void __stdcall ReplayRecordChainGenericProbe(
    TitleMenuRecordView *record, ChainElement *first,
    ChainElement *second, ChainManagerView *manager)
{
    record->~TitleMenuRecordView();
    UnregisterChainElement(first, manager);
    if (second) UnregisterChainElement(second, manager);
}
