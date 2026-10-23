# VC9 linked-function Oracle

The first accepted linked unit is `poll-joystick-buttons`, a complete267-byte
TH11 function. Its pinned VC9 `/GL` cold build produced matching PE, linker
MAP and PDB evidence; all thirteen independently reviewed fields replayed
with zero byte differences. No earlier exact unit was rebuilt to validate it.

This lane compares complete public code contributions from the primary
maintained translation unit. It does not establish the original game's TU
partition, original normal-COFF/LTCG ownership, complete data ownership or a
native game build. Specialized static-owner, EH and split-owner acceptance
still needs further implementation and evidence.

## Cold construction and provenance

`build-match-unit.py` dispatches by explicit artifact kind. A `linked-pe`
context requires exactly one `/GL` switch, one entry, a bounded build output
directory, and declared primary/support source profiles. Quoted local includes
are recursively hashed. Selected normal support sources remain normal COFF;
profile overrides cannot silently remove `/GL` from an LTCG source.

`ltcg_link.py` uses the pinned VC9 compiler/linker, existing Wine/Xvfb wrapper,
single CPU affinity, nice10 and shared serialization lock. It cold-removes
prior outputs and links with fixed nonincremental `/LTCG` options, no default
libraries, no identical-code folding, and explicit PE/MAP/PDB outputs. The
requested entry must resolve to real maintained executable code with matching
MAP/PDB ownership. An unresolved entry cannot acquire a data anchor.

Unresolved dependencies receive distinct0x1000-byte zero data anchors and an
explicit alias response file. These preserve symbolic address bindings in a
non-runnable diagnostic image; they do not implement the game dependencies.
The retained source declarations supply their calling conventions. No target
instructions are embedded or patched into generated code.

The receipt binds the target identity, compiler/linker component hashes, tool
lock, producer/wrapper scripts, complete source/header/profile context, every
compiled object, PE, MAP, PDB, and optional anchor artifact family. Inputs are
checked before and after compilation. A failed build leaves no current
receipt; changed sources, headers, profiles, tools, producers or output bytes
invalidate the receipt.

## Complete ownership and field replay

`compare-linked-function.py` requires the current receipt and pinned Capstone
decoder. It verifies the original executable's full SHA-256/size and the
reviewed target span/source row. Candidate PE, MAP and PDB timestamps, image
base, CodeView GUID and ages must agree.

The selected symbol must have one complete PDB contribution whose full size
equals the target owner. MAP and PDB must identify the primary source object
in its producer directory. Duplicate modules, overlapping contributions,
multiple same-start symbols, interior public entries and nonexecutable owners
are rejected. A shorter `compare_size` is not supported.

The decoder enumerates every PE HIGHLOW field and every decoded relative
control transfer leaving the contribution. All bytes must be decoded or
covered by authoritative PE fields. The manifest must declare exactly that
field set, with no overlap or unsupported selectors. Ordinary external
bindings must resolve uniquely through the real MAP symbol or its declared
anchor alias. Local bindings must retain the same destination offset inside
both complete owners. Every declared field is replayed at its reviewed target
address; every remaining byte is compared directly. These are address
relocations, never a masked exactness score.

## Verification and use

Twenty-five public synthetic tests use generated PE32/MSF7/PDB fixtures,
without game bytes or the historical compiler. They cover complete positive
replay, final-byte corruption, cropped/overlapping contributions, wrong
owners, interior entries, MAP/PDB identity conflicts, undeclared fields, wrong
symbols and local offsets, malformed selectors, incomplete decoding,
incorrect aliases, stale source/header/producer/artifact receipts, tool
changes, and top-level target/source/boundary gates. Existing normal-COFF
negative controls remain separate and unchanged.

```bash
scripts/repo-python scripts/test-linked-oracle.py
scripts/repo-python scripts/replay-exact-units.py --unit poll-joystick-buttons
```

Public CI installs pinned Capstone5.0.6 and runs both synthetic suites. Only
private target-bound cold builds provide actual game exactness evidence.

The registration pair now also validates nonzero member addends: fields name
one runtime object base plus reviewed offsets. The comparator first binds the
candidate's real base symbol before replaying the target base/addend. A
synthetic control rejects a wrong addend despite a compensating target-base
change that would otherwise fit the final bytes. Both registrations share one
cold source/caller context and retain their complete147-byte PDB extents.
