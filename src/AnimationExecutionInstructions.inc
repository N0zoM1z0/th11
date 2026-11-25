// Instruction groups follow reviewed native entry order; fallthroughs stay atomic.
        case 4:
            SetStageTimer(&vm->time, Words(instruction)[1]);
            vm->instruction = reinterpret_cast<AnimationInstruction *>(vm->scriptStart + Words(instruction)[0]);
            continue;
        case 5:
        {
            int *value = IntegerDestination(vm, instruction);
            --*value;
            if (ReadI(vm, instruction, 0) > 0)
            {
                SetStageTimer(&vm->time, Words(instruction)[2]);
                vm->instruction = reinterpret_cast<AnimationInstruction *>(vm->scriptStart + Words(instruction)[1]);
                continue;
            }
            break;
        }
        case 69:
            vm->flags.value &= ~1u;
            // Continue through the same event/pause operation as instruction63.
        case 63:
            if (vm->event)
            {
                event = vm->event;
                goto animation_event;
            }
            vm->flags.value |= 0x1000;
            goto animation_pause;
        case 81:
            vm->time = vm->savedTime;
            vm->instruction = vm->savedInstruction;
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
            vm->flags.value &= ~1u;
        case 2:
            vm->instruction = 0;
            g_AnimationTimeScale = savedScale;
            return 1;
        case 86: ReplaceBits(vm->flags.value, static_cast<unsigned int>(ReadI(vm, instruction, 0)) << 29, 0x20000000); break;
        case 88:
        {
            int script = ReadI(vm, instruction, 0);
            scratch.childId = CreateAnimationVmId(vm->file, script, vm->persistent020).value;
            goto animation_child_draw;
        }
        // Keep the factory's hidden-result temporary alive through ID cleanup;
        // ClearMissingAnimationId may clear this non-const temporary in place.
        case 96:
        {
            int script = ReadI(vm, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(vm->file, script, vm->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            InsertAnimationRelatedNode(&vm->drawNode, &scratch.child->drawNode);
            State(scratch.child)->otherPosition.x = ReadF(vm, instruction, 1);
            State(scratch.child)->otherPosition.y = ReadF(vm, instruction, 2);
            State(scratch.child)->translation = vm->translation;
            break;
        }
        case 97:
        {
            int script = ReadI(vm, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(vm->file, script, vm->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            float x = ReadF(vm, instruction, 1);
            AnimationExecutionView *positionedChild = State(scratch.child);
            positionedChild->otherPosition.x = x;
            positionedChild->otherPosition.y = ReadF(vm, instruction, 2);
            positionedChild->translation = vm->translation;
            break;
        }
        case 95:
        {
            int script = ReadI(vm, instruction, 0);
            const AnimationVmId &childId = CreateAnimationVmId(vm->file, script, vm->persistent020);
            scratch.child = ClearMissingAnimationId(const_cast<unsigned int *>(&childId.value));
            State(scratch.child)->otherPosition = vm->position;
            State(scratch.child)->translation = vm->translation;
            break;
        }
        case 91:
        {
            int script = ReadI(vm, instruction, 0);
            scratch.childId = CreateSecondaryEmitterAnimationVmId(vm->file, script, vm->persistent020).value;
            goto animation_child_draw;
        }
        animation_child_draw:
            scratch.child = ClearMissingAnimationId(&scratch.childId);
            InsertAnimationRelatedNode(&vm->drawNode, &scratch.child->drawNode);
            State(scratch.child)->otherPosition = vm->position;
            State(scratch.child)->translation = vm->translation;
            break;
        case 90:
        {
            int script = ReadI(vm, instruction, 0);
            scratch.childId = CreateSecondaryDrawAnimationVmId(vm->file, script, vm->persistent020).value;
            goto animation_child_front;
        }
        case 92:
        {
            int script = ReadI(vm, instruction, 0);
            scratch.childId = CreateFrontSecondaryDrawAnimationVmId(vm->file, script, vm->persistent020).value;
            goto animation_child_front;
        }
        animation_child_front:
            scratch.child = ClearMissingAnimationId(&scratch.childId);
            InsertAnimationRelatedNode(&vm->drawNode, &scratch.child->drawNode);
            State(scratch.child)->translation = vm->translation;
            State(scratch.child)->otherPosition = vm->position;
            break;
        case 3:
        {
            vm->flags.value |= 1;
            int sprite;
            if (vm->spriteTransform) sprite = vm->spriteTransform(vm, ReadI(vm, instruction, 0));
            else sprite = ReadI(vm, instruction, 0);
            vm->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file), sprite);
            vm->spriteFrame = vm->time.current;
            break;
        }
        case 102:
        {
            vm->flags.value |= 1;
            int sprite;
            if (vm->spriteTransform)
            {
                int first = ReadI(vm, instruction, 0);
                unsigned int count = ReadI(vm, instruction, 1);
                int value = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
                sprite = vm->spriteTransform(vm, value);
            }
            else
            {
                int first = ReadI(vm, instruction, 0);
                unsigned int count = ReadI(vm, instruction, 1);
                sprite = Add(static_cast<int>(RandomNext32(&g_TextRandomState) % count), first);
            }
            vm->BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file), sprite);
            vm->spriteFrame = vm->time.current;
            break;
        }
        case 50:
            vm->scale.x = ReadF(vm, instruction, 0);
            vm->scale.y = ReadF(vm, instruction, 1);
            vm->flags.value |= 8;
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
            vm->flags.value = (vm->flags.value ^ 0x200) | 8;
            vm->scale.x = static_cast<float>(vm->scale.x * -1.0);
            break;
        case 62:
            vm->flags.value = (vm->flags.value ^ 0x400) | 8;
            vm->scale.y = static_cast<float>(vm->scale.y * -1.0);
            break;
        case 49:
            vm->rotation.x = ReadF(vm, instruction, 0);
            vm->rotation.y = ReadF(vm, instruction, 1);
            vm->rotation.z = ReadF(vm, instruction, 2);
            vm->flags.value |= 4;
            break;
        case 53:
            vm->angularVelocity.x = ReadF(vm, instruction, 0);
            vm->angularVelocity.y = ReadF(vm, instruction, 1);
            vm->angularVelocity.z = ReadF(vm, instruction, 2);
            vm->flags.value |= 4;
            break;
        case 54:
            vm->scaleVelocity.x = ReadF(vm, instruction, 0);
            vm->scaleVelocity.y = ReadF(vm, instruction, 1);
            break;
        case 55:
        {
            int duration = ReadI(vm, instruction, 1);
            SetAnimationAlphaTween(vm, duration, 0, Byte(vm, 0x377), static_cast<unsigned char>(Words(instruction)[0]));
            break;
        }
        case 66: ReplaceBits(vm->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 4, 0x70); break;
        case 48:
            if ((reinterpret_cast<unsigned char *>(&vm->flags.value)[1] % 2u))
            {
                float z = ReadF(vm, instruction, 2);
                float y = ReadF(vm, instruction, 1);
                CameraVector value = {ReadF(vm, instruction, 0), y, z};
                vm->otherPosition = value;
            }
            else
            {
                float z = ReadF(vm, instruction, 2);
                float y = ReadF(vm, instruction, 1);
                CameraVector value = {ReadF(vm, instruction, 0), y, z};
                vm->position = value;
            }
            break;
        case 75: RewindSecondaryTimer(&vm->time, ReadI(vm, instruction, 0)); break;
        case 72: ReplaceBits(vm->flags.value, Words(instruction)[0], 1); break;
        case 65:
        {
            unsigned short *values = reinterpret_cast<unsigned short *>(instruction + 1);
            ReplaceBits(vm->flags.value, static_cast<unsigned int>(values[0]) << 18, 0xc0000);
            ReplaceBits(vm->flags.value, static_cast<unsigned int>(values[1]) << 20, 0x300000);
            break;
        }
        case 70: vm->uVelocity = ReadF(vm, instruction, 0); break;
        case 71: vm->vVelocity = ReadF(vm, instruction, 0); break;
        case 73: ReplaceBits(vm->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 11, 0x800); break;
        case 74: ReplaceBits(vm->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 13, 0x2000); break;
        case 89: ReplaceBits(vm->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 31, 0x80000000); break;
        case 56:
        {
            vm->positionTween.duration = ReadI(vm, instruction, 0);
            ZeroControls(vm->positionTween);
            vm->positionTween.mode = Words(instruction)[1];
            vm->positionTween.initial = ((reinterpret_cast<unsigned char *>(&vm->flags.value)[1] % 2u)) ? vm->otherPosition : vm->position;
            float z = ReadF(vm, instruction, 4);
            float y = ReadF(vm, instruction, 3);
            CameraVector target = {ReadF(vm, instruction, 2), y, z};
            vm->positionTween.target = target;
            ResetAnimationVectorTweenTime(&vm->positionTween);
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
            vm->positionTween.duration = ReadI(vm, instruction, 0);
            vm->positionTween.control1 = control1;
            vm->positionTween.control2 = control2;
            vm->positionTween.mode = 8;
            unsigned int initialX;
            unsigned int initialY;
            unsigned int initialZ;
            unsigned int *initialWords =
                reinterpret_cast<unsigned int *>(&vm->positionTween.initial);
            if (!(vm->flags.value & 0x100))
            {
                initialX = *reinterpret_cast<unsigned int *>(&vm->position.x);
                initialY = *reinterpret_cast<unsigned int *>(&vm->position.y);
                initialZ = *reinterpret_cast<unsigned int *>(&vm->position.z);
            }
            else
            {
                initialX = *reinterpret_cast<unsigned int *>(&vm->otherPosition.x);
                initialY = *reinterpret_cast<unsigned int *>(&vm->otherPosition.y);
                initialZ = *reinterpret_cast<unsigned int *>(&vm->otherPosition.z);
            }
            initialWords[0] = initialX;
            initialWords[1] = initialY;
            initialWords[2] = initialZ;
            float z = ReadF(vm, instruction, 6);
            float y = ReadF(vm, instruction, 5);
            float x = ReadF(vm, instruction, 4);
            CameraVector target = {x, y, z};
            vm->positionTween.target = target;
            ResetAnimationVectorTweenTime(&vm->positionTween);
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
            vm->rotationTween.duration = ReadI(vm, instruction, 0);
            ZeroControls(vm->rotationTween);
            vm->rotationTween.mode = Words(instruction)[1];
            vm->rotationTween.initial = vm->rotation;
            vm->rotationTween.target = target;
            ResetAnimationVectorTweenTime(&vm->rotationTween);
            vm->flags.value |= 4;
            break;
        }
        case 60:
        {
            float y = ReadF(vm, instruction, 3);
            AnimationFloatPair target(ReadF(vm, instruction, 2), y);
            int duration = ReadI(vm, instruction, 0);
            SetAnimationScaleTween(vm, &target, &vm->scale, duration, static_cast<unsigned char>(Words(instruction)[1]));
            vm->flags.value |= 8;
            break;
        }
        case 93:
        {
            float target = ReadF(vm, instruction, 2);
            vm->uVelocityTween.duration = ReadI(vm, instruction, 0);
            vm->uVelocityTween.control1 = vm->uVelocityTween.control2 = 0.0f;
            vm->uVelocityTween.mode = Words(instruction)[1];
            vm->uVelocityTween.initial = vm->uVelocity;
            vm->uVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&vm->uVelocityTween));
            break;
        }
        case 94:
        {
            float target = ReadF(vm, instruction, 2);
            vm->vVelocityTween.duration = ReadI(vm, instruction, 0);
            vm->vVelocityTween.control1 = vm->vVelocityTween.control2 = 0.0f;
            vm->vVelocityTween.mode = Words(instruction)[1];
            vm->vVelocityTween.initial = vm->vVelocity;
            vm->vVelocityTween.target = target;
            ResetAnimationScalarTweenTime(reinterpret_cast<AnimationScalarTweenTimeView *>(&vm->vVelocityTween));
            break;
        }
        case 67:
            ReplaceBits(vm->flags.value, static_cast<unsigned int>(Words(instruction)[0]) << 22, 0x3c00000);
            if ((vm->flags.value & 0x3c00000) == 0x2800000) InitializeFanAnimationVm(vm);
            break;
        case 83:
            vm->position = vm->translation;
            vm->translation.x = vm->translation.y = vm->translation.z = 0.0f;
            break;
        case 84:
            vm->flags.value = (vm->flags.value & ~0x1800000u) | 0x2400000;
            vm->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(vm, instruction, 0)) * 56u));
            break;
        case 101:
            vm->flags.value = (vm->flags.value & ~0x800000u) | 0x3400000;
            vm->vertices = static_cast<RenderVertex *>(malloc(static_cast<unsigned int>(ReadI(vm, instruction, 0)) * 56u));
            break;

        case 98:
        {
            CameraVector quad[4];
            BuildModeQuad(quad, vm);
            SetAnimationUv(&vm->uv[0], &quad[0]);
            SetAnimationUv(&vm->uv[1], &quad[1]);
            SetAnimationUv(&vm->uv[2], &quad[2]);
            SetAnimationUv(&vm->uv[3], &quad[3]);
            break;
        }
        case 99: ReplaceBits(vm->flags2, static_cast<unsigned int>(ReadI(vm, instruction, 0)) << 2, 4); break;
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
            scratch.floatResult = static_cast<float>(fmod(left, right));
            goto animation_float_destination;
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
            scratch.floatResult = static_cast<float>(fmod(left, right));
            goto animation_float_destination;
        }
        case 40:
        {
            unsigned int result;
            if (reinterpret_cast<unsigned char *>(&vm->flags.value)[3] & 0x40)
            {
                unsigned int maximum = ReadI(vm, instruction, 1);
                if (maximum) result = RandomNext32(&g_AnimationSceneRandomState) % maximum;
                else result = 0;
            }
            else
            {
                unsigned int maximum = ReadI(vm, instruction, 1);
                if (maximum) result = RandomNext32(&g_TextRandomState) % maximum;
                else result = 0;
            }
            *IntegerDestination(vm, instruction) = static_cast<int>(result);
            break;
        }
        case 41:
        {
            RandomStateView *random;
            float maximum;
            if (reinterpret_cast<unsigned char *>(&vm->flags.value)[3] & 0x40)
            {
                maximum = ReadF(vm, instruction, 1);
                random = &g_AnimationSceneRandomState;
            }
            else
            {
                maximum = ReadF(vm, instruction, 1);
                random = &g_TextRandomState;
            }
            float result = AnimationRandomRange(random, maximum);
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
            scratch.floatResult = static_cast<float>(acos(value));
            goto animation_float_destination;
        }
        case 46:
        {
            float value = ReadF(vm, instruction, 1);
            scratch.floatResult = static_cast<float>(atan(value));
            goto animation_float_destination;
        }
        case 47:
        {
            float value = ReadF(vm, instruction, 0);
            float *destination = FloatDestination(vm, instruction);
            *destination = AddAngles(value, 0.0f);
            break;
        }

animation_float_destination:
            *FloatDestination(vm, instruction) = scratch.floatResult;
            break;

        case 85: ReplaceBits(vm->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 28, 0x10000000); break;
        case 68: vm->persistent020 = static_cast<unsigned char>(Words(instruction)[0]); break;
        case 80: ReplaceBits(vm->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 15, 0x8000); break;
        case 82: ReplaceBits(vm->flags.value, static_cast<unsigned int>(static_cast<unsigned char>(Words(instruction)[0])) << 27, 0x8000000); break;
        case 87:
        {
            unsigned char *flags = reinterpret_cast<unsigned char *>(&vm->flags.value);
            flags[3] = static_cast<unsigned char>((flags[3] & 0xbf) |
                ((static_cast<unsigned char>(Words(instruction)[0]) & 1u) << 6));
            break;
        }
