-- instance
UPDATE instance_template SET ScriptName = 'instance_trial_of_the_crusader' WHERE map = 649;
DELETE FROM creature WHERE id IN (34704, 34720);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(3000100,34704, 649, 15, 1, 620.4653, 134.6597, 395.2328, 4.607669, 7200, 0, 0),
(3000101,34704, 649, 15, 1, 511.4167, 127.158, 395.2664, 4.607669, 7200, 0, 0),
(3000102,34704, 649, 15, 1, 611.6563, 114.2813, 395.2884, 4.607669, 7200, 0, 0),
(3000103,34704, 649, 15, 1, 573.5781, 187.6649, 395.4919, 4.607669, 7200, 0, 0),
(3000104,34704, 649, 15, 1, 568.0486, 187.6701, 395.5632, 4.607669, 7200, 0, 0),
(3000105,34704, 649, 15, 1, 518.5035, 170.6493, 395.2891, 4.607669, 7200, 0, 0),
(3000106,34704, 649, 15, 1, 563.467, 81.93229, 395.2878, 4.607669, 7200, 0, 0),
(3000107,34704, 649, 15, 1, 506.0382, 139.5174, 395.2882, 4.607669, 7200, 0, 0),
(3000108,34704, 649, 15, 1, 619.7708, 143.7101, 395.2442, 4.607669, 7200, 0, 0),
(3000109,34704, 649, 15, 1, 615.1371, 156.9965, 395.2795, 4.607669, 7200, 0, 0),
(3000110,34704, 649, 15, 1, 522.9549, 102.1267, 395.2899, 4.607669, 7200, 0, 0),
(3000111,34704, 649, 15, 1, 539.1788, 184.1319, 395.2818, 4.607669, 7200, 0, 0),
(3000112,34704, 649, 15, 1, 592.7361, 93.66666, 395.2889, 4.607669, 7200, 0, 0),
(3000113,34704, 649, 15, 1, 514.566, 159.9184, 395.287, 4.607669, 7200, 0, 0),
(3000114,34704, 649, 15, 1, 515.3524, 115.349, 395.2883, 4.607669, 7200, 0, 0),
(3000115,34704, 649, 15, 1, 549.7639, 86.44444, 395.2658, 4.607669, 7200, 0, 0),
(3000116,34704, 649, 15, 1, 605.5139, 103.8628, 395.2897, 4.607669, 7200, 0, 0),
(3000117,34704, 649, 15, 1, 577.2986, 186.8542, 395.289, 4.607669, 7200, 0, 0),
(3000118,34704, 649, 15, 1, 554.8177, 187.5677, 395.2875, 2.932153, 7200, 0, 0),
(3000119,34704, 649, 15, 1, 534.7483, 92.64236, 395.2892, 4.607669, 7200, 0, 0),
(3000120,34704, 649, 15, 1, 578.7222, 87.44444, 395.2724, 4.607669, 7200, 0, 0),
(3000121,34704, 649, 15, 1, 608.1163, 171.7309, 395.2892, 4.607669, 7200, 0, 0),
(3000122,34704, 649, 15, 1, 616.4323, 126.4184, 395.2641, 4.607669, 7200, 0, 0),
(3000123,34704, 649, 15, 1, 526.8333, 181.783, 395.2847, 4.607669, 7200, 0, 0),
(3000124,34704, 649, 15, 1, 560.4844, 187.7431, 395.9587, 2.932153, 7200, 0, 0),
(3000125,34704, 649, 15, 1, 597.3802, 183.6719, 395.2797, 4.607669, 7200, 0, 0),
(3000126,34704, 649, 15, 1, 547.7604, 184.6337, 395.2885, 2.932153, 7200, 0, 0),
(3000127,34704, 649, 15, 1, 586.3438, 184.0781, 395.2831, 4.607669, 7200, 0, 0),
(3000128,34704, 649, 15, 1, 509.7431, 149.0052, 395.2527, 4.607669, 7200, 0, 0),
(3000129,34720, 649, 15, 1, 610.9827, 164.6962, 395.2883, 0, 7200, 0, 0),
(3000130,34720, 649, 15, 1, 507.1806, 142.2847, 395.2546, 0, 7200, 0, 0),
(3000131,34720, 649, 15, 1, 526.3368, 99.55556, 395.29, 0, 7200, 0, 0),
(3000132,34720, 649, 15, 1, 538.0243, 92.44097, 395.2885, 0, 7200, 0, 0),
(3000133,34720, 649, 15, 1, 563.9965, 187.6441, 395.489, 0, 7200, 0, 0),
(3000134,34720, 649, 15, 1, 544.0938, 184.6476, 395.2856, 0, 7200, 0, 0),
(3000135,34720, 649, 15, 1, 546.0573, 88.7691, 395.2838, 0, 7200, 0, 0),
(3000136,34720, 649, 15, 1, 517.7222, 169.0694, 395.2889, 0, 7200, 0, 0),
(3000137,34720, 649, 15, 1, 588.6754, 93.09375, 395.2885, 0, 7200, 0, 0),
(3000138,34720, 649, 15, 1, 514.7847, 118.7309, 395.2871, 0, 7200, 0, 0),
(3000139,34720, 649, 15, 1, 599.3073, 98.80035, 395.2898, 0, 7200, 0, 0),
(3000140,34720, 649, 15, 1, 592.1511, 183.8004, 395.2789, 0, 7200, 0, 0),
(3000141,34720, 649, 15, 1, 581.243, 184.0625, 395.2871, 0, 7200, 0, 0),
(3000142,34720, 649, 15, 1, 615.4011, 130.816, 395.2631, 0, 7200, 0, 0),
(3000143,34720, 649, 15, 1, 511.132, 151.1563, 395.264, 0, 7200, 0, 0),
(3000144,34720, 649, 15, 1, 510.7587, 127.3333, 395.2625, 0, 7200, 0, 0),
(3000145,34720, 649, 15, 1, 577.757, 184.4358, 395.2889, 0, 7200, 0, 0),
(3000146,34720, 649, 15, 1, 603.5278, 175.4757, 395.2891, 0, 7200, 0, 0),
(3000147,34720, 649, 15, 1, 524.257, 178.1337, 395.2896, 0, 7200, 0, 0),
(3000148,34720, 649, 15, 1, 515.3993, 159.75, 395.2871, 0, 7200, 0, 0),
(3000149,34720, 649, 15, 1, 606.6858, 106.7309, 395.2894, 0, 7200, 0, 0),
(3000150,34720, 649, 15, 1, 520.2118, 108.4288, 395.2892, 0, 7200, 0, 0),
(3000151,34720, 649, 15, 1, 509.6389, 133.2604, 395.2467, 0, 7200, 0, 0),
(3000152,34720, 649, 15, 1, 535.9236, 184.2066, 395.2793, 0, 7200, 0, 0),
(3000153,34720, 649, 15, 1, 551.3281, 187.6458, 395.5959, 0, 7200, 0, 0),
(3000154,34720, 649, 15, 1, 557.743, 187.7292, 395.9153, 0, 7200, 0, 0),
(3000155,34720, 649, 15, 1, 612.118, 118.8438, 395.2871, 0, 7200, 0, 0),
(3000156,34720, 649, 15, 1, 570.7795, 86.29861, 395.2533, 0, 7200, 0, 0),
(3000157,34720, 649, 15, 1, 580.4861, 89.69097, 395.2867, 0, 7200, 0, 0),
(3000158,34720, 649, 15, 1, 571.158, 187.691, 395.6292, 0, 7200, 0, 0),
(3000159,34720, 649, 15, 1, 615.1371, 150.8177, 395.269, 0, 7200, 0, 0),
(3000160,34720, 649, 15, 1, 618.9653, 139.1736, 395.2881, 0, 7200, 0, 0),
(3000161,34720, 649, 15, 1, 555.6945, 86.02084, 395.2529, 0, 7200, 0, 0);

