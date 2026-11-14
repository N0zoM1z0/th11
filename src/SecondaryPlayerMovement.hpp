#pragma once
#include "SecondaryPlayerController.hpp"

// Physical movement fields; original class identity and total size remain open.
struct SecondaryPlayerMovementView
{
    unsigned char unknown000[0x3b0];
    short primarySprite;
    unsigned char unknown3b2[0x87c - 0x3b2];
    CameraVector position;
    SecondaryIntegerPair fixedPosition;
    int normalSpeed, focusedSpeed, normalDiagonalSpeed, focusedDiagonalSpeed;
    CameraVector movement, rememberedMovement;
    int stepX, stepY;
    unsigned char unknown8c0[0x914 - 0x8c0];
    int previousX, previousY, direction, fireCount, unknown924, mode;
    SecondaryPatternConfigurationView *configuration;
    StageTimerView patternTime, stateTime, auxiliaryTime;
    unsigned char unknown96c[0x756c - 0x96c];
    unsigned int focusedId;
    SecondaryEmitterObjectView emitters[8];
    int patternSlot;
    unsigned char unknown7c94[0x8ba0 - 0x7c94];
    int blend, stillFrames, movingFrames, multiplier;
    StageTimerView statusTime;
    unsigned int flags;
    int collapseFrames;
    unsigned char unknown8bcc[0x8d20 - 0x8bcc];
    int focused;
};

void __stdcall SignalSecondaryAnimationEvent(AnimationVmId id, short event);
void __stdcall UpdateSecondaryPlayerHorizontalWrap(SecondaryDamageReferencesView *self);
void __stdcall AttractSecondaryPlayerItems();

typedef char SecondaryMovementSpeedOffset[(offsetof(SecondaryPlayerMovementView, normalSpeed) == 0x890) ? 1 : -1];
typedef char SecondaryMovementDirectionOffset[(offsetof(SecondaryPlayerMovementView, direction) == 0x91c) ? 1 : -1];
typedef char SecondaryMovementFocusVmOffset[(offsetof(SecondaryPlayerMovementView, focusedId) == 0x756c) ? 1 : -1];
typedef char SecondaryMovementEmittersOffset[(offsetof(SecondaryPlayerMovementView, emitters) == 0x7570) ? 1 : -1];
typedef char SecondaryMovementBlendOffset[(offsetof(SecondaryPlayerMovementView, blend) == 0x8ba0) ? 1 : -1];
typedef char SecondaryMovementFlagsOffset[(offsetof(SecondaryPlayerMovementView, flags) == 0x8bc4) ? 1 : -1];
typedef char SecondaryMovementFocusOffset[(offsetof(SecondaryPlayerMovementView, focused) == 0x8d20) ? 1 : -1];
