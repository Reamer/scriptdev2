-- ---------
-- Instance
-- ---------

UPDATE instance_template SET ScriptName='instance_icecrown_citadel' WHERE map=631;
DELETE FROM scripted_event_id WHERE id IN (23426,23438);
INSERT INTO scripted_event_id VALUES
(23426,'event_gameobject_citadel_valve'),
(23438,'event_gameobject_citadel_valve');

DELETE FROM scripted_areatrigger WHERE entry IN (5709,5732, 5604);
INSERT INTO scripted_areatrigger VALUES
(5709,'at_icecrown_citadel'),
(5732,'at_icecrown_citadel'),
(5604,'at_icecrown_citadel');

-- make teleports always usable
-- TODO: implement "discovering" teleports
UPDATE `gameobject_template` SET `flags` = 0, `ScriptName` = 'go_icecrown_teleporter' WHERE `entry`  IN (202242,202243,202244,202245,202235,202223,202246);


-- ---------
-- Marrowgar
-- ---------

UPDATE creature_template SET ScriptName='boss_lord_marrowgar' WHERE entry=36612;
UPDATE creature_template SET ScriptName='npc_bone_spike' WHERE entry=36619;

-- ---------
-- Lady Deathwhisper
-- ---------
UPDATE creature_template SET ScriptName='boss_lady_deathwhisper' WHERE entry=36855;

-- ---------
-- Deathbringer Saurfang
-- ---------
UPDATE creature_template SET ScriptName='boss_deathbringer_saurfang' WHERE entry=37813;
-- DELETE Muradin and Soldiers before Saurfang encounter, this NPC's are spawn in Instance Script, because different handling for Horde Ally
DELETE FROM creature WHERE id IN (37200, 37830) AND map=631;
-- This Creatures are scripted in instance script and should do nothing
UPDATE creature_template SET AIName = 'NullAI' WHERE entry IN (37200, 37830, 37187, 37920, 37879, 37188);

-- ---------
-- Queen Lanathel Intro
-- ---------
UPDATE creature_template SET ScriptName='npc_queen_lanathel_intro' WHERE entry=38004;
UPDATE creature_template SET ScriptName='boss_blood_queen_lanathel' WHERE entry=37955;
