#include "PauseContinueMenu.hpp"
#include "ChainRegistration.hpp"
#include "StageScript.hpp"

PauseContinueMenuView *__cdecl CreatePauseSession();
void __cdecl DeletePauseSession();
int __fastcall PauseSessionUpdateCallback(PauseContinueMenuView *self);
int __fastcall PauseSessionDrawCallback(PauseContinueMenuView *self);

// Diagnostic only: create, run and delete the session the way the game does,
// with generic calls to the shared helpers.
extern "C" int __stdcall PauseSessionProbe(ChainElement *external, int priority,
    StageTimerView *timer, TitleMenuRecordView *record)
{
    int result = CreatePauseSession() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    if (g_SecondaryPauseSession)
        result += PauseSessionUpdateCallback(g_SecondaryPauseSession)
            + PauseSessionDrawCallback(g_SecondaryPauseSession);
    DeletePauseSession();
    result += AdvanceStageTimer(timer);
    delete record;
    return result;
}
