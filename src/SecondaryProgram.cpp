#include "SecondaryScript.hpp"
#include <stdlib.h>
#include <string.h>

namespace
{
struct SecondaryProgramHeader
{
    DWORD magic;
    WORD version, extensionBytes;
    unsigned char unknown008[8];
    WORD subroutineCount;
    unsigned char unknown012[0x24 - 0x12];
};
typedef char SecondaryProgramHeaderSize[(sizeof(SecondaryProgramHeader) == 0x24) ? 1 : -1];
}

SecondaryProgramView::~SecondaryProgramView()
{
    if (subroutines)
    {
        free(subroutines);
        subroutines = 0;
    }
}

int SecondaryProgramView::Install(void *data)
{
    files[fileCount] = data;
    SecondaryProgramHeader *header = static_cast<SecondaryProgramHeader *>(files[fileCount]);
    if (header->magic != 0x54504353)
    {
        files[fileCount] = 0;
        return -1;
    }
    if (header->version != 1)
    {
        files[fileCount] = 0;
        return -1;
    }
    SecondarySubroutine *old = subroutines;
    DWORD *offsets = reinterpret_cast<DWORD *>(reinterpret_cast<unsigned char *>(header) +
        0x24 + header->extensionBytes);
    WORD added = header->subroutineCount;
    subroutineCount += added;
    const char *name = reinterpret_cast<const char *>(offsets + added);
    subroutines = static_cast<SecondarySubroutine *>(malloc(subroutineCount * sizeof(SecondarySubroutine)));
    if (!old)
    {
        for (int i = 0; i < subroutineCount; ++i)
        {
            subroutines[i].body = static_cast<unsigned char *>(files[fileCount]) + *offsets;
            subroutines[i].name = name;
            name += strlen(name) + 1;
            ++offsets;
        }
    }
    else
    {
        int used = subroutineCount - static_cast<SecondaryProgramHeader *>(files[fileCount])->subroutineCount;
        memcpy(subroutines, old, used * sizeof(SecondarySubroutine));
        if (old) free(old);
        for (int i = 0; i < static_cast<SecondaryProgramHeader *>(files[fileCount])->subroutineCount; ++i)
        {
            int position = 0;
            while (position < used && strcmp(name, subroutines[position].name) > 0) ++position;
            // Native shifts the full allocation, including not-yet-published slots.
            for (int j = subroutineCount - 1; j > position; --j)
                subroutines[j] = subroutines[j - 1];
            subroutines[position].body = static_cast<unsigned char *>(files[fileCount]) + *offsets;
            subroutines[position].name = name;
            name += strlen(name) + 1;
            ++offsets;
            ++used;
        }
    }
    int index = fileCount++;
    SecondaryProgramHeader *installed = static_cast<SecondaryProgramHeader *>(files[index]);
    if (installed->extensionBytes)
        ProcessExtension(reinterpret_cast<unsigned char *>(installed) + 0x24);
    return index;
}

int SecondaryProgramView::ProcessExtension(void *data)
{
    return 0;
}

void *SecondaryProgramView::File(int index)
{
    return files[index];
}

int SecondaryManager::Initialize(SecondaryProgramView *source, const char *name)
{
    program = source;
    SecondaryInstruction *instruction = FindSecondarySubroutine(source, name);
    current->instruction = instruction;
    current->time = 0;
    return 0;
}
int SecondaryManager::Select(SecondaryProgramView *source, const char *name)
{
    SecondaryInstruction *instruction = FindSecondarySubroutine(source, name);
    current->instruction = instruction;
    current->time = 0;
    return current->instruction == 0;
}
int SecondaryManager::Restart(const char *name)
{
    SecondaryInstruction *instruction = FindSecondarySubroutine(program, name);
    current->instruction = instruction;
    current->time = 0;
    return 0;
}
