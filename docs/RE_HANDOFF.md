# Reconstruction handoff

Follow `AGENTS.md`, `RE_WORKFLOW.md` and `ORACLES.md`. Inspect Git status/diff;
continue existing work. Use `scripts/repo-python`, direct `scripts/ghidra.py`
and the isolated TH11 state. Serialize Ghidra and Wine/compiler jobs.

## Current state

The target remains original Japanese TH11v1.00a, SHA256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
VC9 RTM21022/SDK6.0A is the observed compiler candidate; original TU ownership
remains open. Exact counts come from [PROGRESS](PROGRESS.md); findings and topic
links are in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md).

The continue menu owner at42D9D0..42E647/3192 bytes now merges its case13
factory call after practice/record branches, as the native control flow does.
The current 63-support VC9 /GL owner is **3172 bytes/160 fields/
15 returns**, versus native3192/162/15, with **88/88 call sites** and
**2767 differing full bytes** after reviewed-field rebinding (earlier
3112-byte draft had2797). The source retains native ten IMUL/zero IDIV,
but the local frame54 remains smaller than native58. The earlier3048-byte
draft had158 fields,14 returns and four call-count gaps. A second layout with
two explicit result objects recovered frame58 but regressed call/return parity,
so it was rejected. See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

Rebuild with `probes/pause-continue-menu-context.json`; its output is
`build/probe-ltcg/PauseContinueMenuOwner`. The latest local build/audit helpers,
native reviews, comparison records and checkpoint verifier remain in
`.analysis/`. Older unrelated experiment outputs and verifiers are removed.
Canonical source, ledgers and build/comparison scripts are unchanged.

## Next reconstruction work

The large `DecodeAnimationChunk` owner4545A0/811 is now
**strict cold linked-PE exact 811/811** with 15 reviewed
bindings (eight DIR32 and seven REL32), complete original
nine-byte alignment, and full source/support/toolchain/PE/MAP/PDB
receipts. Natural `script[i].offset` and
fileIndex/texture/textureIndex initialization order restored all
ten formerly mismatched code bytes; no ABI spoofing or opcode
patches. The separately exact 149-byte StoreAnimationSprite is
still an independently protected unit. The 212-byte advance owner
remains non-exact. See ANIMATION_CHUNKS.md.

Animation slot release4549A0/51 is now **cold linked exact 51/51**
with two reviewed REL32 fields (document destroy4549E0, operator
delete45FD49). The adjacent full 135-byte scan4548D0 now has
compiled maintained source, narrow /GL115/three calls/two returns
versus native135/3/2, non-exact. Both are in
AnimationDocumentSlotsLifecycle.cpp; see ANIMATION_DOCUMENT_SLOTS.md.

The 415-byte player-storage destructor now inlines the two original
VM buffer frees in reverse member order rather than calling the
separately exact VM destructor body. In the focused linked context
with exact animation-document cleanup243 and VM marker78, the parent
has **435/native415 bytes, 12/12 callee categories (three direct
frees) and one RET4**. Normal COFF446; remains non-exact.
The 272-byte storage constructor keeps the previous COFF271 layout.
See SECONDARY_PLAYER_INITIALIZATION.md.

Document slot scanner4548D0/135 now produces a full native-length
135-byte primary /GL contribution with three calls/two returns;
one non-field byte remains at offset70 because the 212-byte
AdvanceAnimationDocument callee still accepts EAX instead of
native EDI. Do not claim exact; the independent 51-byte
FreeAnimationDocumentSlot remains strict cold exact after rebuilding.
See ANIMATION_DOCUMENT_SLOTS.md.

Registered menu update callback439F10/7 is now canonical cold
normal-COFF **exact 7/7**, one REL32 to the 1224-byte menu owner4399B0.
Its source stays in MenuControllerCallbacks.cpp, and the complete
menu owner itself remains non-exact. See MENU_CONTROLLER.md.

