#include "SecondaryPlayerDrop.hpp"
#include "MeshEffects.hpp"
#include "SecondaryPattern.hpp"
#include <stddef.h>

namespace
{
__forceinline void MoveSecondaryDrop(SecondaryDropRecord *entry,
    const CameraVector *position, float angle, float speed)
{
    entry->position = *position;
    SecondaryDropPolarVector *velocity =
        reinterpret_cast<SecondaryDropPolarVector *>(&entry->velocity);
    velocity->SetPolar(angle, speed);
    entry->velocity.z = 0.0f;
    entry->timer.SetFrame(-1);
    entry->extra44c = 0.0f;
}

// Original signed remainder contract, including negative history values.
__forceinline int SignedWrap(int index, int modulus)
{
    return index % modulus;
}
}

// Complete original TH11 0x424230..0x424582 owner, 851 bytes including the
// native switch table/alignment region. The separate physical pools and
// failure/early return paths are retained without adding index guards.
int __stdcall SpawnSecondaryPlayerDrop(SecondaryControllerDropManager *manager,
    const CameraVector *position, int kind, unsigned int color, float angle, float speed)
{
    if (kind == 8)
    {
        int head = manager->historyHead;
        int count = static_cast<int>(static_cast<unsigned int>(manager->historyCount) + 1u);
        manager->historyCount = count;
        SecondaryDropRecord *entry = &manager->records[150 + head];
        if (entry->state == 0)
        {
            int group;
            if (count >= 0x400) group = SignedWrap(head, 32) + 16;
            else if (count >= 0x200) group = SignedWrap(head, 16) + 8;
            else if (count >= 0x100) group = SignedWrap(head, 8) + 4;
            else group = SignedWrap(head, 4);
            entry->group = group;
            entry->state = 5;
            entry->kind = 8;
            entry->visibleKind = 8;
            MoveSecondaryDrop(entry, position, angle, speed);
        }
        manager->historyHead = SignedWrap(
            static_cast<int>(static_cast<unsigned int>(manager->historyHead) + 1u), 2048);
        return 0;
    }

    SecondaryDropRecord *entry = &manager->records[0];
    int index = 0;
    while (entry->state != 0)
    {
        ++index;
        ++entry;
        if (index >= 150) return 0;
    }
    entry->state = 1;
    entry->position = *position;
    if (!(entry->position.x > -192.0f)) entry->position.x = -192.0f;
    else if (entry->position.x > 192.0f) entry->position.x = 192.0f;
    SecondaryDropPolarVector *velocity =
        reinterpret_cast<SecondaryDropPolarVector *>(&entry->velocity);
    velocity->SetPolar(angle, speed);
    entry->velocity.z = 0.0f;
    entry->timer.SetFrame(-1);
    entry->extra44c = 0.0f;

    if (g_SecondaryPatternPower >= g_SecondaryPatternMaximumPower)
    {
        switch (kind)
        {
        case 1:
        case 4:
        case 10:
        case 11:
            kind = 9;
            break;
        }
    }
    if (kind == 5)
    {
        CreatePositionedAnimationVmId(g_SecondaryActorFiles->file,
            &entry->position, 0x73, 0x16);
        QueueMeshEffectSound(0x34);
    }
    entry->kind = kind;
    entry->visibleKind = kind;
    int script;
    if (kind == 10)
    {
        entry->visibleKind = 1;
        ResetAnimationVm(&entry->animation);
        script = 0x61;
    }
    else if (kind == 11)
    {
        entry->visibleKind = 4;
        ResetAnimationVm(&entry->animation);
        script = 0x64;
    }
    else
    {
        ResetAnimationVm(&entry->animation);
        script = kind + 0x60;
    }
    reinterpret_cast<unsigned char *>(&entry->animation)[0x41c] = 0x10;
    reinterpret_cast<unsigned char *>(&entry->animation)[0x41d] = 0x10;
    BindAnimationScript(g_SecondaryActorFiles->file, script, &entry->animation);
    entry->animation.spriteIndex = static_cast<int>(color);
    return 0;
}
