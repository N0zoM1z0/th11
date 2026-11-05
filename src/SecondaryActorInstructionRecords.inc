// Remaining handled values of the large host: 190..1C2. Shared pair-reader
// temporaries belong to the enclosing function, alongside the other fragments.
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
case 0x191:
{
    int index = Integer(self, 0);
    SecondaryHostRecord *item = Record(self, index);
    item->position = RecordPosition(self, index);
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
case 0x194:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->angle = Real(self, 1);
    item->angle2 = Real(self, 2);
    break;
}
case 0x195:
    recordIndex = Integer(self, 0);
    goto secondary_host_record_real_low;
case 0x196:
{
    SecondaryHostRecord *item = Record(self, Integer(self, 0));
    item->count0 = static_cast<short>(Integer(self, 1));
    item->count1 = static_cast<short>(Integer(self, 2));
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
case 0x19a:
    SetSecondaryHostStageMode(1);
    ClearSecondaryObjects(1, 0);
    break;
case 0x19b:
{
    int source = Integer(self, 1);
    int destination = Integer(self, 0);
    *Record(self, destination) = *Record(self, source);
    break;
}
case 0x19c:
case 0x1af:
{
    SecondaryHostBasicPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (opcode == 0x1af) packet.patterns = Record(self, 0)->patterns;
    packet.position = RecordPosition(self, 0);
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
    packet.position = RecordPosition(self, 0);
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
case 0x1a6:
    recordIndex = Integer(self, 0);
    if (g_SecondaryHostSelectionValue >= 512) goto secondary_host_record_real_middle;
    if (g_SecondaryHostSelectionValue < -512) goto secondary_host_record_real_low;
    goto secondary_host_record_real_lower;
case 0x1a7:
    recordIndex = ReadSecondaryHostInteger(self, 0);
    if (g_SecondaryHostSelectionValue >= 600)
    {
        record = Record(self, recordIndex);
        firstReal = Real(self, 9);
        secondIndex = 10;
    }
    else if (g_SecondaryHostSelectionValue >= 200)
    {
        record = Record(self, recordIndex);
        firstReal = Real(self, 7);
        secondIndex = 8;
    }
    else if (g_SecondaryHostSelectionValue >= -200) goto secondary_host_record_real_middle;
    else if (g_SecondaryHostSelectionValue < -600) goto secondary_host_record_real_low;
    else goto secondary_host_record_real_lower;
    goto secondary_host_record_real_store;
secondary_host_record_real_low:
    record = Record(self, recordIndex);
    firstReal = Real(self, 1);
    secondIndex = 2;
    goto secondary_host_record_real_store;
secondary_host_record_real_lower:
    record = Record(self, recordIndex);
    firstReal = Real(self, 3);
    secondIndex = 4;
    goto secondary_host_record_real_store;
secondary_host_record_real_middle:
    record = Record(self, recordIndex);
    firstReal = Real(self, 5);
    secondIndex = 6;
secondary_host_record_real_store:
    record->speed = firstReal;
    record->speed2 = Real(self, secondIndex);
    break;
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
case 0x1ac:
case 0x1b1:
{
    SecondaryHostBasicPacket packet;
    memset(&packet, 0, sizeof(packet));
    if (opcode == 0x1b1) packet.patterns = Record(self, 0)->patterns;
    packet.position = RecordPosition(self, 0);
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
    packet.position = RecordPosition(self, 0);
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
case 0x1ba:
{
    int value = Integer(self, 0);
    MasterFlags() = (MasterFlags() & ~1u) | (value & 1);
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
case 0x1c1:
    g_SecondaryHostCallbacks[Integer(self, 0)](self);
    break;
case 0x1c2:
{
    int score = Integer(self, 0);
    AddSecondaryActorScore(score);
    ShowSecondaryHostScore(score, &self->world.position, -1);
    break;
}
