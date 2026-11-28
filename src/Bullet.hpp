#pragma once
#include "StageResource.hpp"
#include <stddef.h>

// bullet.cpp ("bullet.cpp:453 BulletInf"). The manager at 0x4A8D68 (also
// g_SceneClearPool / g_SecondaryActorFiles) holds 0x7D1 bullets of 0x910 bytes
// from +0x64 and the bullet.anm document at +0x46D674. SceneClearProjectileView
// is an older partial view of one bullet. The implicit bullet constructor and
// destructor (0x4083D0, 0x408410) are what the vector iterators call.
struct BulletPatternRecord
{
    StageTimerView timer;
    unsigned int words[8];
};

struct BulletData
{
    unsigned int flags, kind;
    StageEmbeddedVm vm;
    unsigned char unknown43c[0x464 - 0x43c];
    StageTimerView clearTime;
    StageTimerView timer478;
    unsigned char unknown48c[0x4b2 - 0x48c];
    unsigned short state;
    unsigned char unknown4b4[0x680 - 0x4b4];
    BulletPatternRecord patterns[11];
    unsigned char unknown8bc[0x8f0 - 0x8bc];
    StageTimerView timer8f0;
    unsigned char unknown904[0x910 - 0x904];
};

struct BulletInf
{
    BulletInf();
    ~BulletInf();

    unsigned int unknown000[2];
    ChainElement *updateElement;
    ChainElement *drawElement;
    BulletData *bulletCursor;
    unsigned char unknown014[0x64 - 0x14];
    // The last entry is a sentinel whose state stays 5.
    BulletData bullets[0x7d1];
    AnimationDocumentView *document;
};

typedef char BulletDataSize[(sizeof(BulletData) == 0x910) ? 1 : -1];
typedef char BulletDataState[(offsetof(BulletData, state) == 0x4b2) ? 1 : -1];
typedef char BulletDataPatterns[(offsetof(BulletData, patterns) == 0x680) ? 1 : -1];
typedef char BulletInfSize[(sizeof(BulletInf) == 0x46d678) ? 1 : -1];
typedef char BulletInfDocument[(offsetof(BulletInf, document) == 0x46d674) ? 1 : -1];

extern BulletInf *g_BulletManager;

int __stdcall InitializeBulletManager(BulletInf *self);
void __cdecl ResetBulletManager();
BulletInf *__cdecl CreateBulletManager();
void __cdecl DeleteBulletManager();
int __fastcall BulletManagerUpdateCallback(BulletInf *self);
int __fastcall BulletManagerDrawCallback(BulletInf *self);
