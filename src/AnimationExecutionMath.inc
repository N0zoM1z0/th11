// Integer/float arguments and destinations retain their separate read order.
        case 6:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = right;
            break;
        }
        case 8:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Add(*destination, right);
            break;
        }
        case 10:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Subtract(*destination, right);
            break;
        }
        case 12:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Multiply(*destination, right);
            break;
        }
        case 14:
        {
            int right = ReadI(vm, instruction, 1);
            int *destination = IntegerDestination(vm, instruction);
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
        case 7:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = right;
            break;
        }
        case 9:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = right + *destination;
            break;
        }
        case 11:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination - right;
            break;
        }
        case 13:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination * right;
            break;
        }
        case 15:
        {
            float right = ReadF(vm, instruction, 1);
            float *destination = FloatDestination(vm, instruction);
            *destination = *destination / right;
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
        case 18:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Add(right, left);
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
        case 22:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = Multiply(right, left);
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
        case 26:
        {
            int left = ReadI(vm, instruction, 1);
            int right = ReadI(vm, instruction, 2);
            int *destination = IntegerDestination(vm, instruction);
            *destination = left % right;
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
        case 21:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = left - right;
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
        case 25:
        {
            float left = ReadF(vm, instruction, 1);
            float right = ReadF(vm, instruction, 2);
            float *destination = FloatDestination(vm, instruction);
            *destination = left / right;
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
        case 28:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
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
        case 32:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
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
        case 36:
        {
            int left = ReadI(vm, instruction, 0);
            int right = ReadI(vm, instruction, 1);
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
        case 29:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left == right) goto animation_jump;
            break;
        }
        case 31:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left != right) goto animation_jump;
            break;
        }
        case 33:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left < right) goto animation_jump;
            break;
        }
        case 35:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left <= right) goto animation_jump;
            break;
        }
        case 37:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left > right) goto animation_jump;
            break;
        }
        case 39:
        {
            float left = ReadF(vm, instruction, 0);
            float right = ReadF(vm, instruction, 1);
            if (left >= right) goto animation_jump;
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
            float maximum;
            if (self->flags & 0x40000000) maximum = ReadF(vm, instruction, 1);
            else maximum = ReadF(vm, instruction, 1);
            float result = AnimationRandomRange(&g_TextRandomState, maximum);
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
