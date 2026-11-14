// A non-runnable linked VC9 context for the real replay-file writer.
extern int __fastcall WritePauseMenuReplay(const char *filename,
                                             const char *name, int mode);
extern "C" void __stdcall ReplayWriterProbe(const char *filename,
                                            const char *name, int mode)
{
    WritePauseMenuReplay(filename, name, mode);
    return;
}
