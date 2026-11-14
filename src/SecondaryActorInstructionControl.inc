// Included in the complete host's instruction switch.
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
case 0x14d:
secondary_host_reset_frame:
    SetStageTimer(&self->frame, 0);
    break;
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
case 0x14f:
    SetStageTimer(&self->timer1558, Integer(self, 0));
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
case 0x155:
{
    int slot = ReadSecondaryHostInteger(self, 0);
    self->owner->state.slots[slot].frameScript = reinterpret_cast<const char *>(instruction) + 0x18;
    break;
}
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
    BeginSecondaryHostSpell(reinterpret_cast<const char *>(decodedName), id, value);
    self->counter.flags |= 1;
    self->counter.scaled = ScaleCounter(self->counter.value);
    goto secondary_host_reset_frame;
}
case 0x157:
    FinishSecondaryHostSpell();
    self->counter.flags &= ~1u;
    break;
case 0x158:
    SetSecondaryHostSelection(&g_SecondaryHostSelectionState, ReadSecondaryHostInteger(self, 0));
    break;
case 0x15a:
{
    double value = Real(self, 0);
    *reinterpret_cast<float *>(&self->unknown1590) = static_cast<float>(value * value);
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
case 0x168:
    ControlHud()->value43fc = ReadSecondaryHostInteger(self, 0);
    break;
case 0x169:
    SetStageTimer(&self->timer156c, Integer(self, 0));
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
case 0x171:
    SetFlag(self, ReadSecondaryHostInteger(self, 0), 0x8000000);
    break;
case 0x172:
    self->unknown11c = ReadSecondaryHostInteger(self, 0);
    break;
case 0x173:
    ClearSecondaryHostActorsByGroup(Integer(self, 0));
    break;
case 0x174:
    self->enabled = Integer(self, 0);
secondary_host_control_done:
    break;
