#pragma once
#include "SecondaryPlayerMovement.hpp"

struct SecondaryAttractionItemView
{
    int state;
    unsigned int unknown004, unknown008;
    float parameter;
    unsigned char unknown010[0x478 - 0x10];
};
struct SecondaryAttractionManagerView
{
    unsigned char unknown000[0x478];
    SecondaryAttractionItemView items[0x896];
};
struct SecondaryAttractionConfigurationView
{
    unsigned char unknown000[8];
    float parameter;
};
extern SecondaryAttractionManagerView *g_SecondaryAttractionItems;

typedef char SecondaryAttractionItemStride[(sizeof(SecondaryAttractionItemView) == 0x478) ? 1 : -1];
typedef char SecondaryAttractionItemsOffset[(offsetof(SecondaryAttractionManagerView, items) == 0x478) ? 1 : -1];
