// Complete host instruction switch body, in the native case-body order
// (VC9 lays case bodies out lexically). Guarded spawn entries fall into
// their unguarded entries.
// The spawn entries are adjacent in the native layout and share one lexical
// scope and packet, whose stack slot is reused by later opcode families.
{
SecondaryHostSpawnArguments arguments;
int words;
case 0x109:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x100:
{
    words = SpawnWords(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = self->world.position.x;
    if (!(self->flags & 0x400000))
    {
        arguments.position.x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real) + x;
        float y = self->world.position.y;
        arguments.position.y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real) + y;
    }
    else
    {
        CameraVector position;
        position.x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real) + x;
        float y = self->world.position.y;
        position.y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real) + y;
        position.z = self->world.position.z;
        SelectSecondaryHostCamera(&g_SecondaryHostProjectionRenderer, 0);
        CameraView &camera = g_SecondaryHostProjectionRenderer.camera;
        D3DXVec3Project(&arguments.position, &position, &camera.viewport,
            &camera.projection, &camera.view, &g_SecondaryHostProjectionWorld);
        arguments.position.x = static_cast<float>(arguments.position.x - 224.0);
        arguments.position.y = static_cast<float>(arguments.position.y - 16.0);
        arguments.position.z = 0.0f;
    }
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, SpawnPayload(instruction)[words + 2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, SpawnPayload(instruction)[words + 3].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, SpawnPayload(instruction)[words + 4].integer);
    SpawnParameters(self, &arguments);
    SpawnActor(instruction, &arguments);
    break;
}
case 0x10a:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x101:
{
    words = SpawnWords(instruction);
    memset(&arguments, 0, sizeof(arguments));
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real);
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real);
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, SpawnPayload(instruction)[words + 2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, SpawnPayload(instruction)[words + 3].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, SpawnPayload(instruction)[words + 4].integer);
    SpawnParameters(self, &arguments);
    SpawnActor(instruction, &arguments);
    break;
}
case 0x10f:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x10e:
{
    words = SpawnWords(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real);
    arguments.position.x = x + SceneOffset().x;
    float y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real);
    arguments.position.y = y + SceneOffset().y;
    arguments.position.z = ReadSecondaryHostRealValue(self, 3, SpawnPayload(instruction)[words + 2].real);
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 4, SpawnPayload(instruction)[words + 3].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 5, SpawnPayload(instruction)[words + 4].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 6, SpawnPayload(instruction)[words + 5].integer);
    SpawnParameters(self, &arguments);
    arguments.flags |= 1;
    SpawnActor(instruction, &arguments);
    break;
}
case 0x10b:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x104:
{
    words = SpawnWords(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = self->world.position.x;
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real) + x;
    float y = self->world.position.y;
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real) + y;
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, SpawnPayload(instruction)[words + 2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, SpawnPayload(instruction)[words + 3].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, SpawnPayload(instruction)[words + 4].integer);
    SpawnParameters(self, &arguments);
    arguments.mirrored = 1;
    SpawnActor(instruction, &arguments);
    break;
}
case 0x10c:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x105:
{
    words = SpawnWords(instruction);
    memset(&arguments, 0, sizeof(arguments));
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, SpawnPayload(instruction)[words + 0].real);
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, SpawnPayload(instruction)[words + 1].real);
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, SpawnPayload(instruction)[words + 2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, SpawnPayload(instruction)[words + 3].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, SpawnPayload(instruction)[words + 4].integer);
    SpawnParameters(self, &arguments);
    arguments.mirrored = 1;
    SpawnActor(instruction, &arguments);
    break;
}
}
case 0x102:
    Selection(self).file = Integer(self, 0);
    break;
case 0x130:
{
    unsigned int value = static_cast<unsigned int>(Integer(self, 0));
    self->flags = (self->flags & ~0x8000u) | ((value << 15) & 0x8000u);
    break;
}
case 0x1ba:
{
    int value = Integer(self, 0);
    MasterFlags() = (MasterFlags() & ~1u) | (value & 1);
    break;
}
case 0x174:
    self->enabled = Integer(self, 0);
secondary_host_control_done:
    break;
case 0x1c2:
{
    int score = Integer(self, 0);
    AddSecondaryActorScore(score);
    ShowSecondaryHostScore(score, &self->world.position, -1);
    break;
}
case 0x103:
{
    int slot = Integer(self, 0);
    int script = Integer(self, 1);
    unsigned int *id = &self->animationIds[slot];
    ReleaseSecondaryAnimation(AnimationVmId(*id));
    *id = 0;
    if (script >= 0)
    {
        script = Integer(self, 1);
        AnimationVmId created = CreateSecondaryEmitterAnimationVmId(
            File(Selection(self).file), script, AddInteger(self->enabled, 6));
        *id = created.value;
        if (slot == 0)
        {
            Selection(self).savedScript = Integer(self, 1);
            Selection(self).savedFile = Selection(self).file;
        }
        AnimationVmView *vm = ClearMissingAnimationId(id);
        FinishAnimationReplacement(self, id, slot, vm);
    }
    break;
}
case 0x115:
{
    int slot = Integer(self, 0);
    angleVm = ClearMissingAnimationId(&self->animationIds[slot]);
    if (!angleVm) break;
    angleIndex = 1;
}
hostPublishAnimationAngle:
    AnimationState(angleVm)->angle = Real(self, angleIndex);
    angleVm->flags.value |= 4;
    break;
case 0x107:
    if (!(self->flags & 0x400000))
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        CreateOffsetSecondaryEmitterAnimationVmId(File(file), &self->world.position, script);
    }
    else
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        CreatePositionedSecondaryEmitterAnimationVmId(File(file), &self->world.position, script);
    }
    break;
case 0x110:
    if (!(self->flags & 0x400000))
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        File(file)->CreatePositionedVmId(&self->world.position, script, 8);
    }
    else
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        CreateDirectPositionAnimationVmId(File(file), &self->world.position, script, 8);
    }
    break;
