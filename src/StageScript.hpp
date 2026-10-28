#pragma once

#include "StageUpdate.hpp"

struct StageScriptInstruction
{
    int frame;
    short opcode, byteSize;
    union { int words[11]; float values[11]; } args;
};

struct StageVectorTween
{
    CameraVector initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};

struct StageFogView
{
    float start, end;
    float channels[4];
    unsigned int packed;

    void Pack();
};

struct StageFogTween
{
    StageFogView initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};

// A further physical view of the existing stage, without changing its layout.
struct StageScriptStateView
{
    unsigned char unknown000[0x20];
    unsigned char oscillator;
    unsigned char unknown021[3];
    StageTimerView oscillatorTime, scriptTime;
    StageScriptInstruction *instruction;
    StageVectorTween targetTween, positionTween;
    StageFogTween fogTween;
};

struct StageScriptMesh : StageMeshView
{
    StageScriptMesh(int rows, int columns, int flags);
};

typedef char StageVectorTweenSize[(sizeof(StageVectorTween) == 0x4c) ? 1 : -1];
typedef char StageFogSize[(sizeof(StageFogView) == 0x1c) ? 1 : -1];
typedef char StageScriptCursorOffset[(offsetof(StageScriptStateView, instruction) == 0x4c) ? 1 : -1];
typedef char StageScriptPositionOffset[(offsetof(StageScriptStateView, positionTween) == 0x9c) ? 1 : -1];
typedef char StageScriptFogOffset[(offsetof(StageScriptStateView, fogTween) == 0xe8) ? 1 : -1];
typedef char StageScriptFogTimerOffset[(offsetof(StageFogTween, time) == 0x70) ? 1 : -1];
typedef char StageScriptMeshSize[(sizeof(StageScriptMesh) == 0x18) ? 1 : -1];

// Reviewed external boundaries. Bodies are not supplied by these declarations.
CameraVector __stdcall StepStageVectorTween(StageVectorTween *self);
StageFogView __stdcall StepStageFogTween(StageFogTween *self);
void __stdcall AdvanceStageTimer(StageTimerView *self);
void __stdcall SetStageTimer(StageTimerView *self, int frame);
