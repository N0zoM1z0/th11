#pragma once

// Native 0x0042A0D0: copy a music path, select its .wav variant, enqueue opcode 1.
int __stdcall QueueMusicWaveRequest(int selection, const char *filename);
