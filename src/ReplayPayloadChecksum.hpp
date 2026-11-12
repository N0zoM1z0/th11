#pragma once
#include <stddef.h>

// Physical native replay checksum view: the bytes summed start 8 bytes after
// the receiver; the original owning class and translation unit are unknown.
struct ReplayPayloadChecksumView
{
    unsigned char header[8];
    unsigned char bytes[1];
    unsigned int SumPayloadBytes(int length);
};
typedef char ReplayPayloadChecksumDataOffset[
    (offsetof(ReplayPayloadChecksumView, bytes) == 8) ? 1 : -1];
