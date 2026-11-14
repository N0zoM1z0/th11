#include "SecondaryPlayerController.hpp"
#include <stddef.h>

// Native 0x497ED0 is the double 1/128.
extern const double g_SecondaryIntegerPositionScale;

namespace
{
struct IntegerPositionItemView
{
    unsigned int active;
    unsigned char unknown004[0xe4 - 4];
};
struct IntegerPositionPair
{
    int x, y;
};
struct IntegerPositionControllerView
{
    unsigned char unknown000[0x87c];
    float x, y;
    unsigned char unknown884[4];
    IntegerPositionPair integer;
    unsigned char unknown890[0x7644 - 0x890];
    IntegerPositionItemView items[8];
};
typedef char IntegerPositionOffset[(offsetof(IntegerPositionControllerView, integer) == 0x888) ? 1 : -1];
typedef char IntegerPositionItemsOffset[(offsetof(IntegerPositionControllerView, items) == 0x7644) ? 1 : -1];
}

// 00435630. Set the player position from replay integer coordinates and
// mark the eight items active.
void __stdcall SetSecondaryIntegerPlayerPosition(const int *position)
{
    IntegerPositionControllerView *controller =
        reinterpret_cast<IntegerPositionControllerView *>(g_SecondaryControllerMode);
    controller->integer = *reinterpret_cast<const IntegerPositionPair *>(position);
    controller->x = static_cast<float>(controller->integer.x * g_SecondaryIntegerPositionScale);
    controller->y = static_cast<float>(controller->integer.y * g_SecondaryIntegerPositionScale);
    for (int i = 0; i < 8; ++i)
        controller->items[i].active = 1;
}
