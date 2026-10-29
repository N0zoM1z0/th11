#include "SecondaryProgramExtension.hpp"
#include "AnimationDocument.hpp"
#include "ResourceFile.hpp"
#include "ErrorLog.hpp"
#include <string.h>

// Separate physical view of the singleton also used by mesh-effect event counting.
extern unsigned char *g_SecondaryProgramHost;
extern char g_SecondaryProgramFilename[];
extern const char g_SecondaryAnimationLoadError[];

namespace
{
struct SecondaryExtensionHeader
{
    DWORD magic, count;
};
}

int SecondaryProgramWithExtensions::ProcessExtension(void *data)
{
    SecondaryExtensionHeader *header = static_cast<SecondaryExtensionHeader *>(data);
    if (header->magic != 0x4d494e41) return 0;
    char *name = reinterpret_cast<char *>(header + 1);
    for (unsigned int index = 0; index < header->count; ++index)
    {
        AnimationDocumentView *document = AcquireAnimationDocument(index + 8, name);
        reinterpret_cast<AnimationDocumentView **>(g_SecondaryProgramHost + 0x44)[index] = document;
        if (!document)
        {
            AppendLog(&g_RuntimeLog.buffer, g_SecondaryAnimationLoadError);
            return -1;
        }
        name += strlen(name) + 1;
    }
    if (static_cast<int>(name - static_cast<char *>(data)) % 4)
        name += 4 - static_cast<int>(name - static_cast<char *>(data)) % 4;
    header = reinterpret_cast<SecondaryExtensionHeader *>(name);
    if (header->magic == 0x494c4345)
    {
        name += sizeof(SecondaryExtensionHeader);
        for (unsigned int index = 0; index < header->count; ++index)
        {
            LoadFile(name);
            name += strlen(name) + 1;
        }
    }
    return 0;
}

int SecondaryProgramWithExtensions::LoadFile(const char *filename)
{
    g_SecondaryProgramFilename[0] = 0;
    strcat(g_SecondaryProgramFilename, filename);
    void *data = LoadResourceFile(g_SecondaryProgramFilename, 0, 0);
    int result = SecondaryProgramView::Install(data);
    return result < 0 ? -1 : 0;
}
