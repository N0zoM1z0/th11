#include "../src/SharedFile.hpp"
#include "../src/ByteCipher.hpp"
#include "../src/ReplayPayloadChecksum.hpp"

// Non-runnable link context for the 00437A70 statistics-file owner. The
// shared helpers also have callers with live arguments in the game.
extern int __cdecl SaveSecondaryGameplayStatisticsFile();
extern "C" int __stdcall SecondaryGameplayStatisticsWriterProbe(
    unsigned char *data, int size, const char *filename)
{
    int result = SaveSecondaryGameplayStatisticsFile();
    result += reinterpret_cast<ReplayPayloadChecksumView *>(data)->SumPayloadBytes(size);
    GatherXorBlocks(data, size, data[0], data[1], size, size);
    result += OpenSharedFileForWrite(filename);
    return result;
}
