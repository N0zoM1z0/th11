#pragma once
#include <windows.h>
#include <stddef.h>
#include <string.h>

struct SecondaryInstruction
{
    int time;
    short opcode;
    WORD bytes, references;
    BYTE difficulty, arguments;
    DWORD unknown00c;
};
union SecondaryValue { int integer; unsigned int bits; float real; };
struct SecondaryStack
{
    void *RelativeAddress(int offset);
    void SetTop(int offset);
    unsigned char data[4096];
    int top, frame;
};
struct SecondaryManager;
struct SecondaryVm
{
    float time;
    SecondaryInstruction *instruction;
    SecondaryStack stack;
    int id;
    SecondaryManager *owner;
    int unknown1018;
    BYTE difficulty, unknown101d[3];
    unsigned int flags;
    SecondaryVm() { stack.top = stack.frame = 0; }
};
struct SecondaryThread
{
    SecondaryVm *vm;
    SecondaryThread *next, *previous;
};
struct SecondarySubroutine { const char *name; unsigned char *body; };
struct SecondaryProgramView
{
    SecondaryProgramView();
    ~SecondaryProgramView();
    virtual int Install(void *data);
    virtual int ProcessExtension(void *data);
    void *File(int index);
    int fileCount, subroutineCount;
    void *files[32];
    SecondarySubroutine *subroutines;
    // Both native constructors clear this observed allocation extent; contents stay unknown.
    unsigned char unknown090[0x1000];
    DWORD unknown1090, unknown1094;
};
inline SecondaryProgramView::SecondaryProgramView() : unknown1090(0), unknown1094(0)
{
    memset(this, 0, sizeof(*this));
}
struct SecondaryManager
{
    virtual int ExecuteInstruction();
    virtual int ReadGlobalInt(int reference);
    virtual int *GlobalIntPointer(int reference);
    virtual float ReadGlobalFloat(int reference);
    virtual float *GlobalFloatPointer(int reference);
    virtual ~SecondaryManager();
    int Initialize(SecondaryProgramView *source, const char *name);
    int Select(SecondaryProgramView *source, const char *name);
    int Restart(const char *name);
    SecondaryVm *current;
    SecondaryVm main;
    SecondaryProgramView *program;
    SecondaryThread threads;
};
struct SecondaryVector2
{
    float x, y;
    void SetPolar(float angle, float magnitude);
};

typedef char SecondaryInstructionSize[(sizeof(SecondaryInstruction) == 16) ? 1 : -1];
typedef char SecondaryStackSize[(sizeof(SecondaryStack) == 0x1008) ? 1 : -1];
typedef char SecondaryVmSize[(sizeof(SecondaryVm) == 0x1024) ? 1 : -1];
typedef char SecondaryVmTop[(offsetof(SecondaryVm, stack) + offsetof(SecondaryStack, top) == 0x1008) ? 1 : -1];
typedef char SecondaryVmOwner[(offsetof(SecondaryVm, owner) == 0x1014) ? 1 : -1];
typedef char SecondaryManagerMain[(offsetof(SecondaryManager, main) == 8) ? 1 : -1];
typedef char SecondaryManagerThreads[(offsetof(SecondaryManager, threads) == 0x1030) ? 1 : -1];
typedef char SecondaryManagerSize[(sizeof(SecondaryManager) == 0x103c) ? 1 : -1];
typedef char SecondaryProgramTable[(offsetof(SecondaryProgramView, subroutines) == 0x8c) ? 1 : -1];
typedef char SecondaryProgramTail[(offsetof(SecondaryProgramView, unknown1090) == 0x1090) ? 1 : -1];
typedef char SecondaryProgramSize[(sizeof(SecondaryProgramView) == 0x1098) ? 1 : -1];

void *__stdcall SecondaryStackAbsoluteAddress(SecondaryStack *self, int offset);
int __stdcall SecondaryStackTop(SecondaryStack *self);
SecondaryProgramView *__stdcall SecondaryScriptTable(SecondaryManager *self);
int __stdcall PushSecondaryValue(SecondaryStack *self, char type, const void *value);
int __stdcall PopSecondaryValue(SecondaryStack *self, char type, void *value);
int __stdcall BeginSecondaryStackFrame(SecondaryStack *self, int bytes);
int __stdcall EndSecondaryStackFrame(SecondaryStack *self);
int __stdcall ReadSecondaryIntReference(SecondaryVm *self, int reference);
float __stdcall ReadSecondaryFloatReference(SecondaryVm *self, float reference);
int __stdcall ReadSecondaryIntArgument(SecondaryVm *self, int index);
float __stdcall ReadSecondaryFloatArgument(SecondaryVm *self, int index);
int *__stdcall SecondaryIntDestination(SecondaryVm *self);
float *__stdcall SecondaryFloatDestination(SecondaryVm *self, int index);
SecondaryInstruction *__stdcall FindSecondarySubroutine(SecondaryProgramView *self, const char *name);
int __stdcall EnterSecondaryScriptFrame(SecondaryVm *self, SecondaryVm *caller, int skip);
int __stdcall RunSecondaryScript(SecondaryVm *self, float delta);
int __stdcall UpdateSecondaryScripts(SecondaryManager *self, float delta);
void __stdcall StartSecondaryThread(SecondaryManager *self, int id, int skip);
SecondaryThread *__stdcall FindSecondaryThread(SecondaryManager *self, int id);
void __stdcall StopSecondaryThreads(SecondaryManager *self);
