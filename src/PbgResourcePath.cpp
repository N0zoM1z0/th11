#include <stdlib.h>

// Native 0x441BD0 directory/path duplication helper. The original forwards
// its pathname in a private EDI register; ordinary source ABI remains open.
char *__stdcall DuplicatePbgResourcePath(const char *source)
{
    const char *end = source;
    do ++end; while (end[-1]);
    char *copy = static_cast<char *>(malloc(end - source));
    if (copy)
    {
        char *dest = copy;
        while ((*dest++ = *source++) != 0) {}
    }
    return copy;
}
