-- instance
UPDATE instance_template SET ScriptName = 'instance_trial_of_the_crusader' WHERE map = 649;
-- ************** Twin Valkyr *************
UPDATE creature_template SET scriptname = 'boss_fjola', AIName = '' WHERE entry = 34497;
UPDATE creature_template SET scriptname = 'boss_eydis', AIName = '' WHERE entry = 34496;
UPDATE creature_template SET npcflag = 1, scriptname = 'npc_light_or_dark_essence',  AIName = '' WHERE entry IN (34568, 34567);
UPDATE creature_template SET AIName = '', scriptname = 'npc_light_or_dark_bullet' WHERE entry IN (34628, 34630);

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
UPDATE creature_template SET ScriptName = 'mob_frost_sphere' , AIName = '' WHERE entry = 34606;
UPDATE creature_template SET Scriptname = 'mob_swarm_scarab', AIName ='' WHERE entry = 34605;
UPDATE creature_template SET Scriptname = 'mob_nerubian_borrower', AIName = '' WHERE entry = 34607;
UPDATE creature_template SET Scriptname = 'mob_anubarak_spike', AIName = '' WHERE entry = 34660;

-- Nerubian Burrower - by Reamer
DELETE FROM spell_script_target WHERE entry IN (66332, 67755, 67756, 67757);
INSERT INTO spell_script_target VALUES
(66332, 1, 34862),
(67755, 1, 34862),
(67756, 1, 34862),
(67757, 1, 34862);

DELETE FROM spell_script_target WHERE entry = 66181;
INSERT INTO spell_script_target VALUES
{66181, 1, 34606);

UPDATE creature_template SET flags_extra = '2' WHERE entry = 34862; -- Nerubian Borrow

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

-- by Reamer
DELETE FROM spell_script_target WHERE entry = 66636;
INSERT INTO spell_script_target VALUES
(66636, 1, 34796);

-- ********************** Grand crusaders ******************
UPDATE creature_template SET scriptname = 'mob_toc_warrior', AIName = '' WHERE entry IN (34475,34453);
UPDATE creature_template SET scriptname = 'mob_toc_mage', AIName = '' WHERE entry IN (34468,34449);
UPDATE creature_template SET scriptname = 'mob_toc_shaman', AIName = '' WHERE entry IN (34463,34455);
UPDATE creature_template SET scriptname = 'mob_toc_enh_shaman', AIName = '' WHERE entry IN (34470,34444);
UPDATE creature_template SET scriptname = 'mob_toc_hunter', AIName = '' WHERE entry IN (34467,34448);
UPDATE creature_template SET scriptname = 'mob_toc_rogue', AIName = '' WHERE entry IN (34472,34454);
UPDATE creature_template SET scriptname = 'mob_toc_priest', AIName = '' WHERE entry IN (34466,34447);
UPDATE creature_template SET scriptname = 'mob_toc_shadow_priest', AIName = '' WHERE entry IN (34473,34441);
UPDATE creature_template SET scriptname = 'mob_toc_dk', AIName = '' WHERE entry IN (34461,34458);
UPDATE creature_template SET scriptname = 'mob_toc_paladin', AIName = '' WHERE entry IN (34465,34445);
UPDATE creature_template SET scriptname = 'mob_toc_retro_paladin', AIName = '' WHERE entry IN (34471,34456);
UPDATE creature_template SET scriptname = 'mob_toc_druid', AIName = '' WHERE entry IN (34460,34451);
UPDATE creature_template SET scriptname = 'mob_toc_boomkin', AIName = '' WHERE entry IN (34469,34459);
UPDATE creature_template SET scriptname = 'mob_toc_warlock' WHERE entry IN (34474,34450);
UPDATE creature_template SET scriptname = 'mob_toc_pet_warlock', AIName = '' WHERE entry IN (35465);
UPDATE creature_template SET scriptname = 'mob_toc_pet_hunter', AIName = '' WHERE entry IN (35610);
