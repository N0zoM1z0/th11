#pragma once
#include "SecondaryGameplay.hpp"

// The original 0x40DAC0 owner consumes a 0x710-byte debug session in ESI,
// through an ESI-preserving callback at 0x40DDE0. The historical class and
// translation unit are not established.
int __fastcall RenderSecondaryDebugOverlay(SecondaryEclDebugView *session);
int __fastcall RenderSecondaryDebugOverlayCallback(SecondaryEclDebugView *session);
