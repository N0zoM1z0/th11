#pragma once

#include "AnimationDocument.hpp"

struct StageBlobHeaderView
{
    short objectCount, vmCount;
    unsigned int section004Offset, section008Offset;
    unsigned int unknown00c;
    char animationFilename[128];
};

struct StageResourceView
{
    unsigned char unknown000[0x10];
    StageBlobHeaderView *data;
    unsigned char **objects;
    unsigned char *section004, *section008;
    unsigned char unknown020[0x178 - 0x20];
    AnimationDocumentView *animation;
    void *vms;
    unsigned char unknown180[0x3008 - 0x180];
    unsigned int variant;
    unsigned char unknown300c[0x3038 - 0x300c];
    unsigned char *source;
    unsigned int sourceSize;
};

typedef char StageBlobHeaderSize[(sizeof(StageBlobHeaderView) == 0x90) ? 1 : -1];
typedef char StageResourceAnimationOffset[(offsetof(StageResourceView, animation) == 0x178) ? 1 : -1];
typedef char StageResourceVariantOffset[(offsetof(StageResourceView, variant) == 0x3008) ? 1 : -1];
typedef char StageResourceSourceOffset[(offsetof(StageResourceView, source) == 0x3038) ? 1 : -1];

extern char g_StageResourceName[];
extern const char g_StageAnimationError[];
int __stdcall LoadStageResource(StageResourceView *self, const char *filename);
