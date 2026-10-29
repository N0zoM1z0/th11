// Independent object and payload. The diagnostic image is never executed.
#include "SecondaryProgramExtension.hpp"
#include "ResourceFile.hpp"
#include "AnimationDocument.hpp"
extern "C" int __stdcall SecondaryProgramExtensionProbe(int kind,
    SecondaryProgramWithExtensions *program, void *data, const char *filename,
    unsigned int *size, int fromDisk, RenderBatchView *renderer)
{
    if (kind == 0) return program->SecondaryProgramWithExtensions::ProcessExtension(data);
    if (kind == 1) return program->SecondaryProgramWithExtensions::LoadFile(static_cast<const char *>(data));
    if (kind == 2) return reinterpret_cast<int>(LoadResourceFile(filename, size, fromDisk));
    return reinterpret_cast<int>(BeginLoadAnimationDocument(renderer, filename, fromDisk));
}
