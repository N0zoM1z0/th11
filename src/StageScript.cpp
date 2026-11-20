#include "StageScript.hpp"
#include "AngleMath.hpp"
#include "MainRuntime.hpp"
#include <math.h>
#include <new>
#include <intrin.h>

static __forceinline StageScriptStateView &ScriptState(StageResourceView *self)
{
    return *reinterpret_cast<StageScriptStateView *>(self);
}

static __forceinline StageFogView &CameraFog(StageResourceView *self)
{
    return *reinterpret_cast<StageFogView *>(&self->camera.fogStart);
}

static __forceinline void ScriptVector(CameraVector &output, const float *values)
{
    output.x = values[0];
    output.y = values[1];
    output.z = values[2];
}

static __forceinline void ScriptTweenFrame(StageTimerView &timer, int frame)
{
    unsigned int flags = timer.flags;
    if (!(flags & 1))
    {
        timer.current = 0;
        _ReadWriteBarrier();
        timer.previous = -999999;
        _ReadWriteBarrier();
        timer.fraction = 0.0f;
        _ReadWriteBarrier();
        flags |= 1;
        _ReadWriteBarrier();
        timer.scale = &g_AnimationTimeScale;
        _ReadWriteBarrier();
        timer.flags = flags;
    }
    timer.current = 0;
    _ReadWriteBarrier();
    timer.fraction = 0.0f;
    _ReadWriteBarrier();
    timer.previous = frame;
}

