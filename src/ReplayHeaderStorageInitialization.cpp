#include "ReplayHeaderStorageInitialization.hpp"
#include <string.h>

// 00420D80. Reset the settings block to its defaults.
void __stdcall InitializeReplayHeaderStorage(ReplayHeaderStorageView *storage)
{
    memset(storage, 0, sizeof(*storage));
    storage->flags |= 0x100;
    storage->value016 = 600;
    storage->byte01a = 0;
    storage->byte01d = 0;
    storage->byte01e = 0;
    storage->version = 0x110003;
    storage->value018 = 600;
    storage->byte01b = 1;
    storage->byte01c = 1;
    storage->defaults = g_ReplayHeaderStorageDefaults;
    storage->byte01f = 2;
    storage->byte023 = 2;
    storage->byte022 = 0;
    storage->byte020 = 100;
    storage->byte021 = 80;
    storage->value028 = 0x80000000;
    storage->value02c = 0x80000000;
}