-- ************** Twin Valkyr *************
UPDATE creature_template SET scriptname = 'boss_fjola', AIName = '' WHERE entry = 34497;
UPDATE creature_template SET scriptname = 'boss_eydis', AIName = '' WHERE entry = 34496;
DELETE FROM creature_template_addon WHERE entry IN (34497, 34496);
UPDATE creature_template SET npcflag = 1, scriptname = 'npc_light_or_dark_essence',  AIName = '' WHERE entry IN (34568, 34567);
UPDATE creature_template SET AIName = '', scriptname = 'npc_light_or_dark_bullet' WHERE entry IN (34628, 34630);
UPDATE creature_template SET AIName ='NullAI' WHERE entry IN (34720, 34704); -- bullet Stalker

-- Twin Valkyrs
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34497', '65766'); -- Fjona light surge
REPLACE INTO `creature_template_addon` (entry, auras) VALUES ('34496', '65768'); -- Eydis dark surge

-- Twin pact by Wowka321
DELETE FROM spell_script_target WHERE entry IN (65875,67303,67304,67305,65876,67306,67307,67308);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES 
(65875, 1, 34497),
(67303, 1, 34497),
(67304, 1, 34497),
(67305, 1, 34497),
(65876, 1, 34496),
(67306, 1, 34496),
(67307, 1, 34496),
(67308, 1, 34496);
-- Twin empathy by Reamer
DELETE FROM spell_script_target WHERE entry IN (66132, 66133);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES
(66132, 1, 34496),
(66133, 1, 34497);
-- Instant kill by Reamer
DELETE FROM spell_script_target WHERE entry IN (68400, 68401);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES
(68400, 1, 34496),
(68401, 1, 34497);
-- Summon Light Bullet by Reamer
-- Spell from Fjola Lightbane to Val'kyr Twins Bullet Stalker Light
DELETE FROM spell_script_target WHERE entry IN (66140, 67158, 67159, 67160);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES
(66140, 1, 34720),
(67158, 1, 34720),
(67159, 1, 34720),
(67160, 1, 34720);
-- Summon Dark Bullet by Reamer
-- Spell from Eydis Darkbane to Val'kyr Twins Bullet Stalker Dark
DELETE FROM spell_script_target WHERE entry IN (66141, 67161, 67162, 67163);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES
(66141, 1, 34704),
(67161, 1, 34704),
(67162, 1, 34704),
(67163, 1, 34704);