static __forceinline void ScriptTime(StageTimerView &timer, int frame)
{
    if (!(timer.flags & 1))
    {
        // Preserve the native field-write order before publishing the flag.
        timer.current = 0;
        timer.previous = -999999;
        timer.fraction = 0.0f;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.current = frame;
    timer.previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
    timer.fraction = static_cast<float>(frame);
}

static __forceinline float OscillatorSine(float time, double inversePeriod)
{
    const double pi = static_cast<double>(3.1415927410125732f);
    double angle = time * pi;
    float phase = static_cast<float>((angle * 2.0) * inversePeriod - pi);
    return static_cast<float>(sin(static_cast<double>(phase)));
}

int __stdcall ExecuteStageScript(StageResourceView *self)
{
    StageScriptStateView &state = ScriptState(self);
    CameraVector bezierA, bezierB;
    for (;;)
    {
        int currentFrame = state.scriptTime.current;
        if (state.instruction->frame > currentFrame)
            break;
        StageScriptInstruction *instruction = state.instruction;
        const int *words = instruction->args.words;
        const float *values = instruction->args.values;
        switch (instruction->opcode)
        {
        case 0:
            goto ApplyTweens;
        case 1:
            ScriptTime(state.scriptTime, words[1]);
            state.instruction = reinterpret_cast<StageScriptInstruction *>(
                self->section008 + state.instruction->args.words[0]);
            continue;
        case 2:
        {
            self->camera.updateOffset = self->camera.view.position;
            ScriptVector(self->camera.view.position, values);
            bezierA.x = self->camera.view.position.x - self->camera.updateOffset.x;
            bezierA.y = self->camera.view.position.y - self->camera.updateOffset.y;
            bezierA.z = self->camera.view.position.z - self->camera.updateOffset.z;
            self->camera.updateOffset = bezierA;
            break;
        }
        case 3:
        {
            CameraVector target = { values[2], values[3], values[4] };
            state.positionTween.duration = words[0];
            state.positionTween.mode = words[1];
            state.positionTween.initial = self->camera.view.position;
            state.positionTween.target = target;
        }
            ScriptTweenFrame(state.positionTween.time, -1);
            break;
        case 4:
            ScriptVector(self->camera.view.targetOffset, values);
            break;
        case 5:
        {
            CameraVector target = { values[2], values[3], values[4] };
            state.targetTween.duration = words[0];
            state.targetTween.mode = words[1];
            state.targetTween.initial = self->camera.view.targetOffset;
            state.targetTween.target = target;
        }
            ScriptTweenFrame(state.targetTween.time, -1);
            break;
        case 6:
            ScriptVector(self->camera.view.up, values);
            break;
        case 7:
            self->camera.view.fieldOfView = values[0];
            break;
        case 8:
        {
            StageFogView &fog = CameraFog(self);
            fog.packed = words[0];
            const unsigned char *color = reinterpret_cast<const unsigned char *>(&fog.packed);
            const int red = color[0];
            const int green = color[1];
            const int blue = color[2];
            const int alpha = color[3];
            _ReadWriteBarrier();
            fog.channels[0] = static_cast<float>(red);
            fog.channels[1] = static_cast<float>(green);
            fog.channels[2] = static_cast<float>(blue);
            fog.channels[3] = static_cast<float>(alpha);
            fog.start = values[1];
            fog.end = values[2];
            break;
        }
        case 9:
        {
            StageFogView fog;
            fog.start = values[3];
            fog.end = values[4];
            const unsigned char *color = reinterpret_cast<const unsigned char *>(&words[2]);
            fog.channels[0] = static_cast<float>(color[0]);
            fog.channels[1] = static_cast<float>(color[1]);
            fog.channels[2] = static_cast<float>(color[2]);
            fog.channels[3] = static_cast<float>(color[3]);
            fog.Pack();
            state.fogTween.duration = words[0];
            state.fogTween.mode = words[1];
            state.fogTween.initial = CameraFog(self);
            state.fogTween.target = fog;
            state.fogTween.time.SetFrame(-1);
            break;
        }
        case 10:
        {
            ScriptVector(bezierA, values + 2);
            ScriptVector(bezierB, values + 5);
            CameraVector control2 = { values[8], values[9], values[10] };
            state.positionTween.duration = words[0];
            state.positionTween.initial = self->camera.view.position;
            state.positionTween.control1 = bezierA;
            state.positionTween.target = bezierB;
            state.positionTween.control2 = control2;
            state.positionTween.mode = 8;
            ScriptTweenFrame(state.positionTween.time, -1);
            break;
        }
        case 11:
        {
            ScriptVector(bezierA, values + 2);
            ScriptVector(bezierB, values + 5);
            CameraVector control2 = { values[8], values[9], values[10] };
            state.targetTween.duration = words[0];
            state.targetTween.initial = self->camera.view.targetOffset;
            state.targetTween.control1 = bezierA;
            state.targetTween.target = bezierB;
            state.targetTween.control2 = control2;
            state.targetTween.mode = 8;
            ScriptTweenFrame(state.targetTween.time, -1);
            break;
        }
        case 12:
            state.oscillator = static_cast<unsigned char>(words[0]);
            if (!state.oscillator)
            {
                self->camera.view.eyeOffset.x = 0.0f;
                self->camera.view.eyeOffset.y = 0.0f;
                self->camera.view.eyeOffset.z = 0.0f;
                self->camera.view.up.x = 0.0f;
                self->camera.view.up.y = 1.0f;
                self->camera.view.up.z = 0.0f;
            }
            state.oscillatorTime.SetFrame(-1);
            break;
        case 13:
            *reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x9c0) = words[0];
            break;
        case 14:
        {
            int script = words[1];
            if (script >= 0)
            {
                AnimationFileView *file = reinterpret_cast<AnimationFileView *>(self->animation);
                AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
                    reinterpret_cast<unsigned char *>(self->vms180) + words[0] * static_cast<int>(sizeof(AnimationVmView)));
                ResetAnimationVm(vm);
                reinterpret_cast<unsigned char *>(vm)[0x41d] = 0x10;
                reinterpret_cast<unsigned char *>(vm)[0x41c] = 0x10;
                BindAnimationScript(file, script, vm);
            }
            else
            {
                AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
                    reinterpret_cast<unsigned char *>(self->vms180) + words[0] * static_cast<int>(sizeof(AnimationVmView)));
                vm->flags.value &= ~1u;
            }
            break;
        }
        case 17:
        {
            VmBindingBlockView *block = self->bindings;
            if (block)
            {
                ReleaseVmBindingBlock(block);
                operator delete(block);
            }
            self->minimumRadius = 112.0f;
            self->bindings = 0;
            self->unknown301c = 0xffffffff;
            self->radius = 192.0f;
            float phase = NormalizeAngle(0.0f);
            self->phaseY = self->phaseX = phase;
            // Keep the cursor read after angle normalization: the native
            // reloads this live member after the call sequence.
            _ReadWriteBarrier();
            self->distortionMode = state.instruction->args.words[0];
            if (self->minimumRadius > 0.0f)
            {
                if (self->distortionMode == 1)
                    self->bindings = reinterpret_cast<VmBindingBlockView *>(new StageScriptMesh(17, 25, 0));
                else
                    self->bindings = reinterpret_cast<VmBindingBlockView *>(new StageScriptMesh(17, 17, 0));
            }
            break;
        }
        }
        state.instruction = reinterpret_cast<StageScriptInstruction *>(
            reinterpret_cast<unsigned char *>(state.instruction) + state.instruction->byteSize);
    }
    AdvanceStageTimer(&state.scriptTime);

