#pragma once

#include "MainRuntime.hpp"

struct ExecutableChecksumView
{
    unsigned int checksum;
    unsigned int size;
};

unsigned int __cdecl ComputeExecutableChecksum();
