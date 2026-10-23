#pragma once

#include "MainRuntime.hpp"

extern int g_StartupReturnGate;
extern const char g_ShortcutExtension[];

int __stdcall ReadShortcutTarget(const char *filename, char *destination);
int __cdecl CheckStartupPath();
