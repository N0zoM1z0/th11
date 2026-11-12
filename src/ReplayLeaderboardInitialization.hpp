#pragma once
#include <stddef.h>

// Native CR/ST record header and large on-disk table layouts. Names and
// full file format remain under investigation; the measured extents are fixed.
struct ReplayScoreArchiveView
{
    unsigned char bytes[0x68d4];
};
struct ReplaySeedArchiveView
{
    unsigned char bytes[0x448];
};
typedef char ReplayScoreArchiveSize[(sizeof(ReplayScoreArchiveView) == 0x68d4) ? 1 : -1];
typedef char ReplaySeedArchiveSize[(sizeof(ReplaySeedArchiveView) == 0x448) ? 1 : -1];
void __fastcall InitializeReplayScoreArchive(ReplayScoreArchiveView *self);
void __fastcall InitializeReplaySeedArchive(ReplaySeedArchiveView *self);