case 0x108:
{
    int file = Integer(self, 0);
    int script = Integer(self, 1);
    CreateSecondaryEmitterAnimationVmId(File(file), script, 8);
    break;
}
case 0x111:
{
    AnimationVmId id(0);
    if (!(self->flags & 0x400000))
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        id = CreateOffsetSecondaryEmitterAnimationVmId(File(file), &self->world.position, script);
    }
    else
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        id = CreatePositionedSecondaryEmitterAnimationVmId(File(file), &self->world.position, script);
    }
    AnimationVmView *vm = ClearMissingAnimationId(&id.value);
    if (!(self->flags & 0x400000))
        AnimationState(vm)->position = *OffsetSecondaryHostPosition(&self->world.position);
    else
        AnimationState(vm)->position = self->world.position;
    angleVm = vm;
    angleIndex = 2;
    goto hostPublishAnimationAngle;
}
case 0x106:
{
    int slot = Integer(self, 0);
    int script = Integer(self, 1);
    unsigned int *id = &self->animationIds[slot];
    ReleaseSecondaryAnimation(AnimationVmId(*id));
    *id = 0;
    AnimationVmId created = CreateSecondaryEmitterAnimationVmId(File(Selection(self).file), script, 7);
    *id = created.value;
    AnimationVmView *vm = ClearMissingAnimationId(id);
    FinishAnimationReplacement(self, id, slot, vm);
    if (slot == 0)
    {
        self->flags |= 0x10000;
        self->scriptBase = script;
        self->direction = 0;
        Selection(self).savedScript = script;
        Selection(self).savedFile = Selection(self).file;
    }
    break;
}
case 0x114:
{
    ReleaseSecondaryAnimation(AnimationVmId(self->animationIds[0]));
    self->animationIds[0] = 0;
    AnimationVmId id = CreateSecondaryEmitterAnimationVmId(File(Selection(self).file), self->scriptBase, 7);
    self->flags &= ~0x10000u;
    self->animationIds[0] = id.value;
    self->direction = 0;
    Selection(self).savedScript = self->scriptBase;
    Selection(self).savedFile = Selection(self).file;
    break;
}
case 0x10d:
{
    int slot = Integer(self, 0);
    unsigned int *id = &self->animationIds[slot];
    ReleaseSecondaryAnimation(AnimationVmId(*id));
    *id = 0;
    AnimationVmId created = CreateSecondaryEmitterAnimationVmId(
        File(Selection(self).file), AddInteger(self->scriptBase, 5), 7);
    *id = created.value;
    AnimationVmView *vm = ClearMissingAnimationId(id);
    FinishAnimationReplacement(self, id, slot, vm);
    break;
}
case 0x112:
{
    int slot = Integer(self, 0);
    unsigned int *id = &self->animationIds[slot];
    ReleaseSecondaryAnimation(AnimationVmId(*id));
    *id = 0;
    int offset = Integer(self, 1);
    int script = AddInteger(AddInteger(self->scriptBase, offset), 5);
    AnimationVmId created = CreateSecondaryEmitterAnimationVmId(File(Selection(self).file), script, 7);
    *id = created.value;
    AnimationVmView *vm = ClearMissingAnimationId(id);
    FinishAnimationReplacement(self, id, slot, vm);
    break;
}
case 0x113:
{
    int slot = Integer(self, 0);
    short event = static_cast<short>(Integer(self, 1));
    SignalSecondaryAnimationEvent(AnimationVmId(self->animationIds[slot]), event);
    break;
}
case 0x133:
    self->local.position.x = self->offset.position.x + self->local.position.x;
    self->local.position.y = self->offset.position.y + self->local.position.y;
    self->local.position.z = self->offset.position.z + self->local.position.z;
    ResetSecondaryHostVector(&self->offset.position);
    self->offset.speed = 0.0f;
    self->local.speed = 0.0f;
    self->offset.velocity = g_SecondaryHostVectorPreset;
    self->local.velocity = g_SecondaryHostVectorPreset;
    self->offset.flags &= ~1u;
    self->local.flags &= ~1u;
    self->offset.flags &= ~2u;
    self->local.flags &= ~2u;
    self->localTween.duration = 0;
    self->offsetTween.duration = 0;
    self->pairTweens[0].duration = 0;
    self->pairTweens[1].duration = 0;
    self->pairTweens[2].duration = 0;
    self->pairTweens[3].duration = 0;
    self->pairTweens[4].duration = 0;
    self->pairTweens[5].duration = 0;
    break;
