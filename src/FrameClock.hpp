#pragma once

// Observed storage cells; complete timing-state ownership is still open.
extern __int64 g_FrameClockFrequency;
extern __int64 g_FrameClockCounterBase;
extern double g_FrameClockOrigin;

double __cdecl GetFrameClockSeconds();