ApplyTweens:
    if (state.targetTween.duration)
        self->camera.view.targetOffset = StepStageVectorTween(&state.targetTween);
    if (state.positionTween.duration)
        self->camera.view.position = StepStageVectorTween(&state.positionTween);
    if (state.fogTween.duration)
    {
        StageFogView fog;
        StageFogView *result = StepStageFogTweenInto(&state.fogTween, &fog);
        CameraFog(self) = *result;
    }

    float value;
    switch (state.oscillator)
    {
    case 1:
        value = OscillatorSine(state.oscillatorTime.fraction, 1.0 / 512.0);
        self->camera.view.eyeOffset.x = static_cast<float>(-50.0 * value);
        self->camera.view.up.x = static_cast<float>(-value * static_cast<double>(0.1f));
        AdvanceStageTimer(&state.oscillatorTime);
        if (state.oscillatorTime.current >= 512)
            SetStageTimer(&state.oscillatorTime, 0);
        return 0;
    case 5:
    {
        const double pi = static_cast<double>(3.1415927410125732f);
        double angle = state.oscillatorTime.fraction * pi;
        float phase = static_cast<float>((angle * 2.0) * (1.0 / 2048.0) - pi);
        value = static_cast<float>(sin(static_cast<double>(phase)));
        self->camera.view.eyeOffset.x = static_cast<float>(value * 70.0);
        float twice = NormalizeAngle(static_cast<float>(phase + phase));
        float other = static_cast<float>(sin(static_cast<double>(twice)));
        self->camera.view.eyeOffset.z = static_cast<float>(other * 200.0);
        self->camera.view.up.x = static_cast<float>(-value * static_cast<double>(0.1f));
        AdvanceStageTimer(&state.oscillatorTime);
        if (state.oscillatorTime.current >= 2048)
            SetStageTimer(&state.oscillatorTime, 0);
        return 0;
    }
    case 6:
        value = OscillatorSine(state.oscillatorTime.fraction, 1.0 / 1024.0);
        self->camera.view.eyeOffset.x = static_cast<float>(-50.0 * value);
        self->camera.view.up.x = static_cast<float>(-value * static_cast<double>(0.1f));
        AdvanceStageTimer(&state.oscillatorTime);
        if (state.oscillatorTime.current >= 1024)
            SetStageTimer(&state.oscillatorTime, 0);
        return 0;
    case 7:
        value = OscillatorSine(state.oscillatorTime.fraction, 1.0 / 512.0);
        self->camera.view.eyeOffset.x = static_cast<float>(-15.0 * value);
        self->camera.view.up.x = static_cast<float>(-value * static_cast<double>(0.01f));
        AdvanceStageTimer(&state.oscillatorTime);
        if (state.oscillatorTime.current >= 512)
            SetStageTimer(&state.oscillatorTime, 0);
        return 0;
    default:
        return 0;
    }
    return 0;
}
