        {
            PrepareSecondaryDialogueStageClear();
            MarkSecondaryDialoguePlayerClear();
            SecondaryDialogueSessionView *session = g_SecondaryDialogueSession;
            SecondaryCharacterStatistics *records =
                reinterpret_cast<SecondaryCharacterStatistics *>(g_SecondaryGameplayStatistics);
            if (session->mode == 0 && g_SecondaryActorDifficulty != 4)
            {
                records[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                    .stages[g_SecondaryActorDifficulty * 6 + g_StageVariant].cleared = 1;
                records[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                    .stages[g_SecondaryActorDifficulty * 6 + g_StageVariant].reached = 1;
            }
            if (g_StageResourceFlags & 0x10)
                EnterSecondaryStageClearMenu();
            else
            {
                SecondaryControllerModeView *mode = g_SecondaryControllerMode;
                if (session->mode != 0 &&
                    ((*reinterpret_cast<unsigned char **>(reinterpret_cast<unsigned char *>(mode) + 0x18))[10] & 1))
                {
secondary_dialogue_continue:
                    EnterSecondaryContinueMenu();
                }
                else if (g_StageVariant == 6)
                {
                    SecondaryGameplayHudView *hud = Hud();
                    hud->flags |= 0x20;
                    AddSecondaryActorScore(Multiply(SecondaryDialogueClearValue(&g_SecondaryHostSelectionState), 1000));
                    int bonus = 0;
                    switch (g_SecondaryActorDifficulty)
                    {
                    case 0:
                        bonus = Multiply(AddInteger(Multiply(StateInteger(0x38), 200), StateInteger(8)), 100000);
                        break;
                    case 1:
                        bonus = AddInteger(Multiply(StateInteger(0x38), 25000000), Multiply(StateInteger(8), 150000));
                        break;
                    case 2:
                        bonus = Multiply(AddInteger(Multiply(StateInteger(0x38), 175), StateInteger(8)), 200000);
                        break;
                    case 3:
                        bonus = AddInteger(Multiply(StateInteger(0x38), 40000000), Multiply(StateInteger(8), 300000));
                        break;
                    case 4:
                        bonus = Multiply(AddInteger(Multiply(StateInteger(0x38), 100), StateInteger(8)), 400000);
                        break;
                    }
                    AddSecondaryActorScore(bonus);
                    HudInteger(hud, 0x4450) = AddInteger(HudInteger(hud, 0x4450), bonus);
                    if (g_SecondaryControllerMode->mode == 1)
                        goto secondary_dialogue_continue;
                    else
                    {
                        hud->flags |= 0x10;
                        HudInteger(hud, 0x4454) = 0;
                        if (records[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                                .clearCounts[g_SecondaryActorDifficulty] < 99999)
                            ++records[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                                .clearCounts[g_SecondaryActorDifficulty];
                    }
                }
                else if (g_StageVariant == 7)
                {
                    Hud()->flags |= 0x20;
                    AddSecondaryActorScore(Multiply(SecondaryDialogueClearValue(&g_SecondaryHostSelectionState), 1000));
                    AddSecondaryActorScore(Multiply(StateInteger(0x38), 40000000));
                    AddSecondaryActorScore(Multiply(StateInteger(8), 400000));
                    if (mode->mode == 1)
                        goto secondary_dialogue_continue;
                    else
                    {
                        EnterSecondaryStageClearMenu();
                        SecondaryCharacterStatistics *all =
                            reinterpret_cast<SecondaryCharacterStatistics *>(g_SecondaryGameplayStatistics);
                        if (all[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                                .clearCounts[g_SecondaryActorDifficulty] < 99999)
                            ++all[g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1]
                                .clearCounts[g_SecondaryActorDifficulty];
                    }
                }
                else
                {
                    SetSecondaryGameplayRuntimeMode(&g_MainRuntime, 12);
                    AdvanceSecondaryDialogueStage();
                }
            }
        }
