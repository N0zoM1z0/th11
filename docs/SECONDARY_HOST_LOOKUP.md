# Secondary host dialogue and clearing-object lookups (2026-11-12)

Two previously unsourced owner functions were reviewed against the full mapped
original Japanese TH11 v1.00a executable. Both have contiguous function bodies,
no references into their interiors, ordinary `RET` endings, and following CC
padding. The implementation uses existing gameplay views rather than an
invented register ABI or manually encoded instructions.

- `0x410720..0x410731`, `ReadSecondaryHostDialogueDelay`: the existing HUD
  pointer at `0x4A8D84` supplies its dialogue pointer at `+0x4438`; the helper
  returns the `+0x8C` dialogue delay. It deliberately performs no null guard,
  matching the target. Native caller occurs at `0x415B09`.
- `0x4108D0..0x4108EE`, `FindSecondaryClearObjectByIdentity`: loads the clear
  manager pointer at `0x4A8E94`, follows the first object at `+0x18` and each
  object's next at `+8`, and tests the object identity at `+0x6C`. Identity
  zero returns null even if a list exists. The control logic is shared by
  eight calls in the secondary host interpreter around `0x41644A..0x41660A`.

Each complete function is independently reproduced under the pinned VC9 RTM
normal-COFF `/TP /O2 /Ob1 /Gy /MT /Isrc` compiler context. The first has
18/18 exact bytes, the second 31/31, and each has one reviewed DIR32 relocation
at offset +1. Canonical units are `read-secondary-host-dialogue-delay` and
`find-secondary-clear-object-by-identity`; no relocation masking is credited.
Original owner TU/class names and original `/GL` partition remain unresolved.
