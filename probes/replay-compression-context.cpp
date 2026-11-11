// Non-runnable, isolated VC9 /GL link context for the replay encoder and
// actual native source-call graph. No byte shims or target data are embedded.
extern unsigned char *__stdcall CompressReplayData(const void *, int, int *);
extern "C" unsigned char *__stdcall ReplayCompressionProbe(
    const void *input, int size, int *packedSize)
{
    return CompressReplayData(input, size, packedSize);
}
