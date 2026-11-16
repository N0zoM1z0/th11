#pragma once

// The current primary and secondary stage transition helpers have no explicit
// parameters. Their stage pointers come from the two global stage slots.
void __cdecl BeginSecondaryStageTransition();
void __cdecl BeginPrimaryStageTransition();
