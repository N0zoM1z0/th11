#include "SoundQueue.hpp"
#include "MusicStream.hpp"

namespace
{
__forceinline MusicControllerView *&PreparedController(SoundManagerQueueView *self)
{
    return *reinterpret_cast<MusicControllerView **>(
        reinterpret_cast<unsigned char *>(self) + 0x5208);
}
__forceinline HANDLE &PreparedEvent(SoundManagerQueueView *self)
{
    return *reinterpret_cast<HANDLE *>(
        reinterpret_cast<unsigned char *>(self) + 0x520c);
}
}

// 00449FE0..0044A091: stop prepared music, join the streaming worker and
// release the actual polymorphic controller. Thread stop retries until
// WAIT_OBJECT_0, even after a failed/abandoned wait, as in the native path.
void __stdcall ClosePreparedMusic(SoundManagerQueueView *self)
{
    if (!PreparedController(self))
        return;
    StopMusicStream(PreparedController(self), 1);
    if (self->thread)
    {
        PostThreadMessageA(self->threadId, WM_QUIT, 0, 0);
        while (WaitForSingleObject(self->thread, 0x100) != WAIT_OBJECT_0)
            PostThreadMessageA(self->threadId, WM_QUIT, 0, 0);
        CloseHandle(self->thread);
        CloseHandle(PreparedEvent(self));
        self->thread = 0;
    }
    if (PreparedController(self))
    {
        delete PreparedController(self);
        PreparedController(self) = 0;
    }
}
