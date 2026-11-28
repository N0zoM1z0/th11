#include "AnimationDocument.hpp"
#include "ErrorLog.hpp"
#include "RenderBatch.hpp"
#include "StageResource.hpp"
#include <new>

// front.cpp ("front.cpp:299 FrontInf"): the in-game HUD, published at
// 0x4A8E98. It owns animation slots 5 (front.anm) and 6 (bullet.anm).
extern const char g_TitleMenuAnimationError[];
extern const char g_BulletAnimationName[];
extern const char g_BulletAnimationError[];

int __cdecl LoadFrontDocuments();
int __cdecl ReleaseFrontDocuments();

namespace
{
__forceinline void ReleaseFrontDocument(int index)
{
    AnimationDocumentView **slot =
        &reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch)->files[index];
    if (*slot)
    {
        DestroyAnimationDocument(*slot);
        operator delete(*slot);
        *slot = 0;
    }
}
}

// 0x428920
int __cdecl LoadFrontDocuments()
{
    if (!AcquireAnimationDocument(5, "front.anm"))
    {
        AppendLog(&g_RuntimeLog.buffer, g_TitleMenuAnimationError);
        return -1;
    }
    if (!AcquireAnimationDocument(6, g_BulletAnimationName))
    {
        AppendLog(&g_RuntimeLog.buffer, g_BulletAnimationError);
        return -1;
    }
    return 0;
}

// 0x428980
int __cdecl ReleaseFrontDocuments()
{
    ReleaseFrontDocument(5);
    ReleaseFrontDocument(6);
    return 0;
}
