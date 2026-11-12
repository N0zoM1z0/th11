#include "TitleMenuRecord.hpp"
void __stdcall DestroyTitleMenuRecord(TitleMenuRecordView *record);
// Independent callers make both ChainRemove arguments genuinely dynamic.
// This is a compiler context, not an executable substitute for the game.
extern "C" void __stdcall ReplayRecordChainGenericProbe(
    TitleMenuRecordView *record, ChainElement *first,
    ChainElement *second, ChainManagerView *manager)
{
    DestroyTitleMenuRecord(record);
    UnregisterChainElement(first, manager);
    if (second) UnregisterChainElement(second, manager);
}
