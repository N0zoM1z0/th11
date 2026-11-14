// Included inside the eventual complete host switch, with self, instruction
// and opcode captured at entry. This is a source fragment, not a host fallback.
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
    int pairIndex = opcode == 0x121 ? 0 : 1;
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
    Pair(&self->pairTweens[pairIndex], angularInitial, angularTarget, duration, mode);
    Pair(&self->pairTweens[pairIndex + 2], radialInitial, radialTarget, duration, mode);
    motion->flags |= 1;
    RecomposeSecondaryHostMotion(motion);
    UpdateSecondaryActorPosition(self);
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
case 0x126:
    self->local.position = g_SecondaryMaster->Slot(0)->state.world.position;
    break;
case 0x127:
    self->offset.position = g_SecondaryMaster->Slot(0)->state.world.position;
    break;
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
case 0x130:
{
    unsigned int value = static_cast<unsigned int>(Integer(self, 0));
    self->flags = (self->flags & ~0x8000u) | ((value << 15) & 0x8000u);
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