case 0x118:
case 0x11a:
{
    SecondaryMotionView *motion = opcode == 0x118 ? &self->local : &self->offset;
    float x = Real(self, 0);
    float y = Real(self, 1);
    if (Present(x)) motion->position.x = x;
    if (Present(y)) motion->position.y = y;
    motion->flags &= ~1u;
    RefreshWorldPosition(self);
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x128:
case 0x129:
{
    SecondaryMotionView *motion = opcode == 0x128 ? &self->local : &self->offset;
    float x = Real(self, 0);
    float y = Real(self, 1);
    float z = Real(self, 2);
    motion->position.x += x;
    motion->position.y += y;
    motion->position.z = z + motion->position.z;
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x119:
case 0x11b:
{
    SecondaryMotionView *motion = opcode == 0x119 ? &self->local : &self->offset;
    StageVectorTween *tween = opcode == 0x119 ? &self->localTween : &self->offsetTween;
    float x = Real(self, 2);
    float y = Real(self, 3);
    tween->duration = Integer(self, 0);
    tween->control1 = g_SecondaryHostVectorPreset;
    tween->control2 = g_SecondaryHostVectorPreset;
    tween->mode = Integer(self, 1);
    tween->initial = motion->position;
    CameraVector target = {
        Present(x) ? x : motion->position.x,
        Present(y) ? y : motion->position.y, 0.0f
    };
    tween->target = target;
    ResetSecondaryHostVectorTimer(tween);
    motion->flags &= ~1u;
    break;
}
case 0x131:
case 0x132:
{
    SecondaryMotionView *motion = opcode == 0x131 ? &self->local : &self->offset;
    StageVectorTween *tween = opcode == 0x131 ? &self->localTween : &self->offsetTween;
    float x = Real(self, 3);
    float y = Real(self, 4);
    CameraVector control1 = { Real(self, 1), Real(self, 2), 0.0f };
    CameraVector control2 = { Real(self, 5), Real(self, 6), 0.0f };
    tween->duration = Integer(self, 0);
    tween->control1 = control1;
    tween->control2 = control2;
    tween->mode = 8;
    tween->initial = motion->position;
    CameraVector target = {
        Present(x) ? x : motion->position.x,
        Present(y) ? y : motion->position.y, 0.0f
    };
    tween->target = target;
    ResetSecondaryHostVectorTimer(tween);
    motion->flags &= ~1u;
    break;
}
case 0x11c:
case 0x11e:
{
    SecondaryMotionView *motion = opcode == 0x11c ? &self->local : &self->offset;
    float angle = Real(self, 0);
    float speed = Real(self, 1);
    if (Present(angle))
    {
        if (self->flags & 0x8000) angle = MirroredAngle(angle);
        motion->angle = NormalizeAngle(angle);
    }
    motion->flags &= ~1u;
    if (Present(speed)) motion->speed = speed;
    break;
}
case 0x11d:
case 0x11f:
{
    SecondaryMotionView *motion = opcode == 0x11d ? &self->local : &self->offset;
    SecondaryPairTween *tween = &self->pairTweens[opcode == 0x11d ? 0 : 1];
    float angle = Real(self, 2);
    float speed = Real(self, 3);
    tween->mode = Integer(self, 1);
    SecondaryVector2 target;
    if (tween->mode == 7)
    {
        target.x = Present(angle) ? angle : 0.0f;
        target.y = Present(speed) ? speed : 0.0f;
    }
    else
    {
        if (Present(angle))
        {
            if (self->flags & 0x8000) angle = MirroredAngle(angle);
        }
        else angle = motion->angle;
        target.x = angle;
        target.y = Present(speed) ? speed : motion->speed;
    }
    SecondaryVector2 initial = { motion->angle, motion->speed };
    float difference = initial.x - target.x;
    if (Pi <= AnimationExtentMagnitude(difference))
    {
        if (target.x > initial.x) initial.x = static_cast<float>(initial.x + TwoPi);
        else target.x = static_cast<float>(target.x + TwoPi);
    }
    tween->duration = Integer(self, 0);
    PairControls(tween);
    tween->initial = initial;
    tween->target = target;
    ResetSecondaryPairTimer(tween);
    motion->flags &= ~1u;
    break;
}
case 0x120:
case 0x122:
{
    SecondaryMotionView *motion = opcode == 0x120 ? &self->local : &self->offset;
    float angle = Real(self, 0);
    float speed = Real(self, 1);
    float radius = Real(self, 2);
    float radialVelocity = Real(self, 3);
    unsigned int flags = motion->flags;
    if (!(flags & 1)) motion->velocity = motion->position;
    if (Present(angle)) motion->angle = NormalizeAngle(angle);
    if (Present(speed)) motion->speed = speed;
    if (Present(radius)) motion->radius = radius;
    if (Present(radialVelocity)) motion->radialVelocity = radialVelocity;
    motion->flags = flags | 1;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x121:
case 0x123:
{
    SecondaryMotionView *motion = opcode == 0x121 ? &self->local : &self->offset;
    SecondaryPairTween *angularTween = opcode == 0x121 ? &self->pairTweens[0] : &self->pairTweens[1];
    SecondaryPairTween *radialTween = opcode == 0x121 ? &self->pairTweens[2] : &self->pairTweens[3];
    float speed = Real(self, 2);
    float radius = Real(self, 3);
    float radialVelocity = Real(self, 4);
    SecondaryVector2 angularInitial = { motion->angle, motion->speed };
    SecondaryVector2 angularTarget = { 0.0f, Present(speed) ? speed : motion->speed };
    SecondaryVector2 radialInitial = { motion->radius, motion->radialVelocity };
    SecondaryVector2 radialTarget = {
        Present(radius) ? radius : motion->radius,
        Present(radialVelocity) ? radialVelocity : motion->radialVelocity
    };
    int duration = Integer(self, 0);
    int mode = Integer(self, 1);
    Pair(angularTween, angularInitial, angularTarget, duration, mode);
    Pair(radialTween, radialInitial, radialTarget, duration, mode);
    motion->flags |= 1;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x12a:
case 0x12b:
{
    SecondaryMotionView *motion = opcode == 0x12a ? &self->local : &self->offset;
    float x = Real(self, 0);
    float y = Real(self, 1);
    if (Present(x)) motion->velocity.x = x;
    if (Present(y)) motion->velocity.y = y;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
    // Native falls through and rereads the original instruction's opcode.
}
case 0x12c:
case 0x12e:
{
    SecondaryMotionView *motion = instruction->opcode == 0x12c ? &self->local : &self->offset;
    float angle = Real(self, 0);
    float speed = Real(self, 1);
    float radius = Real(self, 2);
    float radialVelocity = Real(self, 3);
    float tilt = Real(self, 4);
    float ellipseRatio = Real(self, 5);
    unsigned int flags = motion->flags;
    if (!(flags & 1)) motion->velocity = motion->position;
    if (Present(angle)) motion->angle = NormalizeAngle(angle);
    if (Present(speed)) motion->speed = speed;
    if (Present(radius)) motion->radius = radius;
    if (Present(radialVelocity)) motion->radialVelocity = radialVelocity;
    if (Present(tilt)) motion->tilt = NormalizeAngle(tilt);
    if (Present(ellipseRatio)) motion->ellipseRatio = ellipseRatio;
    motion->flags = flags | 3;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x12d:
case 0x12f:
{
    SecondaryMotionView *motion = opcode == 0x12d ? &self->local : &self->offset;
    // Keep the three tween selections independent. The combined pair-index
    // expression folds these into one branch and loses the native staging.
    SecondaryPairTween *angular = instruction->opcode == 0x12d ? &self->pairTweens[0] : &self->pairTweens[1];
    SecondaryPairTween *radial = static_cast<short>(opcode) == 0x12d ? &self->pairTweens[2] : &self->pairTweens[3];
    // This case group contains only 12d and 12f; the inverted test preserves
    // the third independent pointer selection in the native branch.
    SecondaryPairTween *ellipse = instruction->opcode != 0x12f ? &self->pairTweens[4] : &self->pairTweens[5];
    float angle = Real(self, 2);
    float speed = Real(self, 3);
    float radius = Real(self, 4);
    float radialVelocity = Real(self, 5);
    float tilt = Real(self, 6);
    float ellipseRatio = Real(self, 7);
    if (!Present(speed)) speed = motion->speed;
    if (!Present(angle)) angle = motion->angle;
    SecondaryVector2 angularInitial = { motion->angle, motion->speed };
    SecondaryVector2 angularTarget = { angle, speed };
    if (!Present(radialVelocity)) radialVelocity = motion->radialVelocity;
    if (!Present(radius)) radius = motion->radius;
    SecondaryVector2 radialInitial = { motion->radius, motion->radialVelocity };
    SecondaryVector2 radialTarget = { radius, radialVelocity };
    if (!Present(ellipseRatio)) ellipseRatio = motion->ellipseRatio;
    if (!Present(tilt)) tilt = motion->tilt;
    SecondaryVector2 ellipseInitial = { motion->tilt, motion->ellipseRatio };
    SecondaryVector2 ellipseTarget = { tilt, ellipseRatio };
    int duration = Integer(self, 0);
    int mode = Integer(self, 1);
    Pair(angular, angularInitial, angularTarget, duration, mode);
    Pair(radial, radialInitial, radialTarget, duration, mode);
    Pair(ellipse, ellipseInitial, ellipseTarget, duration, mode);
    motion->velocity = motion->position;
    motion->flags |= 3;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
    break;
}
case 0x126:
    self->local.position = g_SecondaryMaster->Slot(0)->state.world.position;
    break;
case 0x127:
    self->offset.position = g_SecondaryMaster->Slot(0)->state.world.position;
    break;
case 0x144:
    self->flags |= 0x2000;
    self->boundX = Real(self, 0);
    self->boundY = Real(self, 1);
    self->boundWidth = Real(self, 2);
    self->boundHeight = Real(self, 3);
    break;
case 0x145:
    self->flags &= ~0x2000u;
    break;
case 0x15a:
{
    double value = Real(self, 0);
    *reinterpret_cast<float *>(&self->unknown1590) = static_cast<float>(value * value);
    break;
}
case 0x124:
case 0x125:
{
    SecondaryMotionView *motion = opcode == 0x124 ? &self->local : &self->offset;
    SecondaryPairTween *tween = &self->pairTweens[opcode == 0x124 ? 0 : 1];
    float angle;
    // These bound branches include unordered comparisons; the sentinel gates
    // elsewhere in this fragment use ordered greater-than instead.
    if (!(self->boundX - self->boundWidth * 0.25 > self->world.position.x))
    {
        bool center = !(self->world.position.x > self->boundX + self->boundWidth * 0.25);
        if (center && self->world.position.x < g_SecondaryActorReferences->position.x)
            angle = static_cast<float>(SecondaryHostRandomAngle() * 0.5);
        else
        {
            if (center)
                angle = static_cast<float>(SecondaryHostRandomAngle() * 0.5 + Pi);
            else
                angle = static_cast<float>(SecondaryHostRandomAngle() / 3.0 + Pi);
            angle = NormalizeAngle(angle);
        }
    }
    else angle = static_cast<float>(SecondaryHostRandomAngle() / 3.0);
    if (!(self->boundY - self->boundHeight * 0.25 > self->world.position.y))
    {
        if (self->boundY + self->boundHeight * 0.25 < self->world.position.y)
            angle = -AnimationExtentMagnitude(angle);
    }
    else angle = AnimationExtentMagnitude(angle);
    tween->mode = Integer(self, 1);
    SecondaryVector2 target = { angle, 0.0f };
    SecondaryVector2 initial = { angle, Real(self, 2) };
    tween->duration = Integer(self, 0);
    PairControls(tween);
    tween->initial = initial;
    tween->target = target;
    ResetSecondaryPairTimer(tween);
    motion->flags &= ~1u;
    break;
}
case 0x15b:
{
    int threshold = Integer(self, 2);
    float value = Real(self, 1);
    int total = self->counter.unknown004;
    int slot = Integer(self, 0);
    SecondaryHostControlHud *hud = ControlHud();
    hud->gauges[slot].threshold = threshold;
    hud->gauges[slot].fraction = static_cast<float>(static_cast<double>(value) / total);
    break;
}
case 0x1bb:
    self->hook1630 = g_SecondaryHostCallbacks[Integer(self, 0)];
    break;
case 0x1bc:
    self->hook1634 = g_SecondaryHostUpdateCallbacks[Integer(self, 0)];
    break;
case 0x1bd:
    self->hook1638 = g_SecondaryHostDrawCallbacks[Integer(self, 0)];
    break;
case 0x1c1:
    g_SecondaryHostCallbacks[Integer(self, 0)](self);
    break;
case 0x140:
    self->extentX = Real(self, 0);
    self->extentY = Real(self, 1);
    break;
case 0x141:
    self->extentZ = Real(self, 0);
    self->unknown0dc = Real(self, 1);
    break;
case 0x142:
{
    self->flags |= Integer(self, 0);
    if (self->flags & 0x20)
        for (int i = 0; i < 10; ++i) ClearSecondaryHostAnimationFlag2(&self->animationIds[i]);
    break;
}
case 0x143:
{
    self->flags &= ~Integer(self, 0);
    if (!(self->flags & 0x20))
        for (int i = 0; i < 10; ++i) SetSecondaryHostAnimationFlag2(&self->animationIds[i]);
    break;
}
case 0x146:
    memset(self->effect.unknown004, 0, 0x30);
    break;
case 0x147:
{
    int index = EffectIndex(self, Integer(self, 0));
    int value = Integer(self, 1);
    reinterpret_cast<int *>(&self->effect)[index] = value;
    break;
}
case 0x148:
{
    float height = Real(self, 1);
    self->effect.width = Real(self, 0);
    self->effect.height = height;
    break;
}
case 0x149:
    ResetSecondaryStateWithEffect(&self->effect, &self->world.position);
    break;
case 0x14a:
    self->effect.id = EffectIndex(self, Integer(self, 0));
    break;
case 0x14b:
{
    int value = Integer(self, 0);
    self->counter.value = value;
    self->counter.unknown004 = value;
    if (self->flags & 0x80000)
    {
        SecondaryHostControlHud *hud = ControlHud();
        for (int i = 0; i < 4; ++i)
        {
            hud->gauges[i].fraction = 0.0f;
            hud->gauges[i].threshold = 0;
        }
    }
    self->counter.unknown008 = self->counter.value;
    if (self->flags & 0x80000) self->flags |= 0x4000000;
    self->counter.scaled = ScaleCounter(self->counter.value);
    break;
}
case 0x14c:
{
    int slot = Integer(self, 0);
    MasterFlags() &= ~1u;
    if (slot < 0)
    {
        if (self->flags & 0x80000) g_SecondaryMaster->Slot(self->index) = 0;
        self->flags &= ~0x80000u;
    }
    else
    {
        self->flags |= 0x80000;
        g_SecondaryMaster->Slot(slot) = self->owner;
        self->index = slot;
    }
    break;
}
case 0x14f:
    SetStageTimer(&self->timer1558, Integer(self, 0));
    break;
case 0x169:
    SetStageTimer(&self->timer156c, Integer(self, 0));
    break;
case 0x173:
    ClearSecondaryHostActorsByGroup(Integer(self, 0));
    break;
case 0x163:
{
    int index;
    switch (g_SecondaryActorDifficulty)
    {
    case 0: index = 1; break;
    case 1: index = 2; break;
    case 2: index = 3; break;
    case 3: case 4: index = 4; break;
    default: goto secondary_host_control_done;
    }
    int value = Integer(self, index);
    int *destination = SecondaryHostIntegerDestination(self);
    *destination = value;
    break;
}
case 0x164:
{
    int index;
    switch (g_SecondaryActorDifficulty)
    {
    case 0: index = 1; break;
    case 1: index = 2; break;
    case 2: index = 3; break;
    case 3: case 4: index = 4; break;
    default: goto secondary_host_control_done;
    }
    float value = Real(self, index);
    float *destination = SecondaryFloatDestination(self->owner->current, 0);
    *destination = value;
    break;
}
case 0x190:
{
    int index = Integer(self, 0);
    SecondaryHostRecord *item = Record(self, index);
    memset(item, 0, sizeof(*item));
    item->angle = 0.0f;
    item->count1 = 1;
    item->count0 = 1;
    item->speed = 2.0f;
    item->value204 = 7;
    item->value208 = 0x18;
    item->flags = 0x83;
    RecordLocal(self, index).x = 0.0f;
    RecordLocal(self, index).y = 0.0f;
    RecordOffset(self, index).x = 0.0f;
    RecordOffset(self, index).y = 0.0f;
    RecordOffset(self, index).z = 0.0f;
    break;
}
case 0x19b:
{
    int source = Integer(self, 1);
    int destination = Integer(self, 0);
    *Record(self, destination) = *Record(self, source);
    break;
}
case 0x191:
{
    int index = Integer(self, 0);
    SecondaryHostRecord *item = Record(self, index);
    StoreRecordPosition(item->position, self, index);
    ProjectileScale() = *reinterpret_cast<float *>(&self->unknown1590);
    EmitSecondaryHostRecord(item);
    ProjectileScale() = 0.0f;
    break;
}
case 0x192:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->type = static_cast<short>(Integer(self, 1));
    item->subtype = static_cast<short>(Integer(self, 2));
    break;
}
case 0x193:
{
    int index = Integer(self, 0);
    RecordLocal(self, index).x = Real(self, 1);
    RecordLocal(self, index).y = Real(self, 2);
    break;
}
case 0x1b5:
{
    int index = Integer(self, 0);
    float angle = Real(self, 1);
    float magnitude = Real(self, 2);
    SecondaryVector2 position;
    SecondaryHostPolarVector(&position, angle, magnitude);
    RecordLocal(self, index).x = position.x;
    RecordLocal(self, index).y = position.y;
    break;
}
case 0x1b6:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->value20 = Real(self, 1);
    break;
}
case 0x1b7:
{
    int index = Integer(self, 0);
    RecordOffset(self, index).x = Real(self, 1);
    RecordOffset(self, index).y = Real(self, 2);
    RecordOffset(self, index).z = !(RecordOffset(self, index).x < -990.0) ? 1.0f : 0.0f;
    break;
}
case 0x194:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->angle = Real(self, 1);
    item->angle2 = Real(self, 2);
    break;
}
case 0x195:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->speed = Real(self, 1);
    item->speed2 = Real(self, 2);
    break;
}
case 0x196:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->count0 = static_cast<short>(Integer(self, 1));
    item->count1 = static_cast<short>(Integer(self, 2));
    break;
}
case 0x1b3:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->speed = Real(self, DifficultyArgument(1));
    item->speed2 = Real(self, DifficultyArgument(5));
    break;
}
case 0x1b4:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->count0 = static_cast<short>(Integer(self, DifficultyArgument(1)));
    item->count1 = static_cast<short>(Integer(self, DifficultyArgument(5)));
    break;
}
case 0x1a6:
{
    int index = Integer(self, 0);
    if (g_SecondaryHostSelectionValue >= 512)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 5);
        item->speed2 = Real(self, 6);
    }
    else if (g_SecondaryHostSelectionValue >= -512)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 3);
        item->speed2 = Real(self, 4);
    }
    else
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 1);
        item->speed2 = Real(self, 2);
    }
    break;
}
case 0x1a7:
{
    int index = ReadSecondaryHostInteger(self, 0);
    if (g_SecondaryHostSelectionValue >= 600)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 9);
        item->speed2 = Real(self, 10);
    }
    else if (g_SecondaryHostSelectionValue >= 200)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 7);
        item->speed2 = Real(self, 8);
    }
    else if (g_SecondaryHostSelectionValue >= -200)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 5);
        item->speed2 = Real(self, 6);
    }
    else if (g_SecondaryHostSelectionValue >= -600)
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 3);
        item->speed2 = Real(self, 4);
    }
    else
    {
        SecondaryHostRecord *item = Record(self, index);
        item->speed = Real(self, 1);
        item->speed2 = Real(self, 2);
    }
    break;
}
case 0x1a8:
{
    int index = ReadSecondaryHostInteger(self, 0);
    float lower0 = Real(self, 1);
    float lower1 = Real(self, 2);
    float upper0 = Real(self, 3);
    float upper1 = Real(self, 4);
    SecondaryHostRecord *item = Record(self, index);
    item->speed = static_cast<float>((static_cast<double>(upper0) - lower0) *
        (static_cast<double>(g_SecondaryHostSelectionValue) + 1024.0) * (1.0 / 2048.0) + lower0);
    item->speed2 = static_cast<float>((static_cast<double>(upper1) - lower1) *
        (static_cast<double>(g_SecondaryHostSelectionValue) + 1024.0) * (1.0 / 2048.0) + lower1);
    break;
}
case 0x1a9:
    record = Record(self, ReadSecondaryHostInteger(self, 0));
    if (g_SecondaryHostSelectionValue >= 512)
    {
        record->count0 = static_cast<short>(ReadSecondaryHostInteger(self, 5));
        record->count1 = static_cast<short>(ReadSecondaryHostInteger(self, 6));
        break;
    }
    if (g_SecondaryHostSelectionValue < -512) goto secondary_host_record_count_low;
    goto secondary_host_record_count_lower;
