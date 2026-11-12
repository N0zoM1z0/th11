#pragma once

#include "Chain.hpp"
#include "AnimationDocument.hpp"
#include <stddef.h>

// The native 0x40E0A0/0x40E190/0x40E240 object is a distinct 0x24-byte
// auxiliary owner, not the replay file browser at 0x4A8EC0.
struct SecondaryAuxiliaryOwnerView
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    AnimationDocumentView *document;
    unsigned int unknown014, unknown018, unknown01c, unknown020;

    SecondaryAuxiliaryOwnerView()
    {
        unsigned int *words = reinterpret_cast<unsigned int *>(this);
        for (int i = 0; i < 9; ++i) words[i] = 0;
    }
};
typedef char AuxiliaryOwnerSize[(sizeof(SecondaryAuxiliaryOwnerView) == 0x24) ? 1 : -1];
typedef char AuxiliaryDocumentOffset[(offsetof(SecondaryAuxiliaryOwnerView, document) == 0x10) ? 1 : -1];

extern SecondaryAuxiliaryOwnerView *g_SecondaryAuxiliaryOwner;
int __stdcall InitializeSecondaryAuxiliaryOwner(SecondaryAuxiliaryOwnerView *self);
void __fastcall DestroySecondaryAuxiliaryOwner(SecondaryAuxiliaryOwnerView *self);
SecondaryAuxiliaryOwnerView *__cdecl CreateSecondaryAuxiliaryOwner();
