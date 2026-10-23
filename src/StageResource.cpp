#include "StageResource.hpp"
#include "ResourceFile.hpp"
#include "ErrorLog.hpp"
#include <stdlib.h>
#include <string.h>

int __stdcall LoadStageResource(StageResourceView *self, const char *filename)
{
    if (!self->source)
    {
        g_StageResourceName[0] = '\0';
        strcat(g_StageResourceName, filename);
        self->source = LoadResourceFile(g_StageResourceName, &self->sourceSize, 0);
        if (!self->source)
            return -1;
    }
    self->data = static_cast<StageBlobHeaderView *>(malloc(self->sourceSize));
    memcpy(self->data, self->source, self->sourceSize);
    self->animation = AcquireAnimationDocument((self->variant & 1) + 3,
        self->data->animationFilename);
    if (!self->animation)
    {
        AppendLog(&g_RuntimeLog.buffer, g_StageAnimationError);
        return -1;
    }
    unsigned char *base = reinterpret_cast<unsigned char *>(self->data);
    self->objects = reinterpret_cast<unsigned char **>(base + 0x90);
    self->section004 = base + self->data->section004Offset;
    self->section008 = base + self->data->section008Offset;
    for (int i = 0; i < self->data->objectCount; ++i)
        self->objects[i] += reinterpret_cast<unsigned int>(self->data);
    self->vms = malloc(self->data->vmCount * 0x434);
    return 0;
}
