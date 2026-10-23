#pragma once

// Conventional source views of the paired TH11 byte transforms. The target
// passes key in AL and the other five arguments on the stack; recovering that
// optimizer context is still open. These declarations do not claim exact ABI.
unsigned char * __stdcall ScatterXorBlocks(unsigned char *data, int size,
    unsigned char key, unsigned char step, int blockSize, int limit);
unsigned char * __stdcall GatherXorBlocks(unsigned char *data, int size,
    unsigned char key, unsigned char step, int blockSize, int limit);
