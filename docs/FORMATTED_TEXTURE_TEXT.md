# Formatted texture text

Three real callers of the placement dispatcher now have maintained source.
Two pass the strict cold linked Oracle; the centered path remains non-exact.

| Function | Native span | Bytes | Result |
| --- | --- | ---: | --- |
| `FormatTextureText` | `0x00454C60..0x00454CFB` | 156 | exact, four bindings |
| `FormatRightAlignedTextureText` | `0x00454D00..0x00454DDF` | 224 | exact, six bindings |
| `FormatCenteredTextureText` | `0x00454DE0..0x00454EB3` | 212 | source212, thirteen stack-offset differences |

Direct, read-only Ghidra explicit-span review covers all branches, returns,
incoming references and neighboring bytes. There are no external interior
references or embedded tables. The original centered inventory entry stopped
at176 bytes, immediately before its dispatcher call. Its36-byte tail through
dirty publication, GS check and RET belongs to the complete212-byte function.
Both ledgers now use that full extent; no Oracle scope was shortened.

## Physical views and behavior

The partial VM-like view has an attachment pointer at +3AC, changed flags at
+404, text flags at +408 and an unsigned font-height byte at +41C. The attachment
has a binding pointer at +8, placement floats at +0C/+10/+14/+18 and a float
width at +38. The binding's first cell holds the texture pointer. These are
observed physical views, not recovered complete original class definitions.

All three routines format through vsprintf into the observed132-byte character
region. This is not a bounded-formatting change. The initial128-byte hypothesis
produced the same instruction lengths with incorrect stack offsets. Native
buffer/cookie addresses and the132-byte source close the direct/right bodies.
Both aligned routines establish default height17 before replacing it with a
nonzero height byte. The direct routine forwards the raw byte, leaving its zero
fallback to the downstream dispatcher. A conditional-expression hypothesis
generated a different branch direction; the default-then-conditional assignment
reproduces the native branch.

Every call selects plain mode from `(text_flags >> 1) & 1`, forwards the font
choice, spacing, foreground and outline, then ORs changed flags with1 after
the dispatcher returns. Dirty publication also happens when a positive height
between1 and8 makes the dispatcher skip rendering.

Let h be the aligned routine's selected height and n the formatted byte length.
Multiplication and shifts use native unsigned32-bit arithmetic.

- Direct x is supplied by the caller.
- Right-aligned x is `int(width - (((h-1)*n) >> 1))`, with floating subtraction
  before the conversion toward zero.
- Centered x is `int(width)/2 - (((h-1)*n) >> 2)`. Width conversion occurs first;
  its signed division by2 truncates toward zero, before the unsigned subtraction
  is passed onward as the physical32-bit x value.

These expressions differ for fractional and negative widths. They are not
replaced by a shared approximate alignment formula. Native inputs are ESI for
the view; the direct routine additionally retains font choice in EDI. The
remaining fixed parameters and varargs use cdecl stack slots. va_start points
after the real format slot, and all caller-owned arguments remain on return.

## Exact proof and independent bindings

Canonical units `format-texture-text` and `format-right-aligned-texture-text`
share one cold family under pinned VC9 RTM `/O2 /Ob1 /Gy /MT /GL`. Both are
complete public primary-source PDB contributions,156/224 bytes, with matching
PE/MAP/PDB identity, current source/header/tool/producer/artifact receipts,
exhaustive fields and zero final byte differences. They add380 exact bytes,
taking totals to40 functions / 8,894 bytes. Only these new units were built
and compared; none of the previous38 canonical proofs was replayed.

| Field | Direct offset | Right offset | Independently reviewed native destination |
| --- | ---: | ---: | --- |
| security cookie, DIR32 | 7 | 7 | `0x004A2088` |
| vsprintf, REL32 | 51 | 63 | `0x0045FDD2` |
| unsigned float correction, DIR32 | — | 156 | `0x00497F98`, bits `4F800000`, value 2^32 |
| FTOL entry, REL32 | — | 163 | `0x004864E0` |
| real placement dispatcher, REL32 | 122 | 189 | `0x00454BB0` |
| GS check, REL32 | 145 | 213 | `0x0045FC2C` |

The FTOL entry consumes x87 ST0. Native review identifies its feature gate at
`0x004C9CCC`, double spill/CVTTSD2SI path and x87 fallback. The Ghidra inferred
fastcall stack arguments are not its real input contract. No helper body is
newly mapped or credited. The correction literal was checked directly as
little-endian bytes `00 00 80 4F`, independently of the linked candidate.

The centered routine has the correct212-byte full contribution but its frame
is0x88 rather than native0x8C. Its13 remaining differences are stack-relative
offsets, including the native extra slot before the character region. Original
frame/type context remains open. No dummy local or padding is inserted to
erase this difference. A future edit to this shared source must revalidate
the two affected exact units, without replaying unrelated units.

## Bounded diagnostic and phase limits

Private `.analysis/formatted-text-differential.json` records123 complete-body
machine-code cases. vsprintf, finite FTOL, placement dispatch and cookie checks
are modeled. Observations include actual va_list values, formatted bytes,
all physical dispatcher parameters, x87 inputs, unchanged VM bytes, changed
flag publication, stack balance and preserved registers. An independent integer/
floating reference checks each alignment expression. Cases include byte-height
0/1/8/9/17/255, empty/odd/CP932 strings,131 output bytes, fractional positive/
negative width, rendering flags, colors and a changed flag modified during the
modeled dispatcher. The GS cookie is verified after every case.

This diagnostic does not validate the complete printf/FTOL implementations or
exceptional values, and grants no full native/runtime authority. It runs on
one CPU/nice10 with maximum RSS106,712 KiB. Ghidra/compiler serialization and
resource limits remain unchanged. Existing outlined/plain text and filter
sources are compiled only as direct support, with no changes to old canonical
inputs. All three source origins remain independently indeterminate despite
their established renderer/VM-like use. Native game build/runtime closure
remains open.
