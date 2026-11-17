#include "SecondaryClearing.hpp"
#include <stddef.h>

// The existing secondary clear-object list also supports a distinct virtual
// rectangle query at slot +0x18. Other virtual entries are not called here;
// their signatures remain undetermined by this owner.
struct StageAuxiliaryAreaSubscriber
{
    virtual void Unknown00() = 0;
    virtual void Unknown04() = 0;
    virtual void Unknown08() = 0;
    virtual void Unknown0c() = 0;
    virtual void Unknown10() = 0;
    virtual void Unknown14() = 0;
    virtual int ApplyArea(const CameraVector *origin,
        const CameraVector *extent, int effects, int mode) = 0;

    unsigned int unknown004;
    StageAuxiliaryAreaSubscriber *next;
    int state;
    unsigned char unknown010[0x69 - 0x10];
    unsigned char enabled;
};

// A wider view of the existing 0x4A8E94 clear manager, not a second data
// owner. The original function publishes two adjacent three-float vectors.
struct StageAuxiliaryAreaManager
{
    unsigned char unknown000[0x18];
    StageAuxiliaryAreaSubscriber *first;
    unsigned char unknown01c[0x45c - 0x1c];
    CameraVector origin;
    CameraVector extent;
};

typedef char StageAuxiliaryAreaNextOffset[
    (offsetof(StageAuxiliaryAreaSubscriber, next) == 8) ? 1 : -1];
typedef char StageAuxiliaryAreaStateOffset[
    (offsetof(StageAuxiliaryAreaSubscriber, state) == 0xc) ? 1 : -1];
typedef char StageAuxiliaryAreaEnabledOffset[
    (offsetof(StageAuxiliaryAreaSubscriber, enabled) == 0x69) ? 1 : -1];
typedef char StageAuxiliaryAreaManagerOriginOffset[
    (offsetof(StageAuxiliaryAreaManager, origin) == 0x45c) ? 1 : -1];
typedef char StageAuxiliaryAreaManagerExtentOffset[
    (offsetof(StageAuxiliaryAreaManager, extent) == 0x468) ? 1 : -1];

// Full 0x424F30..0x424FA1 original. The area callback is a normal C++
// virtual function, not an assembly shim or a guessed private-register ABI.
int __stdcall DispatchStageAuxiliaryArea(
    const CameraVector *origin, const CameraVector *extent, int effects)
{
    StageAuxiliaryAreaManager *manager =
        reinterpret_cast<StageAuxiliaryAreaManager *>(g_SecondaryClearManager);
    StageAuxiliaryAreaSubscriber *object = manager->first;

    manager->origin = *origin;
    manager->extent = *extent;

    int total = 0;
    while (object)
    {
        StageAuxiliaryAreaSubscriber *next = object->next;
        if (object->state != 1 && object->enabled)
        {
            const int result = object->ApplyArea(origin, extent, effects, 1);
            total = static_cast<int>(static_cast<unsigned int>(total) +
                static_cast<unsigned int>(result));
        }
        object = next;
    }
    return total;
}
