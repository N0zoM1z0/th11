#include "../src/MusicWaveRequest.hpp"

extern "C" int __stdcall MusicWaveRequestProbe(int selection, const char *filename)
{
    return QueueMusicWaveRequest(selection, filename);
}
