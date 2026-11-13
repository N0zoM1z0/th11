// EXPERIMENTAL __fastcall alternative only: the maintained path source is
// still __stdcall and this probe is not a canonical matching producer.
// Bounded generic pathname caller; the real Pbg File/ResFile owners form the
// second part of this historical /GL producer context.
char *__fastcall DuplicatePbgResourcePath(const char *path);
extern "C" char *__stdcall PbgResourcePathProbe(const char *path)
{
    return DuplicatePbgResourcePath(path);
}
