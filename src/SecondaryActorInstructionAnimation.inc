// Creation/animation portion of the complete host switch. The guarded spawn
// entries deliberately fall into their corresponding unguarded entries.
// The spawn entries share one lexical scope and packet. Their lifetime ends
// before the other opcode families, allowing the native scratch-slot reuse.
{
SecondaryHostSpawnArguments arguments;
const SecondaryValue *values;
case 0x109:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x100:
{
    values = SpawnValues(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = self->world.position.x;
    if (!(self->flags & 0x400000))
    {
        arguments.position.x = ReadSecondaryHostRealValue(self, 1, values[0].real) + x;
        float y = self->world.position.y;
        arguments.position.y = ReadSecondaryHostRealValue(self, 2, values[1].real) + y;
    }
    else
    {
        CameraVector position;
        position.x = ReadSecondaryHostRealValue(self, 1, values[0].real) + x;
        float y = self->world.position.y;
        position.y = ReadSecondaryHostRealValue(self, 2, values[1].real) + y;
        position.z = self->world.position.z;
        SelectSecondaryHostCamera(&g_SecondaryHostProjectionRenderer, 0);
        CameraView &camera = g_SecondaryHostProjectionRenderer.camera;
        D3DXVec3Project(&arguments.position, &position, &camera.viewport,
            &camera.projection, &camera.view, &g_SecondaryHostProjectionWorld);
        arguments.position.x = static_cast<float>(arguments.position.x - 224.0);
        arguments.position.y = static_cast<float>(arguments.position.y - 16.0);
        arguments.position.z = 0.0f;
    }
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, values[2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, values[3].integer);
    goto hostSpawnLastValue;
}
case 0x10a:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x101:
{
    values = SpawnValues(instruction);
    memset(&arguments, 0, sizeof(arguments));
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, values[0].real);
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, values[1].real);
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, values[2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, values[3].integer);
}
hostSpawnLastValue:
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, values[4].integer);
    SpawnParameters(self, &arguments);
    SpawnActor(instruction, &arguments);
    break;
case 0x102:
    Selection(self).file = Integer(self, 0);
    break;
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
case 0x10b:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x104:
{
    values = SpawnValues(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = self->world.position.x;
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, values[0].real) + x;
    float y = self->world.position.y;
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, values[1].real) + y;
    goto hostSpawnMirroredValues;
}
case 0x10c:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x105:
{
    values = SpawnValues(instruction);
    memset(&arguments, 0, sizeof(arguments));
    arguments.position.x = ReadSecondaryHostRealValue(self, 1, values[0].real);
    arguments.position.y = ReadSecondaryHostRealValue(self, 2, values[1].real);
}
hostSpawnMirroredValues:
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 3, values[2].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 4, values[3].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 5, values[4].integer);
    SpawnParameters(self, &arguments);
    arguments.mirrored = 1;
    SpawnActor(instruction, &arguments);
    break;
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
case 0x108:
{
    int file = Integer(self, 0);
    int script = Integer(self, 1);
    CreateSecondaryEmitterAnimationVmId(File(file), script, 8);
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
case 0x10f:
    if (g_SecondaryMaster->Slot(0)) break;
case 0x10e:
{
    values = SpawnValues(instruction);
    memset(&arguments, 0, sizeof(arguments));
    float x = ReadSecondaryHostRealValue(self, 1, values[0].real);
    arguments.position.x = x + SceneOffset().x;
    float y = ReadSecondaryHostRealValue(self, 2, values[1].real);
    arguments.position.y = y + SceneOffset().y;
    arguments.position.z = ReadSecondaryHostRealValue(self, 3, values[2].real);
    arguments.value014 = ReadSecondaryHostIntegerValue(self, 4, values[3].integer);
    arguments.value00c = ReadSecondaryHostIntegerValue(self, 5, values[4].integer);
    arguments.value010 = ReadSecondaryHostIntegerValue(self, 6, values[5].integer);
    SpawnParameters(self, &arguments);
    arguments.flags |= 1;
    SpawnActor(instruction, &arguments);
    break;
}
}
case 0x110:
    if (!(self->flags & 0x400000))
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        CreatePositionedAnimationVmId(File(file), &self->world.position, script, 8);
    }
    else
    {
        int script = Integer(self, 1);
        int file = Integer(self, 0);
        CreateDirectPositionAnimationVmId(File(file), &self->world.position, script, 8);
    }
    break;
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
