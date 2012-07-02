-- some spells for vortex ride
DELETE FROM spell_script_target WHERE spell_script_target.targetEntry = 30090;
INSERT INTO spell_script_target VALUES
(55853, 1, 30090),  -- nonhero
(56263, 1, 30090),
(56264, 1, 30090),
(56265, 1, 30090),
(56266, 1, 30090),
(61071, 1, 30090),--  hero
(61072, 1, 30090),
(61073, 1, 30090),
(61074, 1, 30090),
(61075, 1, 30090);

-- find right target
DELETE FROM spell_script_target WHERE entry = 56429;
INSERT INTO spell_script_target VALUES (56429, 1, 22517);

DELETE FROM spell_script_target WHERE entry = 56140;
INSERT INTO spell_script_target VALUES (56140, 1, 30118);

-- no AI for Vortex and Malygos Portal
UPDATE creature_template SET InhabitType = 7, AIName ='NullAI' WHERE entry IN (30090, 30118);

-- Eye of Eternity from Tassadar && bwsrv
UPDATE `instance_template` SET `ScriptName`='instance_eye_of_eternity' WHERE map=616;
UPDATE `gameobject_template` SET `ScriptName`='go_focusing_iris' WHERE entry IN (193958, 193960);
UPDATE `creature_template` SET `ScriptName`='boss_malygos' WHERE entry=28859;
UPDATE `creature_template` SET  InhabitType = 7,`ScriptName`='npc_power_spark' WHERE entry=30084;
UPDATE `creature_template` SET `ScriptName`='npc_nexus_lord' WHERE entry=30245;
UPDATE `creature_template` SET `ScriptName`='npc_scion_of_eternity' WHERE entry=30249;
UPDATE `creature_template` SET `ScriptName`='npc_hover_disk' WHERE entry IN (30234, 30248);
UPDATE `creature_template` SET `PowerType` = 3, `InhabitType` = 7, `ScriptName`='npc_whyrmrest_skytalon' WHERE entry=30161;
UPDATE `creature_template` SET `PowerType` = 3, `InhabitType` = 7 WHERE entry=31752;
UPDATE `creature_template` SET `InhabitType` = 7 WHERE entry=30592; --  static field
UPDATE `creature_template` SET `ScriptName`='npc_alexstrasza' WHERE entry=32295;

-- Two Hover Disks
DELETE FROM `npc_spellclick_spells` WHERE npc_entry IN (30234, 30248);
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, quest_start, quest_start_active, quest_end, cast_flags) VALUES
(30234, 48754, 0, 0, 0, 1),
(30248, 48754, 0, 0, 0, 1);

-- Wyrmrest Skytalon
UPDATE `creature_template` SET Spell1=56091, Spell2=56092, Spell3=57090, Spell4=57143, Spell5=57108, Spell6=57092 WHERE entry IN (30161, 31752);
REPLACE INTO `creature_template_addon` VALUES (30161,0,0,0,0,0,0,'57403');
REPLACE INTO `creature_template_addon` VALUES (31752,0,0,0,0,0,0,'57403');

