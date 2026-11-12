#include "SecondaryGameplay.hpp"
#include "SecondaryActorState.hpp"
#include "SecondaryPlayerController.hpp"

// Original names/classes for these four distinct debug startup callees
// remain unknown. Keep their individually reviewed native boundaries.
void __cdecl SecondaryDebugInitialize_00408710();
void __cdecl SecondaryDebugInitialize_00406440();
void __cdecl SecondaryDebugInitialize_00423440();
void __cdecl SecondaryDebugInitialize_00437F60();
void __cdecl SecondaryDebugFinish_0041B2A0();
SecondaryMasterView *__stdcall CreateSecondaryMaster(const char *filename);
SecondaryPlayerControllerView *__cdecl CreateSecondaryPlayerController();
extern SecondaryEclDebugView *g_SecondaryEclDebug;

// Full original 0x40D500..0x40D56E, the 111-byte secondary ECL thread
// session loader. The neighboring 10-byte thread entry is a separate owner.
unsigned int __cdecl LoadSecondaryDebugSessionBody()
{
    SecondaryEclDebugView *session = g_SecondaryEclDebug;
    CreateSecondaryPlayerController();
    SecondaryDebugInitialize_00408710();
    SecondaryDebugInitialize_00406440();
    SecondaryDebugInitialize_00423440();
    SecondaryDebugInitialize_00437F60();
    CreateSecondaryMaster(session->filenames[session->selections[1].current]);
    SecondaryDebugFinish_0041B2A0();
    session->selections[2].count = g_SecondaryMaster->program->subroutineCount;
    MoveSecondaryDebugSelection(&session->selections[2], 1);
    MoveSecondaryDebugSelection(&session->selections[2], -1);
    session->state = 1;
    return 0;
}
