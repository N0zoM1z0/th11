// Native 0x0044AB00 is a single RET. Callers pass ordinary printf-style
// diagnostics and clean their own stack; this release build discards them.
// The source keeps the real variadic interface, without assembly or padding.
void __cdecl TraceSoundQueue(const char *format, ...)
{
}
