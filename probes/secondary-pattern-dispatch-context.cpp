#include "SecondaryPattern.hpp"
#include "ResourceFile.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"

extern "C" int __stdcall SecondaryPatternDispatchProbe(SecondaryDamageReferencesView *self,
    const char *filename, unsigned int *size, int fromDisk, int frame, RenderBatchView *renderer)
{
    int result = LoadSecondaryProjectilePatterns(self, filename);
    result += EmitSecondaryPatternFrame(self, frame);
    result += UpdateSecondaryPattern(self);
    result += DrawSecondaryPlayerEmitters(self);
    result += reinterpret_cast<AnimationVmView *>(reinterpret_cast<unsigned char *>(self) + 0x14)->DrawByMode(renderer);
    return result + (LoadResourceFile(filename, size, fromDisk) != 0);
}
