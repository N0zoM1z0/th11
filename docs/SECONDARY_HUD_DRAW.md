# Complete secondary HUD drawing

DrawSecondaryHud at41C660 has complete natural C++ source for its2,378-byte
owner. The actual499-byte rectangle dependency is canonical linked exact;
the HUD owner, its time decoder and drawing callback remain non-exact. Original
class identities, TU partition and native product/runtime closure remain open.

## Target and entry

Full direct Ghidra review and independent PE control-flow decoding establish
41C660..41CFA9:564 reachable instructions/2,372 code bytes plus a six-byte no-op
LEA at41CEBA. There is no table or foreign interior entry. Six following CC
bytes precede the separate update callback41CFB0. All27 calls to eight targets
are reviewed. Native entry uses ESI for the HUD, GS protection,64-byte stack
alignment, frame78 and one plain RET returning1.

Its sole caller is the separately registered41CFD0..41CFD9/10-byte drawing
callback: save ESI, move ECX to ESI, call the full owner, restore ESI and return.
Registration is at41A4EC. The decompiler's ECX argument and void-return guesses
are not treated as the owner's actual interface. Maintained conventional C++
uses an explicit HUD pointer and an ordinary fastcall callback; private register
allocation is left to the compiler.

## Full source operations

The physical fields are views of the existing HUD, settings, spell, renderer
and ASCII objects. The source covers every target branch:

- Bonus VM43B4 lookup, signed4450 formatting into16 bytes and centered numeric
  text at Y200. The width calculation converts unsigned string length to float
  before the double0.5/12/224 expression. The second lookup clears a missing ID
  and retains the target's subsequent unchecked alpha read. ASCII state resets
  after printing.
- Tracked43BC/active43C0 handling. A missing spell clears active and releases the
  ID; a missing VM only clears active. Live spell+8F8 frames produce capped999
  seconds and truncated hundredths. A separate decoder supplies the gray best
  time. All four prints retain their positions,0.6/1 scale changes and alpha.
- Positive boss43F0 draws black/white rectangles and four conditional colored
  markers from4400, followed by all four gauge VMs. Unordered marker fractions
  enter the nonzero branch; the minimum selection retains boss value on an
  unordered comparison. Rectangle arithmetic uses double330/41/40 then float
  stores, with separately rounded one-pixel adjustments.
- High/current score formats, leading-space selection below100,000,000, settings
  trailing digits and replay/statistics selection. The record address uses the
  observed68D4 stride,5A4 offset and eight-byte difficulty/variant rows; the
  maximum includes HUD43E4. No original statistics capacity is inferred.
- Power quotient/fraction/maximum, integer value and combined points/multiplier
  display. Wrapped integer multiplication is retained where the target uses
  IMUL. The combined `%.6d*%1.2f` passes an integer followed by an actual double:
  FILD signed32, divide by double100 and store eight outgoing bytes without an
  intermediate float round. Fifteen physical ASCII print sites are retained.
- Conditional two-digit VM clock: master present, seconds nonnegative,
  master1C nonzero, master3C bit0 clear and no dialogue. It draws both digit VMs,
  uses their color, prints the dot and HUD4444 fraction, then restores ASCII
  scale/color/style. Every full-owner path returns1.

DecodeSecondaryHudBestTime41A250 owns151 bytes/61 instructions with two RET4
paths. It reads spell+90C and validates `encoded/100000-22` against the sum of
`(encoded/100%100+34)%100` and `(encoded%100+67)%100`. Failure writes999/99;
success reloads the word and writes both components. Signed division/remainder
and actual two-output storage are retained; no unsigned reinterpretation or
new guard is introduced. Its current167-byte candidate has160 complete byte
differences after its one field, without exact credit.

## Rectangle dependency

DrawHudSolidRectangle447F70..448162 is complete499 bytes/157 instructions,
plain RET, no table/padding/interior entry, followed by thirteen CC bytes. Five
reviewed callers include three sites in this HUD owner. EDI supplies the four
left/top/right/bottom floats and EBX the packed color; those registers are not
encoded as a false source ABI.

The function flushes the real batch, builds four20-byte XYZ/RHW/ARGB vertices,
selects diffuse color/alpha stages and inverse-source-alpha destination blend,
sets FVF44 and draws two triangle-strip primitives. It then invalidates the six
observed renderer cache fields and restores four texture-stage settings. All
12 calls, eleven device loads at runtime+8 and both renderer globals are retained.
The four real point temporaries reproduce the native float/DWORD copy sequence.
Initializing RHW/color in reverse3..0 order closes the remaining eight byte
differences; this changes actual vertex stores without padding or byte patches.

Canonical `draw-hud-solid-rectangle` cold-builds SecondaryHudDrawSupport.cpp as
the public primary /GL TU, with the real whole HUD/ASCII/batch/draw support and
an ordinary function-pointer entry. All499 bytes and14 independently bound
fields replay with zero differences through compare-linked-function.py. PE/MAP/
PDB identity, primary owner and current source/header/tool/producer receipts
are checked. Runtime-device bindings use base4C3280 plus addend8. No original
TU or complete-game link claim follows from this diagnostic family.

## Whole-owner code generation

The maintained public primary HUD family contains eight public support files;
only the function-pointer entry is ordinary COFF. The full candidate is2,343
bytes/559 instructions, GS/alignment64/frame74, all27 calls in native order and
135 exhaustive call/global/literal/string fields. Full replay leaves2,162 byte
differences. The native frame78 plus two saved registers and source74 plus three
both reserve0x80 bytes after saves; source still spills the incoming HUD pointer.
The native callback preserves ESI outside the owner; the candidate moves that
save/copy inside and generates a five-byte tail callback. Temporary homes,
conditional-format scheduling and decoder output registers remain open.

A twelve-byte local position value with a constructor assigning its actual
three floats and ordinary temporary assignment restores observed three-word
copies and runtime Y adjustments. The initial array spelling gave2,250 bytes
and folded the Y+7 computations to135.0f, a literal absent from the target. The
value spelling gives2,341 bytes with all135 fields bound; adding the real
rectangle body gives the maintained2,343/2,162 result. All variants already had
64-byte alignment: it is not evidence uniquely identifying this constructor.
Nominal vector/constructor API and original header/TU remain hypotheses.

Earlier full-animation probes in this checkpoint also rule out two stack-frame
spellings: explicit component accumulation expands12,296 to12,404 while keeping
frameE0; narrower strip-value scopes are fully byte-identical after407 fields.
Those changes are not retained. The prior183 frozen source inputs remain
unchanged; only the new rectangle unit needs a canonical cold replay. No broad
behavior, null/failure, caller or multiframe suite is used.

Private current evidence: secondary-hud-draw-review/full/dependencies.*, complete
function/support/vararg proofs, secondary-hud-rectangle-canonical-comparison.json
and verify-secondary-hud-draw-checkpoint.py. Counts become457 mappings,574
reviewed boundaries and235 exact functions/28,045 bytes. Four source mappings
cover3,038 native bytes;499 of those bytes receive new exact credit.