-- ************* Anub'arak ****************
UPDATE creature_template SET Scriptname = 'boss_anubarak_trial', /*unit_flags = 0,*/ AIName = '' WHERE entry = 34564;
UPDATE creature_template SET Type = 4, InhabitType = 7, ScriptName = 'mob_frost_sphere' , AIName = '' WHERE entry = 34606;
UPDATE creature_template SET Scriptname = 'mob_swarm_scarab', AIName ='' WHERE entry = 34605;
UPDATE creature_template SET Scriptname = 'mob_nerubian_borrower', AIName = '' WHERE entry = 34607;
UPDATE creature_template SET modelid_1 = 17612, modelid_2 = 17612, Scriptname = 'mob_anubarak_spike', AIName = '' WHERE entry = 34660;
UPDATE creature_template SET AIName ='NullAI' WHERE entry = 34862; -- Nerubian Borrow

-- Nerubian Burrower - by Reamer
DELETE FROM spell_script_target WHERE entry IN (66332, 67755, 67756, 67757);
INSERT INTO spell_script_target VALUES
(66332, 1, 34862),
(67755, 1, 34862),
(67756, 1, 34862),
(67757, 1, 34862);

-- Spider Frenzy from Reamer
DELETE FROM spell_script_target WHERE entry = 66129;
INSERT INTO spell_script_target VALUES
(66129, 1, 34607);