Document cleanup4549E0/243 is now **cold linked-PE exact 243/243**
with eight independently reviewed fields (one renderer DIR32,
marker REL32 and six free REL32), native8 calls/one return and
complete primary-TU PE/MAP/PDB/source receipts. Its direct
callee `MarkMenuFileAnimations`4564C0/78 is independently
**cold linked-PE exact 78/78** with no relocation fields in a real
MenuController caller context. Both benefit from an explicit
generic renderer/file caller so LTCG preserves native ABI. The
415-byte player destructor430010 remains non-exact; next reconcile
its resource cleanup with this newly exact callee. See
[ANIMATION_DOCUMENT_DESTROY](ANIMATION_DOCUMENT_DESTROY.md).


Two additional cold VC9 exact three-byte identity constructors
at4021D0 and433F00 (zero relocations) are used by the storage
constructor's two seven-element arrays. A behavioral bug in
the 1557-byte initializer was fixed: the second collision extent
now reads the SAME original 4A7AB0 table as the pattern configuration
instead of the incorrect 4A7AC0 focus table; normal COFF bytes and
complete exact status remain unchanged. See SECONDARY_PLAYER_INITIALIZATION.md.

The controller initializer42F8A0/1557 now places the extent before
the position in all four maximum-corner C++ expressions. This restores
11/12 native FADD operand sequences in the focused /GL code (versus
1/12), while preserving 1479 bytes, 13 calls and two returns.
The remaining first-component duplicate is optimizer-dependent;
the owner remains non-exact. See SECONDARY_PLAYER_INITIALIZATION.md.

Controller initialization owner42F8A0/1557 now has compiled full
source in SecondaryPlayerInitialization.cpp: pinned normal COFF1492/40
fields and narrow /GL1479/13 calls/2 returns versus native1557/13/2.
Six direct support TUs yield1520/14 calls/3 returns, still non-exact.
The child42F580/286 stays cold-linked exact. The constructor factory4301B0/152 now uses real C++ RAII,
restoring the native VC9 SEH frame: normal COFF150 (previous67), scoped
initializer-only /GL149, lifecycle+initializer /GL152/native152 with
5/5 calls and 2/2 returns. **Still non-exact**, with 121 raw bytes
different and private call ABI/stack differences. Both lifecycle
dependencies also have compiled source in
SecondaryPlayerStorageLifecycle.cpp. Constructor42F760/272 now explicitly calls the real array iterator
4021A0 twice per emitter: normal COFF now271 (before287 after
replacing two raw placement-new VM calls with inlined typed member
construction), scoped /GL278 with
**five native calls and one RET4** (previous236/three calls).
The iterator4021A0/33 has separate source; generic /GL33
with29/33 bytes equal but specialized real-caller /GL31, non-exact.
Destructor430010/415 now uses scoped C++ locks and automatic
reverse destruction of embedded VMs: normal COFF389, focused
/GL **400/native415 with 12/12 calls and 1/1 return** when
StageLifecycle.cpp supplies real VM destructor bodies (previous
357/12 calls/2 returns). Without that TU the helper tail gives
387/11/1; both remain non-exact. The two 3-byte identity
constructors remain cold exact. Continue EH/lifetime register allocation recovery without unrelated
cold replay. C++ placement-array trials for the identity constructors
regressed to /GL215/native272 after dead-code elimination and were
not adopted. See
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


Dialogue phase query42F540/27 is strict normal-COFF exact.
Controller animation pair reset42F580/286 now also has strict linked-PE
**exact 286/286** with one g_RenderBatch DIR32 field and the natural
EBX argument recovered from caller42FEC0/304. Both have C++ source.
The 304-byte controller restart caller is source-present but
**non-exact**, initially linked319/native304. See
[SECONDARY_PLAYER_ANIMATION_RESET](SECONDARY_PLAYER_ANIMATION_RESET.md).


The normal pause42C8D0 and continue42D9D0 owners now use native-style
constant-multiply signed keyboard remainder: pause 4 IMUL/0 IDIV,
continue 10 IMUL/0 IDIV. Narrow /GL probes give pause3244->3276/native3212
(length regression) and continue3032->3096/native3192; the refreshed
63-support context now yields3172/native3192 with call/return parity. Both non-exact. See PAUSE_MENU.md and
PAUSE_CONTINUE_MENU.md.

