// Instruction groups follow reviewed native entry order; fallthroughs stay atomic.
        case 4:
            SetStageTimer(&self->time, Words(instruction)[1]);
            self->instruction = reinterpret_cast<AnimationInstruction *>(self->scriptStart + Words(instruction)[0]);
            continue;
        case 5:
        {
            int *value = IntegerDestination(self, instruction);
            --*value;
            if (ReadI(self, instruction, 0) > 0)
            {
                SetStageTimer(&self->time, Words(instruction)[2]);
                self->instruction = reinterpret_cast<AnimationInstruction *>(self->scriptStart + Words(instruction)[1]);
                continue;
            }
            break;
        }
        case 69:
            self->flags.value &= ~1u;
            // Continue through the same event/pause operation as instruction63.
        case 63:
            if (self->event)
            {
                event = self->event;
                goto animation_event;
            }
            self->flags.value |= 0x1000;
            goto animation_pause;
        case 81:
            self->time = self->savedTime;
            self->instruction = self->savedInstruction;
            continue;
// Integer/float arguments and destinations retain their separate read order.
        case 28:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left == right) goto animation_jump;
            break;
        }
        case 29:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left == right) goto animation_jump;
            break;
        }
        case 30:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left != right) goto animation_jump;
            break;
        }
        case 31:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left != right) goto animation_jump;
            break;
        }
        case 32:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left < right) goto animation_jump;
            break;
        }
        case 33:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left < right) goto animation_jump;
            break;
        }
        case 34:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left <= right) goto animation_jump;
            break;
        }
        case 35:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left <= right) goto animation_jump;
            break;
        }
        case 36:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left > right) goto animation_jump;
            break;
        }
        case 37:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left > right) goto animation_jump;
            break;
        }
        case 38:
        {
            int left = ReadI(self, instruction, 0);
            int right = ReadI(self, instruction, 1);
            if (left >= right) goto animation_jump;
            break;
        }
        case 39:
        {
            float left = ReadF(self, instruction, 0);
            float right = ReadF(self, instruction, 1);
            if (left >= right) goto animation_jump;
            break;
        }
        case -1:
        case 1:
            self->flags.value &= ~1u;
        case 2:
            self->instruction = 0;
            g_AnimationTimeScale = savedScale;
            return 1;
        case 86: ReplaceBits(self->flags.value, static_cast<unsigned int>(ReadI(self, instruction, 0)) << 29, 0x20000000); break;
        case 88:
        {
            int script = ReadI(self, instruction, 0);
            scratch.childId = CreateAnimationVmId(self->file, script, self->persistent020).value;
            goto animation_child_draw;
        }
        // Keep the factory's hidden-result temporary alive through ID cleanup;
        // ClearMissingAnimationId may clear this non-const temporary in place.
        case 96:
        {
            int script = ReadI(self, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(self->file, script, self->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            InsertAnimationRelatedNode(&self->drawNode, &scratch.child->drawNode);
            State(scratch.child)->otherPosition.x = ReadF(self, instruction, 1);
            State(scratch.child)->otherPosition.y = ReadF(self, instruction, 2);
            State(scratch.child)->translation = self->translation;
            break;
        }
        case 97:
        {
            int script = ReadI(self, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(self->file, script, self->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            float x = ReadF(self, instruction, 1);
            AnimationExecutionView *positionedChild = State(scratch.child);
            positionedChild->otherPosition.x = x;
            positionedChild->otherPosition.y = ReadF(self, instruction, 2);
            positionedChild->translation = self->translation;
            break;
        }
        case 95:
        {
            int script = ReadI(self, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(self->file, script, self->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            State(scratch.child)->otherPosition = self->position;
            State(scratch.child)->translation = self->translation;
            break;
        }
        case 91:
        {
            int script = ReadI(self, instruction, 0);
            scratch.childId = CreateSecondaryEmitterAnimationVmId(self->file, script, self->persistent020).value;
            goto animation_child_draw;
        }
        animation_child_draw:
            scratch.child = ClearMissingAnimationId(&scratch.childId);
            InsertAnimationRelatedNode(&self->drawNode, &scratch.child->drawNode);
            State(scratch.child)->otherPosition = self->position;
            State(scratch.child)->translation = self->translation;
            break;
        case 90:
        {
            int script = ReadI(self, instruction, 0);
            scratch.childId = CreateSecondaryDrawAnimationVmId(self->file, script, self->persistent020).value;
            goto animation_child_front;
        }
        case 92:
        {
            int script = ReadI(self, instruction, 0);
            scratch.childId = CreateFrontSecondaryDrawAnimationVmId(self->file, script, self->persistent020).value;
            goto animation_child_front;
        }
        animation_child_front:
            scratch.child = ClearMissingAnimationId(&scratch.childId);
            InsertAnimationRelatedNode(&self->drawNode, &scratch.child->drawNode);
            State(scratch.child)->translation = self->translation;
            State(scratch.child)->otherPosition = self->position;
            break;
        case 3:
        {
            self->flags.value |= 1;
            int sprite;
            if (self->spriteTransform) sprite = self->spriteTransform(self, ReadI(self, instruction, 0));
            else sprite = ReadI(self, instruction, 0);
            self->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(self->file), sprite);
            self->spriteFrame = self->time.current;
            break;
        }
        case 102:
        {
            self->flags.value |= 1;
            int sprite;
            if (self->spriteTransform)
            {
                int first = ReadI(self, instruction, 0);
                unsigned int count = ReadI(self, instruction, 1);
                int value = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
                sprite = self->spriteTransform(self, value);
            }
            else
            {
                int first = ReadI(self, instruction, 0);
                unsigned int count = ReadI(self, instruction, 1);
                sprite = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
            }
            self->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(self->file), sprite);
            self->spriteFrame = self->time.current;
            break;
        }
        case 50:
            self->scale.x = ReadF(self, instruction, 0);
            self->scale.y = ReadF(self, instruction, 1);
            self->flags.value |= 8;
            break;
        case 51: Byte(self, 0x377) = static_cast<unsigned char>(ReadI(self, instruction, 0)); break;
        case 52:
            Byte(self, 0x376) = static_cast<unsigned char>(ReadI(self, instruction, 0));
            Byte(self, 0x375) = static_cast<unsigned char>(ReadI(self, instruction, 1));
            Byte(self, 0x374) = static_cast<unsigned char>(ReadI(self, instruction, 2));
            break;
        case 77: Byte(self, 0x37b) = static_cast<unsigned char>(ReadI(self, instruction, 0)); break;
        case 76:
            Byte(self, 0x37a) = static_cast<unsigned char>(ReadI(self, instruction, 0));
            Byte(self, 0x379) = static_cast<unsigned char>(ReadI(self, instruction, 1));
            Byte(self, 0x378) = static_cast<unsigned char>(ReadI(self, instruction, 2));
            break;
        case 61:
            self->flags.value = (self->flags.value ^ 0x200) | 8;
            self->scale.x = static_cast<float>(self->scale.x * -1.0);
            break;
        case 62:
            self->flags.value = (self->flags.value ^ 0x400) | 8;
            self->scale.y = static_cast<float>(self->scale.y * -1.0);
            break;
        case 49:
            self->rotation.x = ReadF(self, instruction, 0);
            self->rotation.y = ReadF(self, instruction, 1);
            self->rotation.z = ReadF(self, instruction, 2);
            self->flags.value |= 4;
            break;
        case 53:
            self->angularVelocity.x = ReadF(self, instruction, 0);
            self->angularVelocity.y = ReadF(self, instruction, 1);
            self->angularVelocity.z = ReadF(self, instruction, 2);
            self->flags.value |= 4;
            break;
        case 54:
            self->scaleVelocity.x = ReadF(self, instruction, 0);
            self->scaleVelocity.y = ReadF(self, instruction, 1);
            break;
        case 55:
        {
            int duration = ReadI(self, instruction, 1);
            SetAnimationAlphaTween(self, duration, 0, Byte(self, 0x377), static_cast<unsigned char>(Words(instruction)[0]));
            break;
        }
        case 66: ReplaceBits(self->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 4, 0x70); break;
        case 48:
            if ((reinterpret_cast<unsigned char *>(&self->flags.value)[1] % 2u))
            {
                float z = ReadF(self, instruction, 2);
                float y = ReadF(self, instruction, 1);
                CameraVector value = {ReadF(self, instruction, 0), y, z};
                self->otherPosition = value;
            }
            else
            {
                float z = ReadF(self, instruction, 2);
                float y = ReadF(self, instruction, 1);
                CameraVector value = {ReadF(self, instruction, 0), y, z};
                self->position = value;
            }
            break;
        case 75: RewindSecondaryTimer(&self->time, ReadI(self, instruction, 0)); break;
        case 72: ReplaceBits(self->flags.value, Words(instruction)[0], 1); break;
        case 65:
        {
            unsigned short *values = reinterpret_cast<unsigned short *>(instruction + 1);
            ReplaceBits(self->flags.value, static_cast<unsigned int>(values[0]) << 18, 0xc0000);
            ReplaceBits(self->flags.value, static_cast<unsigned int>(values[1]) << 20, 0x300000);
            break;
        }
        case 70: self->uVelocity = ReadF(self, instruction, 0); break;
        case 71: self->vVelocity = ReadF(self, instruction, 0); break;
        case 73: ReplaceBits(self->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 11, 0x800); break;
        case 74: ReplaceBits(self->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 13, 0x2000); break;
        case 89: ReplaceBits(self->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 31, 0x80000000); break;
        case 56:
        {
            self->positionTween.duration = ReadI(self, instruction, 0);
            ZeroControls(self->positionTween);
            self->positionTween.mode = Words(instruction)[1];
            self->positionTween.initial = ((reinterpret_cast<unsigned char *>(&self->flags.value)[1] % 2u)) ? self->otherPosition : self->position;
            float z = ReadF(self, instruction, 4);
            float y = ReadF(self, instruction, 3);
            CameraVector target = {ReadF(self, instruction, 2), y, z};
            self->positionTween.target = target;
            ResetAnimationVectorTweenTime(&self->positionTween);
            break;
        }
        case 100:
        {
            CameraVector control1;
            control1.x = ReadF(self, instruction, 1);
            control1.y = ReadF(self, instruction, 2);
            control1.z = ReadF(self, instruction, 3);
            CameraVector control2;
            control2.x = ReadF(self, instruction, 7);
            control2.y = ReadF(self, instruction, 8);
            control2.z = ReadF(self, instruction, 9);
            self->positionTween.duration = ReadI(self, instruction, 0);
            self->positionTween.control1 = control1;
            self->positionTween.control2 = control2;
            self->positionTween.mode = 8;
            unsigned int initialX;
            unsigned int initialY;
            unsigned int initialZ;
            unsigned int *initialWords =
                reinterpret_cast<unsigned int *>(&self->positionTween.initial);
            if (!(self->flags.value & 0x100))
            {
                initialX = *reinterpret_cast<unsigned int *>(&self->position.x);
                initialY = *reinterpret_cast<unsigned int *>(&self->position.y);
                initialZ = *reinterpret_cast<unsigned int *>(&self->position.z);
            }
            else
            {
                initialX = *reinterpret_cast<unsigned int *>(&self->otherPosition.x);
                initialY = *reinterpret_cast<unsigned int *>(&self->otherPosition.y);
                initialZ = *reinterpret_cast<unsigned int *>(&self->otherPosition.z);
            }
            initialWords[0] = initialX;
            initialWords[1] = initialY;
            initialWords[2] = initialZ;
            float z = ReadF(self, instruction, 6);
            float y = ReadF(self, instruction, 5);
            float x = ReadF(self, instruction, 4);
            CameraVector target = {x, y, z};
            self->positionTween.target = target;
            ResetAnimationVectorTweenTime(&self->positionTween);
            break;
        }
        case 57:
        {
            AnimationColorTriple initial = {Byte(self, 0x374), Byte(self, 0x375), Byte(self, 0x376)};
            AnimationColorTriple target;
            target.blue = static_cast<unsigned char>(ReadI(self, instruction, 4));
            target.green = static_cast<unsigned char>(ReadI(self, instruction, 3));
            target.red = static_cast<unsigned char>(ReadI(self, instruction, 2));
            int duration = ReadI(self, instruction, 0);
            SetAnimationColorTween(self, &target, &initial, duration, static_cast<unsigned char>(Words(instruction)[1]));
            break;
        }
        case 58:
        {
            unsigned char target = static_cast<unsigned char>(ReadI(self, instruction, 2));
            int duration = ReadI(self, instruction, 0);
            SetAnimationAlphaTween(self, duration, static_cast<unsigned char>(Words(instruction)[1]), Byte(self, 0x377), target);
            break;
        }
        case 78:
        {
            AnimationColorTriple initial = {Byte(self, 0x378), Byte(self, 0x379), Byte(self, 0x37a)};
            AnimationColorTriple target;
            target.blue = static_cast<unsigned char>(ReadI(self, instruction, 4));
            target.green = static_cast<unsigned char>(ReadI(self, instruction, 3));
            target.red = static_cast<unsigned char>(ReadI(self, instruction, 2));
            int duration = ReadI(self, instruction, 0);
            SetAnimationColor2Tween(self, &target, &initial, duration, static_cast<unsigned char>(Words(instruction)[1]));
            break;
        }
        case 79:
        {
            unsigned char target = static_cast<unsigned char>(ReadI(self, instruction, 2));
            int duration = ReadI(self, instruction, 0);
            SetAnimationAlpha2Tween(self, duration, static_cast<unsigned char>(Words(instruction)[1]), Byte(self, 0x37b), target);
            break;
        }
        case 59:
        {
            float z = ReadF(self, instruction, 4);
            float y = ReadF(self, instruction, 3);
            CameraVector target = {ReadF(self, instruction, 2), y, z};
            self->rotationTween.duration = ReadI(self, instruction, 0);
            ZeroControls(self->rotationTween);
            self->rotationTween.mode = Words(instruction)[1];
            self->rotationTween.initial = self->rotation;
            self->rotationTween.target = target;
            ResetAnimationVectorTweenTime(&self->rotationTween);
            self->flags.value |= 4;
            break;
        }
        case 60:
        {
            float y = ReadF(self, instruction, 3);
            AnimationFloatPair target(ReadF(self, instruction, 2), y);
            int duration = ReadI(self, instruction, 0);
            SetAnimationScaleTween(self, &target, &self->scale, duration, static_cast<unsigned char>(Words(instruction)[1]));
            self->flags.value |= 8;
            break;
        }
        case 93:
        {
            float target = ReadF(self, instruction, 2);
            self->uVelocityTween.duration = ReadI(self, instruction, 0);
            self->uVelocityTween.control1 = self->uVelocityTween.control2 = 0.0f;
            self->uVelocityTween.mode = Words(instruction)[1];
            self->uVelocityTween.initial = self->uVelocity;
            self->uVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&self->uVelocityTween));
            break;
        }
        case 94:
        {
            float target = ReadF(self, instruction, 2);
            self->vVelocityTween.duration = ReadI(self, instruction, 0);
            self->vVelocityTween.control1 = self->vVelocityTween.control2 = 0.0f;
            self->vVelocityTween.mode = Words(instruction)[1];
            self->vVelocityTween.initial = self->vVelocity;
            self->vVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&self->vVelocityTween));
            break;
        }
        case 67:
            ReplaceBits(self->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 22, 0x3c00000);
            if ((self->flags.value & 0x3c00000) == 0x2800000) InitializeFanAnimationVm(self);
            break;
        case 83:
            self->position = self->translation;
            self->translation.x = self->translation.y = self->translation.z = 0.0f;
            break;
        case 84:
            self->flags.value = (self->flags.value & ~0x1800000u) | 0x2400000;
            self->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(self, instruction, 0)) * 56u));
            break;
        case 101:
            self->flags.value = (self->flags.value & ~0x800000u) | 0x3400000;
            self->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(self, instruction, 0)) * 56u));
            break;

        case 98:
        {
            CameraVector quad[4];
            BuildModeQuad(quad, self);
            SetAnimationUv(&self->uv[0], &quad[0]);
            SetAnimationUv(&self->uv[1], &quad[1]);
            SetAnimationUv(&self->uv[2], &quad[2]);
            SetAnimationUv(&self->uv[3], &quad[3]);
            break;
        }
        case 99: ReplaceBits(self->flags2, static_cast<unsigned int>(ReadI(self, instruction, 0)) << 2, 4); break;
        case 6:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = right;
            break;
        }
        case 7:
        {
            float right = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = right;
            break;
        }
        case 18:
        {
            int left = ReadI(self, instruction, 1);
            int right = ReadI(self, instruction, 2);
            int *destination = IntegerDestination(self, instruction);
            *destination = Add(right, left);
            break;
        }
        case 19:
        {
            float left = ReadF(self, instruction, 1);
            float right = ReadF(self, instruction, 2);
            float *destination = FloatDestination(self, instruction);
            *destination = right + left;
            break;
        }
        case 20:
        {
            int left = ReadI(self, instruction, 1);
            int right = ReadI(self, instruction, 2);
            int *destination = IntegerDestination(self, instruction);
            *destination = Subtract(left, right);
            break;
        }
        case 21:
        {
            float left = ReadF(self, instruction, 1);
            float right = ReadF(self, instruction, 2);
            float *destination = FloatDestination(self, instruction);
            *destination = left - right;
            break;
        }
        case 22:
        {
            int left = ReadI(self, instruction, 1);
            int right = ReadI(self, instruction, 2);
            int *destination = IntegerDestination(self, instruction);
            *destination = Multiply(right, left);
            break;
        }
        case 23:
        {
            float left = ReadF(self, instruction, 1);
            float right = ReadF(self, instruction, 2);
            float *destination = FloatDestination(self, instruction);
            *destination = right * left;
            break;
        }
        case 24:
        {
            int left = ReadI(self, instruction, 1);
            int right = ReadI(self, instruction, 2);
            int *destination = IntegerDestination(self, instruction);
            *destination = left / right;
            break;
        }
        case 25:
        {
            float left = ReadF(self, instruction, 1);
            float right = ReadF(self, instruction, 2);
            float *destination = FloatDestination(self, instruction);
            *destination = left / right;
            break;
        }
        case 26:
        {
            int left = ReadI(self, instruction, 1);
            int right = ReadI(self, instruction, 2);
            int *destination = IntegerDestination(self, instruction);
            *destination = left % right;
            break;
        }
        case 27:
        {
            float right = ReadF(self, instruction, 2);
            float left = ReadF(self, instruction, 1);
            scratch.floatResult = static_cast<float>(fmod(left, right));
            goto animation_float_destination;
        }
        case 8:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = Add(*destination, right);
            break;
        }
        case 9:
        {
            float right = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = right + *destination;
            break;
        }
        case 10:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = Subtract(*destination, right);
            break;
        }
        case 11:
        {
            float right = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = *destination - right;
            break;
        }
        case 12:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = Multiply(*destination, right);
            break;
        }
        case 13:
        {
            float right = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = *destination * right;
            break;
        }
        case 14:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = *destination / right;
            break;
        }
        case 15:
        {
            float right = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = *destination / right;
            break;
        }
        case 16:
        {
            int right = ReadI(self, instruction, 1);
            int *destination = IntegerDestination(self, instruction);
            *destination = *destination % right;
            break;
        }
        case 17:
        {
            float right = ReadF(self, instruction, 1);
            float left = ReadF(self, instruction, 0);
            scratch.floatResult = static_cast<float>(fmod(left, right));
            goto animation_float_destination;
        }
        case 40:
        {
            unsigned int result;
            if (reinterpret_cast<unsigned char *>(&self->flags.value)[3] & 0x40)
            {
                unsigned int maximum = ReadI(self, instruction, 1);
                if (maximum) result = RandomNext32(&g_AnimationSceneRandomState) % maximum;
                else result = 0;
            }
            else
            {
                unsigned int maximum = ReadI(self, instruction, 1);
                if (maximum) result = RandomNext32(&g_TextRandomState) % maximum;
                else result = 0;
            }
            *IntegerDestination(self, instruction) = static_cast<int>(result);
            break;
        }
        case 41:
        {
            RandomStateView *random;
            float maximum;
            if (reinterpret_cast<unsigned char *>(&self->flags.value)[3] & 0x40)
            {
                maximum = ReadF(self, instruction, 1);
                random = &g_AnimationSceneRandomState;
            }
            else
            {
                maximum = ReadF(self, instruction, 1);
                random = &g_TextRandomState;
            }
            float result = AnimationRandomRange(random, maximum);
            *FloatDestination(self, instruction) = result;
            break;
        }
        case 42:
        {
            float value = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = SecondaryUnaryMathA(value);
            break;
        }
        case 43:
        {
            float value = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = SecondaryUnaryMathB(value);
            break;
        }
        case 44:
        {
            float value = ReadF(self, instruction, 1);
            float *destination = FloatDestination(self, instruction);
            *destination = AnimationUnaryMathC(value);
            break;
        }
        case 45:
        {
            float value = ReadF(self, instruction, 1);
            scratch.floatResult = static_cast<float>(acos(value));
            goto animation_float_destination;
        }
        case 46:
        {
            float value = ReadF(self, instruction, 1);
            scratch.floatResult = static_cast<float>(atan(value));
            goto animation_float_destination;
        }
        case 47:
        {
            float value = ReadF(self, instruction, 0);
            float *destination = FloatDestination(self, instruction);
            *destination = AddAngles(value, 0.0f);
            break;
        }

animation_float_destination:
            *FloatDestination(self, instruction) = scratch.floatResult;
            break;

        case 85: ReplaceBits(self->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 28, 0x10000000); break;
        case 68: self->persistent020 = static_cast<unsigned char>(Words(instruction)[0]); break;
        case 80: ReplaceBits(self->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 15, 0x8000); break;
        case 82: ReplaceBits(self->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 27, 0x8000000); break;
        case 87:
        {
            unsigned char *flags = reinterpret_cast<unsigned char *>(&self->flags.value);
            flags[3] = static_cast<unsigned char>((flags[3] & 0xbf) |
                ((static_cast<unsigned char>(Words(instruction)[0]) & 1u) << 6));
            break;
        }