-- Anub'arak craeture linking from sd2
delete from creature_linking_template where entry in (34605,34606,34607,34660,34862);
insert into creature_linking_template values
(34605,649,34564,4112,0),
(34606,649,34564,4112,0),
(34607,649,34564,4112,0),
(34660,649,34564,4112,0),
(34862,649,34564,4112,0);
-- script targets from sd2
delete from spell_script_target where entry IN (65872,66332,66339,66181,66170);
insert into spell_script_target values
(65872,1,34660),
(66332,1,34862),
(66339,1,34862),
(66181,1,34606),
(66170,1,34660);

-- ************** Jaraxxus ***********************
UPDATE creature_template SET scriptname = 'boss_jaraxxus', AIName = '' WHERE entry= 34780;
UPDATE creature_template SET scriptname = 'mob_legion_flame', minlevel = 82, maxlevel = 82, modelid_1 = 11686, modelid_2 = 11686, modelid_3 = 11686, modelid_4 = 11686, AIName = 'NullAI', faction_A = 14, faction_H = 14 WHERE entry = 34784;
UPDATE creature_template SET scriptname = '', AIName ='NullAI' WHERE entry IN (34813, 34825); -- vulcano and nether portal
UPDATE creature_template SET scriptname = 'mob_fel_infernal', AIName ='' WHERE entry = 34815;
UPDATE creature_template SET scriptname = 'mob_mistress_of_pain', AIName ='' WHERE entry = 34826;

DELETE FROM spell_script_target WHERE entry = 66287;
INSERT INTO spell_script_target VALUES
(66287, 1, 34826); -- source Sniff

-- Felflame infernal
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34815;
DELETE FROM `creature_ai_scripts` WHERE `id`=3481501;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3481501,34815,0,0,100,31,7000,10000,8000,10000,11,67047,0,0,0,0,0,0,0,0,0,0, 'Felflame infernal - cast Fel Inferno');
DELETE FROM `creature_ai_scripts` WHERE `id`=3481502;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3481502,34815,0,0,100,31,7000,10000,8000,10000,11,66519,4,0,0,0,0,0,0,0,0,0, 'Felflame infernal - cast Fel Steak');
-- Mistress of pain
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34826;
DELETE FROM `creature_ai_scripts` WHERE `id`=3482601;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482601,34826,0,0,100,31,7000,10000,5000,8000,11,66378,0,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Shivan slash');
DELETE FROM `creature_ai_scripts` WHERE `id`=3482602;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482602,34826,0,0,100,31,7000,10000,5000,8000,11,66283,4,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Spinning pain spike');
DELETE FROM `creature_ai_scripts` WHERE `id`=3482603;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3482603,34826,0,0,100,25,13000,15000,12000,16000,11,66425,0,0,0,0,0,0,0,0,0,0, 'Mistress of pain - cast Mistress Kiss');
-- Legion flame
UPDATE `creature_template` SET `AIName`='EventAI' WHERE `entry`=34784;
DELETE FROM `creature_ai_scripts` WHERE `id`=3478401;
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
(3478401,34784,11,0,100,30,0,0,0,0,11,66201,0,0,0,0,0,0,0,0,0,0, 'Legion Flame - cast Legion Flame');

-- ************* Northend Beast *********************

-- Nortrend beasts
UPDATE creature_template SET scriptname = 'boss_gormok', AIName ='' WHERE entry = 34796;
UPDATE creature_template SET scriptname = 'mob_snobold_vassal', AIName ='' WHERE entry = 34800;
UPDATE creature_template SET scriptname = 'boss_acidmaw_and_dreadscale', AIName ='' WHERE entry IN (34799, 35144);
UPDATE creature_template SET scriptname = 'boss_icehowl', AIName = '' WHERE entry =34797;
UPDATE creature_template SET ScriptName = '' , AIName ='NullAI' WHERE entry IN (34854, 35176); -- Firebomb and Slime Pool

-- Rising Anger by Reamer
DELETE FROM spell_script_target WHERE entry = 66636;
INSERT INTO spell_script_target VALUES
(66636, 1, 34796);