case 0x1aa:
    record = Record(self, ReadSecondaryHostInteger(self, 0));
    if (g_SecondaryHostSelectionValue >= 600)
    {
        firstCount = static_cast<short>(ReadSecondaryHostInteger(self, 9));
        secondIndex = 10;
    }
    else if (g_SecondaryHostSelectionValue >= 200)
    {
        firstCount = static_cast<short>(ReadSecondaryHostInteger(self, 7));
        secondIndex = 8;
    }
    else if (g_SecondaryHostSelectionValue >= -200)
    {
        firstCount = static_cast<short>(ReadSecondaryHostInteger(self, 5));
        secondIndex = 6;
    }
    else if (g_SecondaryHostSelectionValue < -600) goto secondary_host_record_count_low;
    else goto secondary_host_record_count_lower;
    goto secondary_host_record_count_store;
secondary_host_record_count_low:
    record->count0 = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    record->count1 = static_cast<short>(ReadSecondaryHostInteger(self, 2));
    break;
secondary_host_record_count_lower:
    firstCount = static_cast<short>(ReadSecondaryHostInteger(self, 3));
    secondIndex = 4;
secondary_host_record_count_store:
    record->count0 = firstCount;
    record->count1 = static_cast<short>(ReadSecondaryHostInteger(self, secondIndex));
    break;
