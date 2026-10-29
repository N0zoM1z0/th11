#include "SecondaryActorState.hpp"

// Member constructors perform the native timer-bit and record initialization.
SecondaryActorStateView::SecondaryActorStateView() {}

SecondaryActorManagerView::SecondaryActorManagerView(const char *name)
{
    memset(&state, 0, sizeof(state));
    main.flags &= ~1u;
    main.time = 0.0f;
    current = &main;
    main.instruction = 0;
    main.owner = this;
    main.unknown1018 = 0;
    main.id = -1;
    threads.vm = &main;
    threads.next = 0;
    threads.previous = 0;
    state.owner = this;
    state.localTween.duration = 0;
    state.offsetTween.duration = 0;
    for (int i = 0; i < 6; ++i) state.pairTweens[i].duration = 0;
    memset(&state.offset, 0, sizeof(state.offset));
    memset(&state.local, 0, sizeof(state.local));
    memset(&state.world, 0, sizeof(state.world));
    state.extentX = 24.0f;
    state.extentY = 24.0f;
    state.extentZ = 24.0f;
    state.unknown0dc = 24.0f;
    state.index = -1;
    state.node.actor = this;
    state.node.next = 0;
    state.node.previous = 0;
    memset(&state.effect, 0, sizeof(state.effect));
    state.effect.height = 32.0f;
    state.effect.width = 32.0f;
    state.frame.SetFrame(-1);
    state.timer1558.SetFrame(-1);
    state.timer156c.SetFrame(-1);
    state.enabled = 1;
    program = g_SecondaryMaster->program;
    current->instruction = FindSecondarySubroutine(program, name);
    current->time = 0.0f;
    state.counter.value = 0;
    state.counter.unknown004 = 0;
    state.counter.unknown008 = 0;
    for (int i = 0; i < 8; ++i)
    {
        state.slots[i].lifeThreshold = -1;
        state.slots[i].frameLimit = -1;
    }
    for (int i = 0; i < 8; ++i) state.slots[i].lifeScript = 0;
}

SecondaryActorManagerView::~SecondaryActorManagerView()
{
    DetachSecondaryActor(this);
    if (state.flags & 0x80000)
        g_SecondaryMaster->Slot(state.index) = 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (int i = 0; i < 10; ++i)
    {
        unsigned int id = state.animationIds[i];
        BoundVmView *vm = 0;
        if (id)
        {
            for (BoundVmNodeView *node = lists->first; node; node = node->next)
                if (node->vm->id == id) { vm = node->vm; break; }
            if (!vm)
                for (BoundVmNodeView *node = lists->second; node; node = node->next)
                    if (node->vm->id == id) { vm = node->vm; break; }
            if (vm)
            {
                vm->flags |= 0x04000000;
                if (!vm->marker018)
                    for (BoundVmNodeView *node = vm->related; node; node = node->next)
                        node->vm->flags |= 0x04000000;
            }
        }
        state.animationIds[i] = 0;
    }
    SecondaryActorReferencesView *references = g_SecondaryActorReferences;
    if (references)
    {
        if (references->tracked == this)
        {
            references->tracked = 0;
            references->trackingLocked = 0;
        }
        for (int i = 0; i < 256; ++i)
            if (references->records[i].actor == this)
                references->records[i].actor = 0;
    }
    if (state.mesh)
    {
        ReleaseVmBindingBlock(reinterpret_cast<VmBindingBlockView *>(state.mesh));
        delete state.mesh;
    }
    state.mesh = 0;
}

void __stdcall DetachSecondaryActor(SecondaryActorManagerView *self)
{
    SecondaryMasterView *master = g_SecondaryMaster;
    SecondaryActorNode *node = &self->state.node;
    if (master->first == node) master->first = node->next;
    if (master->last == node) master->last = node->previous;
    if (node->next) node->next->previous = node->previous;
    if (node->previous) node->previous->next = node->next;
    node->next = 0;
    node->previous = 0;
    --master->count;
}

void __stdcall UpdateSecondaryActorPosition(SecondaryActorStateView *self)
{
    CameraVector position = {
        self->offset.position.x + self->local.position.x,
        self->offset.position.y + self->local.position.y,
        self->offset.position.z + self->local.position.z
    };
    CameraVector velocity = {
        position.x - self->world.position.x,
        position.y - self->world.position.y,
        position.z - self->world.position.z
    };
    self->world.velocity = velocity;
    ComposeSecondaryMotion(&self->world);
    if (self->flags & 0x2000)
    {
        double halfWidth = self->boundWidth * 0.5;
        if (self->world.position.x < self->boundX - halfWidth)
            self->world.position.x = static_cast<float>(self->boundX - halfWidth);
        else if (self->world.position.x > self->boundX + halfWidth)
            self->world.position.x = static_cast<float>(self->boundX + halfWidth);
        double halfHeight = self->boundHeight * 0.5;
        if (self->world.position.y < self->boundY - halfHeight)
            self->world.position.y = static_cast<float>(self->boundY - halfHeight);
        else if (self->world.position.y > self->boundY + halfHeight)
            self->world.position.y = static_cast<float>(self->boundY + halfHeight);
        CameraVector local = {
            self->world.position.x - self->offset.position.x,
            self->world.position.y - self->offset.position.y,
            self->world.position.z - self->offset.position.z
        };
        self->local.position = local;
    }
}

void __fastcall TrackSecondaryActor(SecondaryActorManagerView *self)
{
    if (!g_SecondaryActorReferences->trackingLocked)
        g_SecondaryActorReferences->tracked = self;
}

int __stdcall SecondaryActorReferenceStatus()
{
    if (g_SecondaryActorReferences->status || (g_SecondaryActorReferences->flags & 2))
        return 1;
    return 0;
}
