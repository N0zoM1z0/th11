#include "SecondaryPlayerWrapItems.hpp"
#include <string.h>

// Complete native 0x40D040..0x40D058. The original pool retains its
// first 0x14 header bytes while resetting the contiguous item storage.
void __cdecl ResetSecondaryGameplayItems()
{
    memset(reinterpret_cast<unsigned char *>(g_SecondaryAttractionItems)
           + 0x14, 0, 0x265e50);
}