case 0x1ab:
{
    int index = ReadSecondaryHostInteger(self, 0);
    int lower0 = ReadSecondaryHostInteger(self, 1);
    int lower1 = ReadSecondaryHostInteger(self, 2);
    int upper0 = ReadSecondaryHostInteger(self, 3);
    int upper1 = ReadSecondaryHostInteger(self, 4);
    SecondaryHostRecord *item = Record(self, index);
    item->count0 = static_cast<short>(InterpolatedInteger(lower0, upper0));
    item->count1 = static_cast<short>(InterpolatedInteger(lower1, upper1));
    break;
}
case 0x197:
{
    SecondaryHostRecord *item = Record(self, ReadSecondaryHostInteger(self, 0));
    item->kind = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    break;
}
case 0x198:
{
    SecondaryHostRecord *item = Record(self, ReadSecondaryHostInteger(self, 0));
    item->value204 = ReadSecondaryHostInteger(self, 1);
    item->value208 = ReadSecondaryHostInteger(self, 2);
    break;
}
case 0x199:
{
    int index = ReadSecondaryHostInteger(self, 0);
    int slot = ReadSecondaryHostInteger(self, 1);
    SecondaryHostPattern *pattern = &Record(self, index)->patterns.values[slot];
    pattern->value5 = ReadSecondaryHostInteger(self, 2);
    pattern->value4 = ReadSecondaryHostInteger(self, 3);
    pattern->value2 = ReadSecondaryHostInteger(self, 4);
    pattern->value3 = ReadSecondaryHostInteger(self, 5);
    pattern->real0 = Real(self, 6);
    pattern->real1 = Real(self, 7);
    break;
}
case 0x14e:
{
    int limit = ReadSecondaryHostInteger(self, 2);
    int life = ReadSecondaryHostInteger(self, 1);
    int slot = ReadSecondaryHostInteger(self, 0);
    SecondaryActorManagerView *owner = self->owner;
    owner->state.slots[slot].lifeThreshold = life;
    if (life >= 0)
        SetSecondaryHostSlot(owner, slot, reinterpret_cast<const char *>(instruction) + 0x20, limit);
    break;
}
case 0x155:
{
    int slot = ReadSecondaryHostInteger(self, 0);
    self->owner->state.slots[slot].frameScript = reinterpret_cast<const char *>(instruction) + 0x18;
    break;
}
case 0x19a:
    SetSecondaryHostStageMode(1);
    ClearSecondaryObjects(1, 0);
    break;