DELETE FROM vehicle_accessory WHERE entry = 34796;
INSERT INTO vehicle_accessory (entry, accessory_entry, seat_id, minion, description) VALUES
(34796, 34800, 0, 0, 'Gormok Snobold Vassal'),
(34796, 34800, 1, 0, 'Gormok Snobold Vassal'),
(34796, 34800, 2, 0, 'Gormok Snobold Vassal'),
(34796, 34800, 3, 0, 'Gormok Snobold Vassal'),
(34796, 34800, 4, 0, 'Gormok Snobold Vassal');

-- ********************** Grand crusaders ******************
UPDATE creature_template SET scriptname = 'mob_toc_warrior', AIName = '' WHERE entry IN (34475,34453);
UPDATE creature_template SET scriptname = 'mob_toc_mage', AIName = '' WHERE entry IN (34468,34449);
UPDATE creature_template SET scriptname = 'mob_toc_shaman', AIName = '' WHERE entry IN (34470,34444);
UPDATE creature_template SET scriptname = 'mob_toc_enh_shaman', AIName = '' WHERE entry IN (34455,34463);
UPDATE creature_template SET scriptname = 'mob_toc_hunter', AIName = '' WHERE entry IN (34467,34448);
UPDATE creature_template SET scriptname = 'mob_toc_rogue', AIName = '' WHERE entry IN (34472,34454);
UPDATE creature_template SET scriptname = 'mob_toc_priest', AIName = '' WHERE entry IN (34466,34447);
UPDATE creature_template SET scriptname = 'mob_toc_shadow_priest', AIName = '' WHERE entry IN (34473,34441);
UPDATE creature_template SET scriptname = 'mob_toc_dk', AIName = '' WHERE entry IN (34461,34458);
UPDATE creature_template SET scriptname = 'mob_toc_paladin', AIName = '' WHERE entry IN (34465,34445);
UPDATE creature_template SET scriptname = 'mob_toc_retro_paladin', AIName = '' WHERE entry IN (34471,34456);
UPDATE creature_template SET scriptname = 'mob_toc_druid', AIName = '' WHERE entry IN (34469,34459);
UPDATE creature_template SET scriptname = 'mob_toc_moomkin', AIName = '' WHERE entry IN (34460,34451);
UPDATE creature_template SET scriptname = 'mob_toc_warlock' WHERE entry IN (34474,34450);
UPDATE creature_template SET scriptname = 'mob_toc_pet_warlock', AIName = '' WHERE entry IN (35465);
UPDATE creature_template SET scriptname = 'mob_toc_pet_hunter', AIName = '' WHERE entry IN (35610);