Prioritize substantial complete owners and their byte-level gaps. The pause
owner42C8D0/3212 remains3084/160 fields/3039 differences. Seven direct child-query
result objects recover frame68/cookie64; an inline wrapper reused one slot and
produced frame4C. Preserve independent input tests and metadata stage68 versus
difficulty64. The shared child finder stays76/native70/41 with an unresolved
register/stack ABI difference. See [PAUSE_MENU](PAUSE_MENU.md).

PreparePauseContinueRecords42D850/380 and InsertPauseContinueScore42BA80/365
now have VC9-compiled source in src/PauseContinueRecords.cpp and
src/PauseRecordInsertion.cpp. Full normal-COFF diagnostic sizes are387/232;
they are not exact. The insertion coefficient uses100.0, and the stage7/saved
run branch is rechecked after insertion. Writer436420/1856 and stream
allocator437300/150 have compiled source in src/ReplayWriter.cpp; normal-COFF
sizes1684/160, neither exact. A focused eight-TU /GL link gives writer
1765/native1856 and 46/46 call sites (15 sprintf, two memset), while the
allocator remains160/native150. Original private ABIs still differ.
See REPLAY_WRITING.md. Compression4423A0 now has compiled source; normal-COFF candidate738 bytes versus native795, non-exact. See
REPLAY_WRITING.md and PAUSE_CONTINUE_MENU.md. Compression tree reset
4428D0/52 has a cold-pinned exact normal-COFF unit with four replayed
relocations. Tree search442910/416 also has compiled source in
src/ReplayCompressionSearch.cpp; normal COFF416/19 fields/270 of340
comparable bytes, nonexact. Continue narrowing the native-vs-VC9 codegen on compressor main
4423A0, then test a focused linked /GL context with the non-exact
node helpers in src/ReplayCompressionTreeOps.cpp (owners79/80/134/49).
The predecessor442BE0/49 is now **cold-pinned linked exact** with three
independent DIR32 field replays; the other three tree edits remain nonexact.
A focused five-TU VC9 /GL link gives encoder760/native795,
tree matcher414/native416, replacement134/native134 (nonexact), deletion
87/native79 and reset52/native52. See REPLAY_WRITING.md for field gaps;
this is diagnostic, not exact. No unrelated cold cohort needed.
The other child-finder caller42E6B0 now has a **reviewed complete3208-byte**
owner with the seven-entry jump table and VC9-compiled source in
src/PauseGameOverMenu.cpp. Branch-local selection reset in state21 improves
normal COFF3068->3116; the signed 13-column keyboard quotient form
eliminates four synthetic IDIVs. A focused /GL link with MeshEffects.cpp
now gives **3228/native3208** bytes, 10/10 IMUL, zero IDIV,
88/88 calls,14/14 returns and the seven table cells. Still non-exact;
align factory-result lifetime and state25 record-name layout next. See
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md). Neighboring 42F340/350
14-byte selection comparisons now have natural C++ source, 12/14 bytes
each and no confirmed xrefs. Overlay queue42F400/103 now has a strict cold linked-PE **103/103
exact** unit (no relocation fields, full PE/MAP/PDB receipt). Adjacent
VM capture42F480/101 has corrected sprite height/width parameter order
but remains non-exact (linked100). An early volatile-index experiment gets
linked101 but swaps native EBP/EBX and was not retained; a float-temp
variant regressed to111. See PAUSE_OVERLAY_RECTANGLES.md. See
[PAUSE_OVERLAY_RECTANGLES](PAUSE_OVERLAY_RECTANGLES.md). Do not rebuild
unrelated gameplay or canonical units.

## Checks

Default `scripts/repo-python scripts/ci.py` checks the public checkout without
private inputs. Cold-build and fully compare only affected exact contexts.
Toolchain identity: `scripts/repo-python scripts/verify-toolchain.py --check`.
Canonical caches remain in `build/match/` and `build/match-linked/`; select a cold
rebuild before relying on a cache. See [ARTIFACTS](ARTIFACTS.md) for retention.