case 0x150:
{
    float x = self->world.position.x;
    int sound = ReadSecondaryHostInteger(self, 0);
    QueueSecondaryActorSound(sound, x);
    break;
}
case 0x151:
{
    int value2 = ReadSecondaryHostInteger(self, 2);
    int value1 = ReadSecondaryHostInteger(self, 1);
    int value0 = ReadSecondaryHostInteger(self, 0);
    CreateSecondaryHostScreenEffect(1, value0, value1, value2, 0x43);
    break;
}
case 0x152:
    CreateSecondaryHostTransition(ReadSecondaryHostInteger(self, 0));
    SetSecondaryHostStageMode(0);
    ClearSecondaryObjects(0, 0);
    // Native fallthrough into the same actor-clear call as 159.
case 0x159:
    ClearSecondaryHostActors();
    break;
case 0x153:
    if (ControlHud()->dialogue && SecondaryHostDialogueStatus() == 0) return -1;
    break;
case 0x154:
    if (g_SecondaryMaster->Slot(0)) return -1;
    break;
case 0x156:
case 0x15c:
case 0x165:
case 0x166:
case 0x167:
{
    // Capacity hypothesis: native storage starts 132 bytes below its GS cookie.
    // Original array type/capacity is unproved; no bounds or terminator are added.
    unsigned char decodedName[128];
    int length = reinterpret_cast<const SecondaryValue *>(instruction)[7].integer;
    const unsigned char *encoded = reinterpret_cast<const unsigned char *>(instruction) + 0x20;
    unsigned char key = 0x77, step = 7;
    for (int i = 0; i < length; ++i)
    {
        decodedName[i] = encoded[i] ^ key;
        key = static_cast<unsigned char>(key + step);
        step = static_cast<unsigned char>(step + 0x10);
    }
    int id = Integer(self, 0);
    switch (instruction->opcode)
    {
    case 0x165: id = AddInteger(id, g_SecondaryActorDifficulty); break;
    case 0x166: id = AddInteger(AddInteger(id, g_SecondaryActorDifficulty), -1); break;
    case 0x167: id = AddInteger(AddInteger(id, g_SecondaryActorDifficulty), -2); break;
    }
    Integer(self, 2);
    int value = Integer(self, 1);
    InitializeSecondarySpellStage(reinterpret_cast<const char *>(decodedName), id, value);
    self->counter.flags |= 1;
    self->counter.scaled = ScaleCounter(self->counter.value);
    goto secondary_host_reset_frame;
}
case 0x14d:
secondary_host_reset_frame:
    SetStageTimer(&self->frame, 0);
    break;
