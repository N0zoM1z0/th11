#include "StageResource.hpp"
#include "ResourceFile.hpp"
#include "LogBuffer.hpp"

extern "C" int __stdcall StageResourceProbe(StageResourceView *first,
    StageResourceView *second, const char *filename, int index, RenderBatchView *renderer,
    int fromDisk, unsigned int *size, LogBufferView *log)
{
    int result = LoadStageResource(first, filename) + LoadStageResource(second, filename);
    result += BeginLoadAnimationDocument(renderer, filename, index) != 0;
    result += AcquireAnimationDocument(index, filename) != 0;
    result += LoadResourceFile(filename, size, fromDisk) != 0;
    AppendLog(log, filename, index);
    return result;
}
