#pragma once

// VC9 RTM libcmt.lib _frnd_.obj, matching TH11 0x489CED in all20 bytes.
// This is the real cdecl CRT boundary: current-x87-mode integral rounding,
// with a double argument and double-rounded result returned in ST0.
extern "C" double __cdecl _frnd(double value);
