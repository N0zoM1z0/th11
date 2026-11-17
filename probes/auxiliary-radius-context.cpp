#include "StageAuxiliary.hpp"

int __stdcall UpdateStageAuxiliaryMode1Effect(StageAuxiliaryView *self);

// The original mode-1 effect calls the 395-byte scene-radius boss, which in
// turn calls the 449-byte actor-radius boss. Keep this dynamic caller live.
extern "C" int __stdcall AuxiliaryRadiusProbe(StageAuxiliaryView *owner)
{
    return UpdateStageAuxiliaryMode1Effect(owner);
}