case 0x157:
    FinishSecondaryHostSpell();
    self->counter.flags &= ~1u;
    break;
case 0x16a:
    SecondaryActorSpell()->flags |= 8;
    break;
case 0x16b:
    ReleaseSecondaryHostSpellAnimation();
    break;
case 0x16c:
    SetFlag(self, ReadSecondaryHostInteger(self, 0), 0x800000);
    break;
case 0x19c:
case 0x1af:
{
    SecondaryHostBasicPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (opcode == 0x1af) packet.patterns = Record(self, 0)->patterns;
    StoreRecordPosition(packet.position, self, 0);
    packet.type = static_cast<short>(ReadSecondaryHostInteger(self, 0));
    packet.subtype = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    packet.angle = NormalizeAngle(Real(self, 2));
    packet.value20 = Real(self, 3);
    packet.speed0 = Real(self, 4);
    packet.speed1 = Real(self, 5);
    packet.angle1 = Real(self, 6);
    packet.value1c = Real(self, 7);
    packet.value1dc = Record(self, 0)->value204;
    packet.value1e0 = Record(self, 0)->value208;
    packet.flags |= 1;
    CreateSecondaryHostObject(&packet, 0);
    break;
}
case 0x19d:
case 0x1b0:
{
    SecondaryHostAdvancedPacket packet;
    ResetSecondaryHostAdvancedPacket(&packet);
    int *destination = 0;
    if (opcode == 0x1b0) packet.patterns = Record(self, 0)->patterns;
    StoreRecordPosition(packet.position, self, 0);
    packet.type = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    packet.subtype = static_cast<short>(ReadSecondaryHostInteger(self, 2));
    packet.angle = NormalizeAngle(Real(self, 3));
    packet.speed1 = Real(self, 4);
    packet.speed0 = Real(self, 5);
    packet.value30 = ReadSecondaryHostInteger(self, 6);
    packet.value34 = ReadSecondaryHostInteger(self, 7);
    packet.value38 = ReadSecondaryHostInteger(self, 8);
    packet.value3c = ReadSecondaryHostInteger(self, 9);
    packet.value28 = Real(self, 10);
    unsigned int flags = ReadSecondaryHostInteger(self, 11);
    packet.value40 = Record(self, 0)->value204;
    packet.value44 = Record(self, 0)->value208;
    packet.flags = flags | 2;
    if (packet.flags & 4) packet.value48 = ReadSecondaryHostInteger(self, 0);
    else destination = SecondaryHostIntegerDestination(self);
    int id = CreateSecondaryHostObject(&packet, 1);
    if (destination) *destination = id;
    break;
}
case 0x1ac:
case 0x1b1:
{
    SecondaryHostBasicPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (opcode == 0x1b1) packet.patterns = Record(self, 0)->patterns;
    StoreRecordPosition(packet.position, self, 0);
    packet.type = static_cast<short>(ReadSecondaryHostInteger(self, 0));
    packet.subtype = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    packet.angle = Real(self, 2);
    packet.value20 = Real(self, 3);
    packet.speed0 = Real(self, 4);
    packet.speed1 = Real(self, 5);
    packet.angle1 = Real(self, 6);
    packet.value1c = Real(self, 7);
    packet.value1dc = Record(self, 0)->value204;
    packet.value1e0 = Record(self, 0)->value208;
    packet.flags &= ~1u;
    CreateSecondaryHostObject(&packet, 0);
    break;
}
case 0x1ad:
case 0x1b2:
{
    SecondaryHostAdvancedPacket packet;
    ResetSecondaryHostAdvancedPacket(&packet);
    int *destination = SecondaryHostIntegerDestination(self);
    if (instruction->opcode == 0x1b2) packet.patterns = Record(self, 0)->patterns;
    StoreRecordPosition(packet.position, self, 0);
    packet.type = static_cast<short>(ReadSecondaryHostInteger(self, 1));
    packet.subtype = static_cast<short>(ReadSecondaryHostInteger(self, 2));
    packet.angle = Real(self, 3);
    packet.speed1 = Real(self, 4);
    packet.speed0 = Real(self, 5);
    packet.value30 = ReadSecondaryHostInteger(self, 6);
    packet.value34 = ReadSecondaryHostInteger(self, 7);
    packet.value38 = ReadSecondaryHostInteger(self, 8);
    packet.value3c = ReadSecondaryHostInteger(self, 9);
    packet.value28 = Real(self, 10);
    unsigned int flags = ReadSecondaryHostInteger(self, 11);
    // These two fields differ by one word from the normalized 19D/1B0 packet.
    packet.value44 = Record(self, 0)->value204;
    packet.value48 = Record(self, 0)->value208;
    packet.flags = flags & ~2u;
    int id = CreateSecondaryHostObject(&packet, 1);
    if (destination) *destination = id;
    break;
}
case 0x1c0:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    while (object)
    {
        object->Clear(1, 0);
        object->id = 0;
        object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    }
    break;
}
case 0x19e:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object)
    {
        float y = Real(self, 2);
        object->position.x = Real(self, 1);
        object->position.y = y;
        object->position.z = 0.0f;
    }
    break;
}
case 0x19f:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object)
    {
        float y = Real(self, 2);
        CameraVector position;
        position.x = Real(self, 1);
        position.y = y;
        position.z = 0.0f;
        SetSecondaryHostObjectTarget(object, &position);
    }
    break;
}
case 0x1a0:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object) object->value060 = Real(self, 1);
    break;
}
case 0x1a1:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object) object->value05c = Real(self, 1);
    break;
}
case 0x1a2:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object) object->value054 = Real(self, 1);
    break;
}
case 0x1a3:
{
    SecondaryHostObjectView *object = FindSecondaryHostObject(ReadSecondaryHostInteger(self, 0));
    if (object) object->value45c = Real(self, 1);
    break;
}
case 0x1a4:
{
    float radius = Real(self, 0);
    const CameraVector *position = &self->world.position;
    ClearSceneProjectilesInRadius(position, radius, 1, 0);
    ClearSecondaryObjectsInRadius(position, radius, 1, 1);
    break;
}
case 0x1a5:
{
    float radius = Real(self, 0);
    const CameraVector *position = &self->world.position;
    ClearSceneProjectilesInRadius(position, radius, 0, 0);
    ClearSecondaryObjectsInRadius(position, radius, 0, 1);
    break;
}
case 0x1be:
{
    float radius = Real(self, 0);
    const CameraVector *position = &self->world.position;
    ClearSceneProjectilesInRadius(position, radius, 1, 1);
    ClearSecondaryObjectsInRadius(position, radius, 1, 1);
    break;
}
case 0x1bf:
{
    float radius = ReadSecondaryHostReal(self, 0);
    const CameraVector *position = &self->world.position;
    ClearSceneProjectilesInRadius(position, radius, 0, 1);
    ClearSecondaryObjectsInRadius(position, radius, 0, 1);
    break;
}
case 0x158:
    SetSecondaryHostSelection(&g_SecondaryHostSelectionState, ReadSecondaryHostInteger(self, 0));
    break;