-- spawn Alexstrasza's Gift and Heart of Magic
DELETE FROM `gameobject` WHERE id IN (193905, 193967, 194158, 194159);
INSERT INTO `gameobject` (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(193905, 616, 1, 1, 754.544, 1301.71, 220.083, 0, 0, 0, 0, 0, -604800, 100, 1),
(193967, 616, 2, 1, 754.544, 1301.71, 220.083, 0, 0, 0, 0, 0, -604800, 100, 1),
(194158, 616, 1, 1, 759.544, 1306.71, 225.083, 0, 0, 0, 0, 0, -604800, 100, 1),
(194159, 616, 2, 1, 759.544, 1306.71, 225.083, 0, 0, 0, 0, 0, -604800, 100, 1);


-- Hack for Portalbeam - TODO: need fix in Core with ChannelCast.cancel()
DELETE FROM `spell_dbc` WHERE ID = 56046;
INSERT INTO `spell_dbc` (`Id`, `Category`, `Dispel`, `Mechanic`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `Stances`, `StancesNot`, `Targets`, `RequiresSpellFocus`, `CasterAuraState`, `TargetAuraState`, `CasterAuraStateNot`, `TargetAuraStateNot`, `CasterAuraSpell`, `TargetAuraSpell`, `ExcludeCasterAuraSpell`, `ExcludeTargetAuraSpell`, `CastingTimeIndex`, `RecoveryTime`, `CategoryRecoveryTime`, `InterruptFlags`, `AuraInterruptFlags`, `ProcFlags`, `ProcChance`, `ProcCharges`, `MaxLevel`, `BaseLevel`, `SpellLevel`, `DurationIndex`, `PowerType`, `ManaCost`, `ManaCostPerLevel`, `ManaPerSecond`, `ManaPerSecondPerLevel`, `RangeIndex`, `Speed`, `StackAmount`, `EquippedItemClass`, `EquippedItemSubClassMask`, `EquippedItemInventoryTypeMask`, `Effect1`, `Effect2`, `Effect3`, `EffectDieSides1`, `EffectDieSides2`, `EffectDieSides3`, `EffectRealPointsPerLevel1`, `EffectRealPointsPerLevel2`, `EffectRealPointsPerLevel3`, `EffectBasePoints1`, `EffectBasePoints2`, `EffectBasePoints3`, `EffectMechanic1`, `EffectMechanic2`, `EffectMechanic3`, `EffectImplicitTargetA1`, `EffectImplicitTargetA2`, `EffectImplicitTargetA3`, `EffectImplicitTargetB1`, `EffectImplicitTargetB2`, `EffectImplicitTargetB3`, `EffectRadiusIndex1`, `EffectRadiusIndex2`, `EffectRadiusIndex3`, `EffectApplyAuraName1`, `EffectApplyAuraName2`, `EffectApplyAuraName3`, `EffectAmplitude1`, `EffectAmplitude2`, `EffectAmplitude3`, `EffectMultipleValue1`, `EffectMultipleValue2`, `EffectMultipleValue3`, `EffectItemType1`, `EffectItemType2`, `EffectMiscValue1`, `EffectMiscValue2`, `EffectMiscValue3`, `EffectMiscValueB1`, `EffectMiscValueB2`, `EffectMiscValueB3`, `EffectTriggerSpell1`, `EffectTriggerSpell2`, `EffectTriggerSpell3`, `EffectSpellClassMaskA1`, `EffectSpellClassMaskA2`, `EffectSpellClassMaskA3`, `EffectSpellClassMaskB1`, `EffectSpellClassMaskB2`, `EffectSpellClassMaskB3`, `EffectSpellClassMaskC1`, `EffectSpellClassMaskC2`, `EffectSpellClassMaskC3`, `SpellIconID`, `ManaCostPercentage`, `StartRecoveryCategory`, `StartRecoveryTime`, `MaxTargetLevel`, `SpellFamilyName`, `SpellFamilyFlags1`, `SpellFamilyFlags2`, `SpellFamilyFlags3`, `MaxAffectedTargets`, `DmgClass`, `PreventionType`, `DmgMultiplier1`, `DmgMultiplier2`, `DmgMultiplier3`, `AreaGroupId`, `SchoolMask`, `RuneCostID`, `Comment`)
VALUES ( 56046 , 0 , 0 , 0 , 256 , 268435460 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 101 , 0 , 0 , 0 , 0 , 21 , 0 , 0 , 0 , 0 , 0 , 13 , 0 , 0 , -1 , 0 , 0 , 6 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 25 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 0 , 1 , 0 ,'Portalstrahl');

UPDATE `spell_dbc` SET
`DurationIndex`=8, -- old value = 21
`Comment` = 'Replacement for spell 56046 "Portalstrahl"'
WHERE Id=56046;
