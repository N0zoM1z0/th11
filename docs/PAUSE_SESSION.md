# Pause session lifetime (0x4A8EB0)

The in-game pause session is a 0x2E0-byte `PauseContinueMenuView` published
at 0x4A8EB0 (the same object `PauseSecondaryGameplayBoss` pauses). Its
`PauseMenuView` state selects the active menu. Source: `src/PauseSession.cpp`,
context `build/match-linked/PauseSession` (`/EHs`; links `ChainRegistration`,
`ChainRemove`, `StageTimer` and `TitleMenuRecord` for their native private
conventions).

Exact (2026-11-28), all from the shared manager template:

| Address | Function | Notes |
| --- | --- | --- |
| 0x42BBF0 | `PauseContinueMenuView()` | ESI this. Timer member constructors, both selection initializers (+0x8C 0, current 0, excluded 0, wrap 1, count 999, as the program browser storage), then memset, flag 2 and publication |
| 0x42BC60 | `InitializePauseSession` | EDI session. Update priority 9 (0x42C600), draw priority 58 (0x42C610), both timers `SetFrame(-1)` |
| 0x42BDC0 | `~PauseContinueMenuView()` | EAX this. Chains unregistered, the 25 replay records deleted, global cleared |
| 0x42BEA0 | `CreatePauseSession` | Calls by 0x41FC7F |
| 0x42BF10 | `DeletePauseSession` | No direct caller |
| 0x42C610 | `PauseSessionDrawCallback` | Forwards to the session draw 0x42C150 (1,157 bytes, open) |

0x42BEF0 is the compiler's scalar deleting destructor for this class
(dtor, delete, return this; no caller).

The constructor needs `AnimationVmId` to be default-constructible without
writing anything: the native constructor stores nothing at `root`/`backdrop`
before the memset, so `AnimationVmId() {}` is empty.

Frontier: `PauseContinueMenuView::Update` (0x42BF30) dispatches state 0
(open the pause menu when the pause key, bit 0x100 of the pressed input, or
input flag 0x10 is set, the stage flag 0x20 is clear, the dialogue state
allows it and the game has run 30 frames), 1..11 (`UpdatePauseMenu`), 12..19
(`UpdatePauseContinueMenu`) and 20..26 (`UpdatePauseGameOverMenu`), then
advances both timers. The body matches except the session register: native
receives it in EAX and keeps it in EBX (so the update callback 0x42C600 is a
`mov eax, ecx; jmp`), VC9 here passes and keeps it in EDI. Making it a member
function, linking `SecondaryDialogueStateQuery.cpp`, the pause entry and the
three menu sources did not change the choice.
