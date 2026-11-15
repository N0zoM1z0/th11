# Joystick configuration page

The joystick page has three owners, all natural C++:

| Owner | Address | Bytes | State |
| --- | --- | ---: | --- |
| `UpdateMenuPage04` | 43B9D0 | 908 (885 code, 3 alignment, 20-byte table) | Not exact |
| `ApplyMenuJoystickBindings` | 43BD60 | 2439 | Exact (`apply-menu-joystick-bindings`) |
| `AssignMenuJoystickButton` | 43C6F0 | 200 | Not exact |

## Boundaries

The display owner runs to 43C6E6 and includes the twenty late child-success
blocks at 43C62F..43C6E2 and the final missing-VM return. The jump at 43C62A
is a tail call to `BindAnimationSprite` 44AB40, not the end of the function, so
do not crop the late blocks. It makes 40 lookups and 19 ordinary binds plus
the tail bind (61 address fields). The page's five-entry table at
43BD48..43BD5B explains its apparent interior data references.

LTCG inputs: the display takes the menu in EDI, the page takes it in EBX, and
the assignment helper takes selection in EAX, button in ECX and menu in EDX.
The source uses ordinary `__stdcall` signatures and leaves this to `/GL`.

## Behaviour

The menu view types the value ID at +2CC and five staged signed shorts at
+59C4..+59CC. `JoystickBindingsView` holds fire/bomb/slow/pause and skip at
+10; saving copies the whole 18-byte record to MainRuntime+1CC, keeping the
four middle words unchanged.

Each staged binding has two digits drawn by two script groups, all from sprite
base 2A:

| Binding | First tens/ones | Second tens/ones |
| --- | --- | --- |
| Fire | 3B/3C | 45/46 |
| Bomb | 3D/3E | 47/48 |
| Slow | 3F/40 | 49/4A |
| Pause | 41/42 | 4B/4C |
| Skip | 43/44 | 4D/4E |

Every digit looks up the root, clears a missing root ID, walks the draw nodes
by 16-bit script and does the second lookup even for ID 0. The root traversal
is unchecked; only the final sprite write is guarded. The binding is read
after the lookup; `/10` and `%10` on the signed short produce the native
multiply-and-correct and `IDIV` sequences.

The seven-selection page loads the staged bindings on opening, starts script 2
on layer 16 and activates after frame 6. It polls with `PollJoystickButtons`,
scans buttons 0..30, stops at the first pressed one and assigns it only when
the selection is at most 4. Selection 5 reloads the current bindings. Cancel on
selection 6 discards and closes; confirm commits first. After ten closing
frames it returns to page 3 and pops the selection history.

Assignment returns early when the old binding equals the button. Five ordered
conflict tests copy the old value into any slot that already holds the button
(re-reading the selected slot each time), then it stores the new value,
refreshes all twenty digits and queues sound 10.

## Exact display context

The display is exact only in the wide link (`probes/wide-context.cpp`, 214
supports) that combines the secondary HUD owners with the full menu context.
In the earlier joystick-only context (since removed) VC9
pushed the file argument at every bind and dropped the final tail jump. With
the real menu and animation callers present, LTCG gives `BindAnimationSprite`
its native register convention and the tail call reappears.

`BindAnimationSprite` 44AB40 multiplies width and height by the double at
497F50, which is 1/256 (not 0.5). It is not exact on its own: 388/389 bytes in
the canonical audio graph.

## Open work

- In the wide context the page is 880/908 bytes (98/225 instructions aligned)
  and the assignment helper 195/200 bytes (30/48).
