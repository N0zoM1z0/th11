    if (self->angularVelocity.x != 0.0f)
    {
        self->rotation.x = AddAngles(self->rotation.x, g_AnimationTimeScale * self->angularVelocity.x);
        self->flags |= 4;
    }
    if (self->angularVelocity.y != 0.0f)
    {
        self->rotation.y = AddAngles(self->rotation.y, self->angularVelocity.y * g_AnimationTimeScale);
        self->flags |= 4;
    }
    if (self->angularVelocity.z != 0.0f)
    {
        self->rotation.z = AddAngles(self->rotation.z, g_AnimationTimeScale * self->angularVelocity.z);
        self->flags |= 4;
    }
    if (self->scaleVelocity.y != 0.0f)
    {
        self->flags |= 8;
        self->scale.y = g_AnimationTimeScale * self->scaleVelocity.y + self->scale.y;
    }
    if (self->scaleVelocity.x != 0.0f)
    {
        self->flags |= 0xc;
        self->scale.x = self->scaleVelocity.x * g_AnimationTimeScale + self->scale.x;
    }
    self->uOffset = g_AnimationTimeScale * self->uVelocity + self->uOffset;
    if (self->uOffset >= 1.0) self->uOffset = static_cast<float>(self->uOffset - 1.0);
    else if (self->uOffset < 0.0f) self->uOffset = static_cast<float>(self->uOffset + 1.0);
    self->vOffset = self->vVelocity * g_AnimationTimeScale + self->vOffset;
    if (self->vOffset >= 1.0) self->vOffset = static_cast<float>(self->vOffset - 1.0);
    else if (self->vOffset < 0.0f) self->vOffset = static_cast<float>(self->vOffset + 1.0);
    if (self->flags & 0x2000)
    {
        const CameraVector *delta = reinterpret_cast<CameraVector *>(
            reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x524);
        self->translation = Sum(self->translation, *delta);
    }
    if (self->flags2 & 4)
    {
        CameraVector quad[4];
        BuildAnimationVmModeQuad(vm, quad);
        InlineUv(&self->uv[0], &quad[0]);
        InlineUv(&self->uv[1], &quad[1]);
        InlineUv(&self->uv[2], &quad[2]);
        InlineUv(&self->uv[3], &quad[3]);
    }
    if (self->positionTween.duration)
    {
        if (self->flags & 0x100) self->otherPosition = StepStageVectorTween(&self->positionTween);
        else self->position = StepStageVectorTween(&self->positionTween);
    }
    if (self->colorTween.duration)
    {
        AnimationIntVector color = StepAnimationIntVectorTween(&self->colorTween);
        Byte(vm, 0x376) = static_cast<unsigned char>(color.z);
        Byte(vm, 0x375) = static_cast<unsigned char>(color.y);
        Byte(vm, 0x374) = static_cast<unsigned char>(color.x);
    }
    if (self->alphaTween.duration) Byte(vm, 0x377) = static_cast<unsigned char>(StepAnimationIntScalarTween(&self->alphaTween));
    if (self->scaleTween.duration)
    {
        self->scale = StepAnimationFloatPairTween(&self->scaleTween);
        self->flags |= 8;
    }
    if (self->rotationTween.duration)
    {
        self->rotation = StepStageVectorTween(&self->rotationTween);
        self->flags |= 4;
    }
    if (self->color2Tween.duration)
    {
        AnimationIntVector color = StepAnimationIntVectorTween(&self->color2Tween);
        Byte(vm, 0x37a) = static_cast<unsigned char>(color.z);
        Byte(vm, 0x379) = static_cast<unsigned char>(color.y);
        Byte(vm, 0x378) = static_cast<unsigned char>(color.x);
    }
    if (self->alpha2Tween.duration) Byte(vm, 0x37b) = static_cast<unsigned char>(StepAnimationIntScalarTween(&self->alpha2Tween));
    if (self->uVelocityTween.duration) self->uVelocity = StepAnimationFloatScalarTween(&self->uVelocityTween);
    if (self->vVelocityTween.duration) self->vVelocity = StepAnimationFloatScalarTween(&self->vVelocityTween);
    unsigned int mode = (self->flags >> 22) & 15;
    if (mode == 9)
    {
        float angle = self->rotation.z;
        int count = Subtract(self->integerVariables[0], 1);
        float angleStep = static_cast<float>(6.2831854820251465 / count);
        float v = 0.0f;
        float vStep = static_cast<float>(self->integerVariables[1]) / count;
        unsigned int color = (self->flags & 0x8000) ? self->color2 : self->color;
        RenderVertex *vertex = self->vertices;
        for (int left = count; left > 0; --left)
        {
            PlaceStripPair(self, vertex, color, angle, v);
            vertex += 2;
            v = vStep + v;
            angle = AddAngles(angle, angleStep);
        }
        vertex[0] = self->vertices[0];
        vertex[0].v = v + self->vOffset;
        vertex[1] = self->vertices[1];
        vertex[1].v = v + self->vOffset;
    }
    else if (mode == 13)
    {
        int count = self->integerVariables[0];
        float angle = NormalizeAngle(static_cast<float>(self->rotation.z - self->rotation.x * 0.5));
        float denominator = static_cast<float>(Subtract(count, 1));
        float angleStep = self->rotation.x / denominator;
        float v = 0.0f;
        float vStep = static_cast<float>(self->integerVariables[1]) / denominator;
        unsigned int color = (self->flags & 0x8000) ? self->color2 : self->color;
        RenderVertex *vertex = self->vertices;
        for (int left = self->integerVariables[0]; left > 0; --left)
        {
            PlaceStripPair(self, vertex, color, angle, v);
            vertex += 2;
            v = vStep + v;
            angle = AddAngles(angle, angleStep);
        }
    }