case 0x15d:
    if (g_SecondaryHostSelectionValue >= 512)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 2);
    }
    else if (g_SecondaryHostSelectionValue >= -512)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 0);
    }
    else
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 0);
    }
    break;
case 0x15e:
    if (g_SecondaryHostSelectionValue >= 600)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 4);
    }
    else if (g_SecondaryHostSelectionValue >= 200)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 3);
    }
    else if (g_SecondaryHostSelectionValue >= -200)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 2);
    }
    else if (g_SecondaryHostSelectionValue >= -400)
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 1);
    }
    else
    {
        float *destination = SecondaryHostFloatDestination(self);
        *destination = ReadSecondaryHostReal(self, 0);
    }
    break;
case 0x15f:
{
    float lower = ReadSecondaryHostReal(self, 1);
    float upper = ReadSecondaryHostReal(self, 2);
    float result = static_cast<float>((static_cast<double>(upper) - lower) *
        (static_cast<double>(g_SecondaryHostSelectionValue) + 1024.0) * (1.0 / 2048.0) + lower);
    float *destination = SecondaryHostFloatDestination(self);
    *destination = result;
    break;
}
case 0x160:
    if (g_SecondaryHostSelectionValue >= 512)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 2);
    }
    else if (g_SecondaryHostSelectionValue >= -512)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 0);
    }
    else
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 0);
    }
    break;
case 0x161:
    if (g_SecondaryHostSelectionValue >= 600)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 4);
    }
    else if (g_SecondaryHostSelectionValue >= 200)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 3);
    }
    else if (g_SecondaryHostSelectionValue >= -200)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 2);
    }
    else if (g_SecondaryHostSelectionValue >= -400)
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 1);
    }
    else
    {
        int *destination = SecondaryHostIntegerDestination(self);
        *destination = ReadSecondaryHostInteger(self, 0);
    }
    break;
case 0x162:
{
    int lower = ReadSecondaryHostInteger(self, 1);
    int upper = ReadSecondaryHostInteger(self, 2);
    int product = static_cast<int>((static_cast<unsigned int>(upper) - lower) *
        (static_cast<unsigned int>(g_SecondaryHostSelectionValue) + 1024u));
    int result = AddInteger(product / 2048, lower);
    int *destination = SecondaryHostIntegerDestination(self);
    *destination = result;
    break;
}
case 0x168:
    ControlHud()->value43fc = ReadSecondaryHostInteger(self, 0);
    break;
case 0x16d:
    MarkSecondaryHostEffects();
    break;
case 0x16e:
{
    SetFlag(self, ReadSecondaryHostInteger(self, 0), 0x1000000);
    int script = ReadSecondaryHostInteger(self, 1);
    self->flags &= ~0x2000000u;
    self->script1584 = script;
    self->script1588 = self->scriptBase;
    break;
}
case 0x16f:
    g_AnimationTimeScale = ReadSecondaryHostReal(self, 0);
    break;
case 0x1ae:
{
    double x = g_SecondaryActorReferences->position.x;
    double y = g_SecondaryActorReferences->position.y;
    float deltaX = static_cast<float>(x - ReadSecondaryHostReal(self, 1));
    float deltaY = static_cast<float>(y - ReadSecondaryHostReal(self, 2));
    float angle = SecondaryAngleMath(deltaY, deltaX);
    float *destination = SecondaryHostFloatDestination(self);
    *destination = angle;
    break;
}
case 0x170:
{
    float amount;
    if (g_SecondaryActorDifficulty == 0) amount = static_cast<float>(ReadSecondaryHostInteger(self, 0));
    else if (g_SecondaryActorDifficulty == 1) amount = static_cast<float>(ReadSecondaryHostInteger(self, 1));
    else if (g_SecondaryActorDifficulty == 2) amount = static_cast<float>(ReadSecondaryHostInteger(self, 2));
    else amount = static_cast<float>(ReadSecondaryHostInteger(self, 3));
    self->owner->current->time -= amount;
    break;
}
case 0x1b8:
{
    if (self->mesh) DestroySecondaryHostMesh(self->mesh);
    self->mesh = 0;
    self->meshLimit = ReadSecondaryHostReal(self, 0);
    self->meshProgress = 16.0f;
    self->meshColor = ReadSecondaryHostInteger(self, 1);
    float angle = NormalizeAngle(0.0f);
    self->meshPhaseX = angle;
    self->meshPhaseY = angle;
    if (self->meshLimit > 0.0f) self->mesh = new StageScriptMesh(0x11, 0x11, 0);
    break;
}
case 0x1b9:
    SelectSecondaryHostStageLabel(ReadSecondaryHostInteger(self, 0));
    break;
case 0x171:
    SetFlag(self, ReadSecondaryHostInteger(self, 0), 0x8000000);
    break;
case 0x172:
    self->unknown11c = ReadSecondaryHostInteger(self, 0);
    break;