-- ****************************** SPELL_HACKS ****************************
DELETE FROM `spell_dbc` WHERE ID=100020;
INSERT INTO `spell_dbc` (`Id`, `Category`, `Dispel`, `Mechanic`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `Stances`, `StancesNot`, `Targets`, `RequiresSpellFocus`, `CasterAuraState`, `TargetAuraState`, `CasterAuraStateNot`, `TargetAuraStateNot`, `CasterAuraSpell`, `TargetAuraSpell`, `ExcludeCasterAuraSpell`, `ExcludeTargetAuraSpell`, `CastingTimeIndex`, `RecoveryTime`, `CategoryRecoveryTime`, `InterruptFlags`, `AuraInterruptFlags`, `ProcFlags`, `ProcChance`, `ProcCharges`, `MaxLevel`, `BaseLevel`, `SpellLevel`, `DurationIndex`, `PowerType`, `ManaCost`, `ManaCostPerLevel`, `ManaPerSecond`, `ManaPerSecondPerLevel`, `RangeIndex`, `Speed`, `StackAmount`, `EquippedItemClass`, `EquippedItemSubClassMask`, `EquippedItemInventoryTypeMask`, `Effect1`, `Effect2`, `Effect3`, `EffectDieSides1`, `EffectDieSides2`, `EffectDieSides3`, `EffectRealPointsPerLevel1`, `EffectRealPointsPerLevel2`, `EffectRealPointsPerLevel3`, `EffectBasePoints1`, `EffectBasePoints2`, `EffectBasePoints3`, `EffectMechanic1`, `EffectMechanic2`, `EffectMechanic3`, `EffectImplicitTargetA1`, `EffectImplicitTargetA2`, `EffectImplicitTargetA3`, `EffectImplicitTargetB1`, `EffectImplicitTargetB2`, `EffectImplicitTargetB3`, `EffectRadiusIndex1`, `EffectRadiusIndex2`, `EffectRadiusIndex3`, `EffectApplyAuraName1`, `EffectApplyAuraName2`, `EffectApplyAuraName3`, `EffectAmplitude1`, `EffectAmplitude2`, `EffectAmplitude3`, `EffectMultipleValue1`, `EffectMultipleValue2`, `EffectMultipleValue3`, `EffectItemType1`, `EffectItemType2`, `EffectMiscValue1`, `EffectMiscValue2`, `EffectMiscValue3`, `EffectMiscValueB1`, `EffectMiscValueB2`, `EffectMiscValueB3`, `EffectTriggerSpell1`, `EffectTriggerSpell2`, `EffectTriggerSpell3`, `EffectSpellClassMaskA1`, `EffectSpellClassMaskA2`, `EffectSpellClassMaskA3`, `EffectSpellClassMaskB1`, `EffectSpellClassMaskB2`, `EffectSpellClassMaskB3`, `EffectSpellClassMaskC1`, `EffectSpellClassMaskC2`, `EffectSpellClassMaskC3`, `SpellIconID`, `ManaCostPercentage`, `StartRecoveryCategory`, `StartRecoveryTime`, `MaxTargetLevel`, `SpellFamilyName`, `SpellFamilyFlags1`, `SpellFamilyFlags2`, `SpellFamilyFlags3`, `MaxAffectedTargets`, `DmgClass`, `PreventionType`, `DmgMultiplier1`, `DmgMultiplier2`, `DmgMultiplier3`, `AreaGroupId`, `SchoolMask`, `RuneCostID`, `Comment`)
VALUES (                 100020 , 0 ,          0 ,         0 ,   0x20000100 ,   0x10000088,          0x4 ,            0x100 ,        0 ,                 0,            0,                0,             0 ,             0 ,         0 ,             0 ,                     0 ,               0 ,             0 ,                 0 ,                    0 ,                 0 ,                 0 ,                     0 ,                         1 ,                 0 ,                     0 ,           0 ,                 0 ,              0 ,          101 ,         0 ,             0 ,       0 ,             0 ,          21 ,            0 ,             0 ,             0 ,                 0 ,               0 ,             13 ,        0 ,         0 ,                 -1 ,                     0 ,                     0 ,                        129 ,         0 ,      0 ,             1 ,                 0 ,                 0 ,                 0 ,                     0 ,                             0 ,                         -1 ,                 0 ,                 0 ,                     0 ,                 0 ,                 0 ,                 1 ,                    0 ,                     0 ,                     0                 ,         0 ,                 0 ,                         28 ,                     0 ,                 0 ,                         296  ,                     0 ,                 0 ,                   0 ,                 0 ,                 0 ,                 0 ,                         0 ,                 0 ,                     0 ,                 0 ,                 443 ,                 0 ,                 0 ,                  0 ,              0 ,                 0 ,                 0 ,                     0 ,                 0 ,                     0 ,                         0 ,                     0 ,                         0 ,                     0 ,                     0 ,                         0 ,                     0 ,                     0 ,                     276 ,            0,                       0 ,                     0 ,                     0 ,                 0 ,                 0 ,                 0 ,                 0 ,                 0 ,               0 ,             1 ,              1 ,             1 ,                 1 ,             0 ,             1 ,          0 ,    'Snobold Vassal Vehicle Aura - should cast from Gormok the Impaler at aggro');

