#include "PauseContinueMenu.hpp"
#include "ChainRegistration.hpp"
#include "GameInf.hpp"
#include "InputBackend.hpp"
#include "MainRuntime.hpp"
#include "StageScript.hpp"
#include <new>
#include <string.h>

// The in-game pause session published at 0x4A8EB0 (0x2E0 bytes): the pause,
// continue and game-over menus share one PauseMenuView whose state selects
// the active menu.
PauseContinueMenuView *g_SecondaryPauseSession;

int __cdecl IsSecondaryDialogueStateFlag2Set();
void __stdcall EnterSecondaryPauseMenu();
int __stdcall InitializePauseSession(PauseContinueMenuView *self);
int __stdcall DrawPauseSession(PauseContinueMenuView *self);
int __fastcall PauseSessionUpdateCallback(PauseContinueMenuView *self);
int __fastcall PauseSessionDrawCallback(PauseContinueMenuView *self);

namespace
{
struct PauseChainElement : ChainElement
{
    PauseChainElement()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline ChainElement *RegisterPauseChain(PauseContinueMenuView *self, ChainCallback callback, int priority, bool draw)
{
    PauseChainElement *chain = new PauseChainElement;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = self;
    chain->flags = (chain->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(chain, priority);
    else AddToUpdateChain(chain, priority);
    return chain;
}

__forceinline void UnregisterPauseChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element) return;
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(manager, element);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}

__forceinline void InitializeSelection(SecondaryDebugSelectionView &selection)
{
    *reinterpret_cast<unsigned int *>(&selection.unknown00c[0x80]) = 0;
    selection.current = 0;
    selection.excludedCount = 0;
    selection.wrapEnabled = 1;
    selection.count = 999;
}

__forceinline ChainElement *&UpdateElement(PauseContinueMenuView *self)
{
    return *reinterpret_cast<ChainElement **>(&self->pause.unknown008[0]);
}

__forceinline ChainElement *&DrawElement(PauseContinueMenuView *self)
{
    return *reinterpret_cast<ChainElement **>(&self->pause.unknown008[4]);
}
}

// 0x42BBF0
PauseContinueMenuView::PauseContinueMenuView()
{
    InitializeSelection(pause.selection);
    InitializeSelection(pause.keyboard);
    memset(this, 0, sizeof(*this));
    pause.unknown000 |= 2;
    g_SecondaryPauseSession = this;
}

// 0x42BC60: update priority 9, draw priority 58.
int __stdcall InitializePauseSession(PauseContinueMenuView *self)
{
    UpdateElement(self) = RegisterPauseChain(self,
        reinterpret_cast<ChainCallback>(PauseSessionUpdateCallback), 9, false);
    DrawElement(self) = RegisterPauseChain(self,
        reinterpret_cast<ChainCallback>(PauseSessionDrawCallback), 58, true);
    self->pause.time.SetFrame(-1);
    self->pause.secondaryTime.SetFrame(-1);
    return 0;
}

// 0x42BDC0
PauseContinueMenuView::~PauseContinueMenuView()
{
    UnregisterPauseChain(UpdateElement(this));
    UnregisterPauseChain(DrawElement(this));
    for (int i = 0; i < 25; ++i)
        if (pause.replays[i])
            delete pause.replays[i];
    g_SecondaryPauseSession = 0;
}

// 0x42BEA0
PauseContinueMenuView *__cdecl CreatePauseSession()
{
    PauseContinueMenuView *self = new PauseContinueMenuView;
    if (InitializePauseSession(self))
    {
        if (self) delete self;
        return 0;
    }
    return self;
}

// 0x42BF30: state 0 waits for the pause key, 1..11 run the pause menu,
// 12..19 the continue menu and 20..26 the game-over menu.
int PauseContinueMenuView::Update()
{
    switch (pause.state)
    {
    case 0:
        if (!(g_StageResourceFlags & 0x20)
            && ((g_InputState.pressed & 0x100) || (g_MainRuntime.flags & 0x10))
            && IsSecondaryDialogueStateFlag2Set() && g_Game->time014 >= 30)
            EnterSecondaryPauseMenu();
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
        UpdatePauseMenu(&pause);
        break;
    case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
        UpdatePauseContinueMenu(this);
        break;
    case 20: case 21: case 22: case 23: case 24: case 25: case 26:
        UpdatePauseGameOverMenu(this);
        break;
    }
    AdvanceStageTimer(&pause.time);
    AdvanceStageTimer(&pause.secondaryTime);
    return 1;
}

// 0x42C600
int __fastcall PauseSessionUpdateCallback(PauseContinueMenuView *self)
{
    return self->Update();
}

// 0x42C610
int __fastcall PauseSessionDrawCallback(PauseContinueMenuView *self)
{
    return DrawPauseSession(self);
}

// 0x42BF10
void __cdecl DeletePauseSession()
{
    PauseContinueMenuView *self = g_SecondaryPauseSession;
    if (self) delete self;
}
