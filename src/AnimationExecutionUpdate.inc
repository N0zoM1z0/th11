    if (vm->angularVelocity.x != 0.0f)
    {
        vm->rotation.x = AddAngles(vm->rotation.x, g_AnimationTimeScale * vm->angularVelocity.x);
        vm->flags.value |= 4;
    }
    if (vm->angularVelocity.y != 0.0f)
    {
        vm->rotation.y = AddAngles(vm->rotation.y, vm->angularVelocity.y * g_AnimationTimeScale);
        vm->flags.value |= 4;
    }
    if (vm->angularVelocity.z != 0.0f)
    {
        vm->rotation.z = AddAngles(vm->rotation.z, g_AnimationTimeScale * vm->angularVelocity.z);
        vm->flags.value |= 4;
    }
    if (vm->scaleVelocity.y != 0.0f)
    {
        vm->flags.value |= 8;
        vm->scale.y = g_AnimationTimeScale * vm->scaleVelocity.y + vm->scale.y;
    }
    if (vm->scaleVelocity.x != 0.0f)
    {
        vm->flags.value |= 0xc;
        vm->scale.x = vm->scaleVelocity.x * g_AnimationTimeScale + vm->scale.x;
    }
    vm->uOffset = g_AnimationTimeScale * vm->uVelocity + vm->uOffset;
    if (vm->uOffset >= 1.0) vm->uOffset = static_cast<float>(vm->uOffset - 1.0);
    else if (vm->uOffset < 0.0f) vm->uOffset = static_cast<float>(vm->uOffset + 1.0);
    vm->vOffset = vm->vVelocity * g_AnimationTimeScale + vm->vOffset;
    if (vm->vOffset >= 1.0) vm->vOffset = static_cast<float>(vm->vOffset - 1.0);
    else if (vm->vOffset < 0.0f) vm->vOffset = static_cast<float>(vm->vOffset + 1.0);
    if (vm->flags.value & 0x2000)
    {
        const CameraVector *delta = reinterpret_cast<CameraVector *>(
            reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x524);
        vm->translation = Sum(vm->translation, *delta);
    }
    if (vm->flags2 & 4)
    {
        CameraVector quad[4];
        switch ((vm->flags.value >> 22) & 15)
        {
        case 0: BuildAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
        case 1: BuildRotatedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
        case 2:
        case 3: BuildUnroundedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
        }
        InlineUv(&vm->uv[0], &quad[0]);
        InlineUv(&vm->uv[1], &quad[1]);
        InlineUv(&vm->uv[2], &quad[2]);
        InlineUv(&vm->uv[3], &quad[3]);
    }
    if (vm->positionTween.duration)
    {
        if ((reinterpret_cast<unsigned char *>(&vm->flags.value)[1] % 2u)) vm->otherPosition = StepStageVectorTween(&vm->positionTween);
        else vm->position = StepStageVectorTween(&vm->positionTween);
    }
    if (vm->colorTween.duration)
    {
        AnimationIntVector color = StepAnimationIntVectorTween(&vm->colorTween);
        Byte(vm, 0x376) = static_cast<unsigned char>(color.z);
        Byte(vm, 0x375) = static_cast<unsigned char>(color.y);
        Byte(vm, 0x374) = static_cast<unsigned char>(color.x);
    }
    if (vm->alphaTween.duration) Byte(vm, 0x377) = static_cast<unsigned char>(StepAnimationIntScalarTween(&vm->alphaTween));
    if (vm->scaleTween.duration)
    {
        vm->scale = StepAnimationFloatPairTween(&vm->scaleTween);
        vm->flags.value |= 8;
    }
    if (vm->rotationTween.duration)
    {
        vm->rotation = StepStageVectorTween(&vm->rotationTween);
        vm->flags.value |= 4;
    }
    if (vm->color2Tween.duration)
    {
        AnimationIntVector color = StepAnimationIntVectorTween(&vm->color2Tween);
        Byte(vm, 0x37a) = static_cast<unsigned char>(color.z);
        Byte(vm, 0x379) = static_cast<unsigned char>(color.y);
        Byte(vm, 0x378) = static_cast<unsigned char>(color.x);
    }
    if (vm->alpha2Tween.duration) Byte(vm, 0x37b) = static_cast<unsigned char>(StepAnimationIntScalarTween(&vm->alpha2Tween));
    if (vm->uVelocityTween.duration) vm->uVelocity = StepAnimationFloatScalarTween(&vm->uVelocityTween);
    if (vm->vVelocityTween.duration) vm->vVelocity = StepAnimationFloatScalarTween(&vm->vVelocityTween);
    unsigned int mode = (vm->flags.value >> 22) & 15;
    if (mode == 9)
    {
        float angle = vm->rotation.z;
        int count = Subtract(vm->integerVariables[0], 1);
        float denominator = static_cast<float>(count);
        float angleStep = static_cast<float>(6.2831854820251465 / denominator);
        float v = 0.0f;
        float vStep = static_cast<float>(vm->integerVariables[1]) / denominator;
        unsigned int color = (vm->flags.value & 0x8000) ? vm->color2 : vm->color;
        RenderVertex *vertex = vm->vertices;
        for (int left = count; left > 0; --left)
        {
            EmitStripPair(vm, vertex, color, angle, v);
            v = vStep + v;
            angle = AddAngles(angle, angleStep);
        }
        vertex[0] = vm->vertices[0];
        vertex[0].v = v + vm->vOffset;
        vertex[1] = vm->vertices[1];
        vertex[1].v = v + vm->vOffset;
    }
    else if (mode == 13)
    {
        int count = vm->integerVariables[0];
        float angle = NormalizeAngle(static_cast<float>(vm->rotation.z - vm->rotation.x * 0.5));
        float denominator = static_cast<float>(Subtract(count, 1));
        float angleStep = vm->rotation.x / denominator;
        float v = 0.0f;
        float vStep = static_cast<float>(vm->integerVariables[1]) / denominator;
        unsigned int color = (vm->flags.value & 0x8000) ? vm->color2 : vm->color;
        RenderVertex *vertex = vm->vertices;
        for (int left = vm->integerVariables[0]; left > 0; --left)
        {
            EmitStripPair(vm, vertex, color, angle, v);
            v = vStep + v;
            angle = AddAngles(angle, angleStep);
        }
    }
