// Non-runnable independent filenames retain both open modes under VC9 /GL.
#include "../src/SharedFile.hpp"

extern "C" int __stdcall SharedFileProbe(const char *a, const char *b, int read)
{
    if (read)
    {
        int first = OpenSharedFileForRead(a);
        CloseSharedFile();
        return first + OpenSharedFileForRead(b);
    }
    int first = OpenSharedFileForWrite(a);
    CloseSharedFile();
    return first + OpenSharedFileForWrite(b);
}
