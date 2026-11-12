extern unsigned char *__stdcall ReadSharedReplayBytes(unsigned int count);
extern int __cdecl CloseSharedReplayReader();
extern "C" unsigned char *__stdcall SharedReplayHelpersProbe(unsigned int count)
{
    unsigned char *data = ReadSharedReplayBytes(count);
    CloseSharedReplayReader();
    return data;
}
