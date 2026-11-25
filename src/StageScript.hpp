#pragma once

#include "StageUpdate.hpp"
#include "StageTweenTypes.hpp"

struct StageScriptInstruction
{
    int frame;
    short opcode, byteSize;
    union { int words[11]; float values[11]; } args;
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

typedef char StageScriptCursorOffset[(offsetof(StageScriptStateView, instruction) == 0x4c) ? 1 : -1];
typedef char StageScriptPositionOffset[(offsetof(StageScriptStateView, positionTween) == 0x9c) ? 1 : -1];
typedef char StageScriptFogOffset[(offsetof(StageScriptStateView, fogTween) == 0xe8) ? 1 : -1];
typedef char StageScriptFogTimerOffset[(offsetof(StageFogTween, time) == 0x70) ? 1 : -1];
typedef char StageScriptMeshSize[(sizeof(StageScriptMesh) == 0x18) ? 1 : -1];

CameraVector __stdcall StepStageVectorTween(StageVectorTween *self);
StageFogView __stdcall StepStageFogTween(StageFogTween *self);
StageFogView *__stdcall StepStageFogTweenInto(StageFogTween *self, StageFogView *output);
void __fastcall SeekStageScriptMarker(void *unused, int marker);
void __cdecl InitializePrimaryStageScripts();
int __stdcall AdvanceStageTimer(StageTimerView *self);
void __stdcall SetStageTimer(StageTimerView *self, int frame);
