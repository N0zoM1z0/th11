// Native 0x441C80..0x441C80 is a bare RET with no side effects. Maintain
// the typed logging call boundary without inventing writes or format output.
void __cdecl TracePbgResourceEvent(const char *format, ...)
{
    (void)format;
}
