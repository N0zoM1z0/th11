// Instruction groups follow reviewed native entry order; fallthroughs stay atomic.
        case 4:
            SetStageTimer(&self->time, Words(instruction)[1]);
            self->instruction = reinterpret_cast<AnimationInstruction *>(self->scriptStart + Words(instruction)[0]);
            continue;
        case 5:
        {
            int *value = IntegerDestination(vm, instruction);
            --*value;
            if (ReadI(vm, instruction, 0) > 0)
            {
                SetStageTimer(&self->time, Words(instruction)[2]);
                self->instruction = reinterpret_cast<AnimationInstruction *>(self->scriptStart + Words(instruction)[1]);
                continue;
            }
            break;
        }
        case 69:
            self->flags &= ~1u;
            // Continue through the same event/pause operation as instruction63.
        case 63:
            if (self->event) goto animation_event;
            self->flags |= 0x1000;
            goto animation_pause;
        case 81:
            self->time = self->savedTime;
            self->instruction = self->savedInstruction;
            continue;
// Integer/float arguments and destinations retain their separate read order.
        case 28:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left == right) goto animation_jump;
            break;
        }
        case 29:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left == right) goto animation_jump;
            break;
        }
        case 30:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left != right) goto animation_jump;
            break;
        }
        case 31:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left != right) goto animation_jump;
            break;
        }
        case 32:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left < right) goto animation_jump;
            break;
        }
        case 33:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left < right) goto animation_jump;
            break;
        }
        case 34:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left <= right) goto animation_jump;
            break;
        }
        case 35:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left <= right) goto animation_jump;
            break;
        }
        case 36:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left > right) goto animation_jump;
            break;
        }
        case 37:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left > right) goto animation_jump;
            break;
        }
        case 38:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
            if (left >= right) goto animation_jump;
            break;
        }
        case 39:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left >= right) goto animation_jump;
            break;
        }
        case -1:
        case 1:
            self->flags &= ~1u;
        case 2:
            self->instruction = 0;
            g_AnimationTimeScale = savedScale;
            return 1;
        case 86: ReplaceBits(self->flags, static_cast<unsigned int>(ReadI(vm, instruction, 0)) << 29, 0x20000000); break;
        case 88:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            InsertAnimationRelatedNode(&vm->drawNode, &child->drawNode);
            State(child)->otherPosition = self->position;
            State(child)->translation = self->translation;
            break;
        }
        case 96:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            InsertAnimationRelatedNode(&vm->drawNode, &child->drawNode);
            State(child)->otherPosition.x = ReadF(vm, instruction, 1);
            State(child)->otherPosition.y = ReadF(vm, instruction, 2);
            State(child)->translation = self->translation;
            break;
        }
        case 97:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            State(child)->otherPosition.x = ReadF(vm, instruction, 1);
            State(child)->otherPosition.y = ReadF(vm, instruction, 2);
            State(child)->translation = self->translation;
            break;
        }
        case 95:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            State(child)->otherPosition = self->position;
            State(child)->translation = self->translation;
            break;
        }
        case 91:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateSecondaryEmitterAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            InsertAnimationRelatedNode(&vm->drawNode, &child->drawNode);
            State(child)->otherPosition = self->position;
            State(child)->translation = self->translation;
            break;
        }
        case 90:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateSecondaryDrawAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            InsertAnimationRelatedNode(&vm->drawNode, &child->drawNode);
            State(child)->otherPosition = self->position;
            State(child)->translation = self->translation;
            break;
        }
        case 92:
        {
            short script = static_cast<short>(ReadI(vm, instruction, 0));
            AnimationVmId id = CreateFrontSecondaryDrawAnimationVmId(self->file, script, vm->persistent020);
            AnimationVmView *child = ClearMissingAnimationId(&id.value);
            InsertAnimationRelatedNode(&vm->drawNode, &child->drawNode);
            State(child)->otherPosition = self->position;
            State(child)->translation = self->translation;
            break;
        }
        case 3:
        {
            self->flags |= 1;
            int sprite;
            if (self->spriteTransform) sprite = self->spriteTransform(vm, ReadI(vm, instruction, 0));
            else sprite = ReadI(vm, instruction, 0);
            BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(self->file), sprite, vm);
            self->spriteFrame = self->time.current;
            break;
        }
        case 102:
        {
            self->flags |= 1;
            int sprite;
            if (self->spriteTransform)
            {
                int first = ReadI(vm, instruction, 0);
                unsigned int count = ReadI(vm, instruction, 1);
                int value = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
                sprite = self->spriteTransform(vm, value);
            }
            else
            {
                int first = ReadI(vm, instruction, 0);
                unsigned int count = ReadI(vm, instruction, 1);
                sprite = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
            }
            BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(self->file), sprite, vm);
            self->spriteFrame = self->time.current;
            break;
        }
        case 50:
            self->scale.x = ReadF(vm, instruction, 0);
            self->scale.y = ReadF(vm, instruction, 1);
            self->flags |= 8;
            break;
        case 51: Byte(vm, 0x377) = static_cast<unsigned char>(ReadI(vm, instruction, 0)); break;
        case 52:
            Byte(vm, 0x376) = static_cast<unsigned char>(ReadI(vm, instruction, 0));
            Byte(vm, 0x375) = static_cast<unsigned char>(ReadI(vm, instruction, 1));
            Byte(vm, 0x374) = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            break;
        case 77: Byte(vm, 0x37b) = static_cast<unsigned char>(ReadI(vm, instruction, 0)); break;
        case 76:
            Byte(vm, 0x37a) = static_cast<unsigned char>(ReadI(vm, instruction, 0));
            Byte(vm, 0x379) = static_cast<unsigned char>(ReadI(vm, instruction, 1));
            Byte(vm, 0x378) = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            break;
        case 61:
            self->flags = (self->flags ^ 0x200) | 8;
            self->scale.x = static_cast<float>(self->scale.x * -1.0);
            break;
        case 62:
            self->flags = (self->flags ^ 0x400) | 8;
            self->scale.y = static_cast<float>(self->scale.y * -1.0);
            break;
        case 49:
            self->rotation.x = ReadF(vm, instruction, 0);
            self->rotation.y = ReadF(vm, instruction, 1);
            self->rotation.z = ReadF(vm, instruction, 2);
            self->flags |= 4;
            break;
        case 53:
            self->angularVelocity.x = ReadF(vm, instruction, 0);
            self->angularVelocity.y = ReadF(vm, instruction, 1);
            self->angularVelocity.z = ReadF(vm, instruction, 2);
            self->flags |= 4;
            break;
        case 54:
            self->scaleVelocity.x = ReadF(vm, instruction, 0);
            self->scaleVelocity.y = ReadF(vm, instruction, 1);
            break;
        case 55:
        {
            int duration = ReadI(vm, instruction, 1);
            SetAnimationAlphaTween(vm, duration, 0, Byte(vm, 0x377), static_cast<unsigned char>(Words(instruction)[0]));
            break;
        }
        case 66: ReplaceBits(self->flags, static_cast<unsigned int>(Words(instruction)[0]) << 4, 0x70); break;
        case 48:
            if (self->flags & 0x100)
            {
                float z = ReadF(vm, instruction, 2);
                float y = ReadF(vm, instruction, 1);
                CameraVector value = {ReadF(vm, instruction, 0), y, z};
                self->otherPosition = value;
            }
            else
            {
                float z = ReadF(vm, instruction, 2);
                float y = ReadF(vm, instruction, 1);
                CameraVector value = {ReadF(vm, instruction, 0), y, z};
                self->position = value;
            }
            break;
        case 75: RewindSecondaryTimer(&self->time, ReadI(vm, instruction, 0)); break;
        case 72: ReplaceBits(self->flags, Words(instruction)[0], 1); break;
        case 65:
        {
            unsigned short *values = reinterpret_cast<unsigned short *>(instruction + 1);
            ReplaceBits(self->flags, static_cast<unsigned int>(values[0]) << 18, 0xc0000);
            ReplaceBits(self->flags, static_cast<unsigned int>(values[1]) << 20, 0x300000);
            break;
        }
        case 70: self->uVelocity = ReadF(vm, instruction, 0); break;
        case 71: self->vVelocity = ReadF(vm, instruction, 0); break;
        case 73: ReplaceBits(self->flags, static_cast<unsigned int>(Words(instruction)[0]) << 11, 0x800); break;
        case 74: ReplaceBits(self->flags, static_cast<unsigned int>(Words(instruction)[0]) << 13, 0x2000); break;
        case 89: ReplaceBits(self->flags, static_cast<unsigned int>(Words(instruction)[0]) << 31, 0x80000000); break;
        case 56:
        {
            self->positionTween.duration = ReadI(vm, instruction, 0);
            ZeroControls(self->positionTween);
            self->positionTween.mode = Words(instruction)[1];
            self->positionTween.initial = (self->flags & 0x100) ? self->otherPosition : self->position;
            float z = ReadF(vm, instruction, 4);
            float y = ReadF(vm, instruction, 3);
            CameraVector target = {ReadF(vm, instruction, 2), y, z};
            self->positionTween.target = target;
            ResetAnimationVectorTweenTime(&self->positionTween);
            break;
        }
        case 100:
        {
            CameraVector control1;
            control1.x = ReadF(vm, instruction, 1);
            control1.y = ReadF(vm, instruction, 2);
            control1.z = ReadF(vm, instruction, 3);
            CameraVector control2;
            control2.x = ReadF(vm, instruction, 7);
            control2.y = ReadF(vm, instruction, 8);
            control2.z = ReadF(vm, instruction, 9);
            self->positionTween.duration = ReadI(vm, instruction, 0);
            self->positionTween.control1 = control1;
            self->positionTween.control2 = control2;
            self->positionTween.mode = 8;
            self->positionTween.initial = (self->flags & 0x100) ? self->otherPosition : self->position;
            float z = ReadF(vm, instruction, 6);
            float y = ReadF(vm, instruction, 5);
            CameraVector target = {ReadF(vm, instruction, 4), y, z};
            self->positionTween.target = target;
            ResetAnimationVectorTweenTime(&self->positionTween);
            break;
        }
        case 57:
        {
            AnimationColorTriple initial = {Byte(vm, 0x374), Byte(vm, 0x375), Byte(vm, 0x376)};
            AnimationColorTriple target;
            target.blue = static_cast<unsigned char>(ReadI(vm, instruction, 4));
            target.green = static_cast<unsigned char>(ReadI(vm, instruction, 3));
            target.red = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            int duration = ReadI(vm, instruction, 0);
            SetAnimationColorTween(vm, &target, &initial, duration, static_cast<unsigned char>(Words(instruction)[1]));
            break;
        }
        case 58:
        {
            unsigned char target = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            int duration = ReadI(vm, instruction, 0);
            SetAnimationAlphaTween(vm, duration, static_cast<unsigned char>(Words(instruction)[1]), Byte(vm, 0x377), target);
            break;
        }
        case 78:
        {
            AnimationColorTriple initial = {Byte(vm, 0x378), Byte(vm, 0x379), Byte(vm, 0x37a)};
            AnimationColorTriple target;
            target.blue = static_cast<unsigned char>(ReadI(vm, instruction, 4));
            target.green = static_cast<unsigned char>(ReadI(vm, instruction, 3));
            target.red = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            int duration = ReadI(vm, instruction, 0);
            SetAnimationColor2Tween(vm, &target, &initial, duration, static_cast<unsigned char>(Words(instruction)[1]));
            break;
        }
        case 79:
        {
            unsigned char target = static_cast<unsigned char>(ReadI(vm, instruction, 2));
            int duration = ReadI(vm, instruction, 0);
            SetAnimationAlpha2Tween(vm, duration, static_cast<unsigned char>(Words(instruction)[1]), Byte(vm, 0x37b), target);
            break;
        }
        case 59:
        {
            float z = ReadF(vm, instruction, 4);
            float y = ReadF(vm, instruction, 3);
            CameraVector target = {ReadF(vm, instruction, 2), y, z};
            self->rotationTween.duration = ReadI(vm, instruction, 0);
            ZeroControls(self->rotationTween);
            self->rotationTween.mode = Words(instruction)[1];
            self->rotationTween.initial = self->rotation;
            self->rotationTween.target = target;
            ResetAnimationVectorTweenTime(&self->rotationTween);
            self->flags |= 4;
            break;
        }
        case 60:
        {
            float y = ReadF(vm, instruction, 3);
            AnimationFloatPair target(ReadF(vm, instruction, 2), y);
            int duration = ReadI(vm, instruction, 0);
            SetAnimationScaleTween(vm, &target, &self->scale, duration, static_cast<unsigned char>(Words(instruction)[1]));
            self->flags |= 8;
            break;
        }
        case 93:
        {
            float target = ReadF(vm, instruction, 2);
            self->uVelocityTween.duration = ReadI(vm, instruction, 0);
            self->uVelocityTween.control1 = self->uVelocityTween.control2 = 0.0f;
            self->uVelocityTween.mode = Words(instruction)[1];
            self->uVelocityTween.initial = self->uVelocity;
            self->uVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&self->uVelocityTween));
            break;
        }
        case 94:
        {
            float target = ReadF(vm, instruction, 2);
            self->vVelocityTween.duration = ReadI(vm, instruction, 0);
            self->vVelocityTween.control1 = self->vVelocityTween.control2 = 0.0f;
            self->vVelocityTween.mode = Words(instruction)[1];
            self->vVelocityTween.initial = self->vVelocity;
            self->vVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&self->vVelocityTween));
            break;
        }
        case 67:
            ReplaceBits(self->flags, static_cast<unsigned int>(Words(instruction)[0]) << 22, 0x3c00000);
            if ((self->flags & 0x3c00000) == 0x2800000) InitializeFanAnimationVm(vm);
            break;
        case 83:
            self->position = self->translation;
            self->translation.x = self->translation.y = self->translation.z = 0.0f;
            break;
        case 84:
            self->flags = (self->flags & ~0x1800000u) | 0x2400000;
            self->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(vm, instruction, 0)) * 56u));
            break;
        case 101:
            self->flags = (self->flags & ~0x800000u) | 0x3400000;
            self->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(vm, instruction, 0)) * 56u));
            break;

        case 98:
        {
            CameraVector quad[4];
            BuildAnimationVmModeQuad(vm, quad);
            SetAnimationUv(&self->uv[0], &quad[0]);
            SetAnimationUv(&self->uv[1], &quad[1]);
            SetAnimationUv(&self->uv[2], &quad[2]);
            SetAnimationUv(&self->uv[3], &quad[3]);
            break;
        }
        case 99: ReplaceBits(self->flags2, static_cast<unsigned int>(ReadI(vm, instruction, 0)) << 2, 4); break;
        case 6:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = right;
            break;
        }
        case 7:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = right;
            break;
        }
        case 18:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Add(right, left);
            break;
        }
        case 19:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = right + left;
            break;
        }
        case 20:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Subtract(left, right);
            break;
        }
        case 21:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = left - right;
            break;
        }
        case 22:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Multiply(right, left);
            break;
        }
        case 23:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = right * left;
            break;
        }
        case 24:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = left / right;
            break;
        }
        case 25:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = left / right;
            break;
        }
        case 26:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = left % right;
            break;
        }
        case 27:
        {
            float right = ReadF(vm, instruction, 2);
            float left = ReadF(vm, instruction, 1);
            float result = static_cast<float>(fmod(left, right));
            *FloatDestination(vm, instruction) = result;
            break;
        }
        case 8:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Add(*destination, right);
            break;
        }
        case 9:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = right + *destination;
            break;
        }
        case 10:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Subtract(*destination, right);
            break;
        }
        case 11:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination - right;
            break;
        }
        case 12:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Multiply(*destination, right);
            break;
        }
        case 13:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination * right;
            break;
        }
        case 14:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = *destination / right;
            break;
        }
        case 15:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination / right;
            break;
        }
        case 16:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = *destination % right;
            break;
        }
        case 17:
        {
            float right = ReadF(vm, instruction, 1);
            float left = ReadF(vm, instruction, 0);
            float result = static_cast<float>(fmod(left, right));
            *FloatDestination(vm, instruction) = result;
            break;
        }
        case 40:
        {
            unsigned int result = 0;
            if (self->flags & 0x40000000)
            {
                unsigned int maximum = ReadI(vm, instruction, 1);
                if (maximum) result = RandomNext32(&g_AnimationSceneRandomState) % maximum;
            }
            else
            {
                unsigned int maximum = ReadI(vm, instruction, 1);
                if (maximum) result = RandomNext32(&g_TextRandomState) % maximum;
            }
            *IntegerDestination(vm, instruction) = static_cast<int>(result);
            break;
        }
        case 41:
        {
            float result;
            if (self->flags & 0x40000000)
            {
                float maximum = ReadF(vm, instruction, 1);
                result = AnimationRandomRange(&g_AnimationSceneRandomState, maximum);
            }
            else
            {
                float maximum = ReadF(vm, instruction, 1);
                result = AnimationRandomRange(&g_TextRandomState, maximum);
            }
            *FloatDestination(vm, instruction) = result;
            break;
        }
        case 42:
        {
            float value = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = SecondaryUnaryMathA(value);
            break;
        }
        case 43:
        {
            float value = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = SecondaryUnaryMathB(value);
            break;
        }
        case 44:
        {
            float value = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = AnimationUnaryMathC(value);
            break;
        }
        case 45:
        {
            float value = ReadF(vm, instruction, 1);
            float result = static_cast<float>(acos(value));
            *FloatDestination(vm, instruction) = result;
            break;
        }
        case 46:
        {
            float value = ReadF(vm, instruction, 1);
            float result = static_cast<float>(atan(value));
            *FloatDestination(vm, instruction) = result;
            break;
        }
        case 47:
        {
            float value = ReadF(vm, instruction, 0);
            float *destination = FloatDestination(vm, instruction);
            *destination = AddAngles(value, 0.0f);
            break;
        }

        case 85: ReplaceBits(self->flags, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 28, 0x10000000); break;
        case 68: vm->persistent020 = static_cast<unsigned char>(Words(instruction)[0]); break;
        case 80: ReplaceBits(self->flags, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 15, 0x8000); break;
        case 82: ReplaceBits(self->flags, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 27, 0x8000000); break;
        case 87: ReplaceBits(self->flags, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 30, 0x40000000); break;
