-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_forgemaster_garfrost', `AIName` ='' WHERE `entry`=36494;
UPDATE `creature_template` SET `ScriptName`='boss_ick' WHERE `entry`=36476;
UPDATE `creature_template` SET `ScriptName`='boss_krick' WHERE `entry`=36477;
UPDATE `creature_template` SET `ScriptName`='boss_rimefang' WHERE `entry`=36661;
UPDATE `creature_template` SET `ScriptName`='boss_scourgelord_tyrannus', `AIName` ='' WHERE `entry`=36658;
-- Dungeon Mobs
UPDATE `creature_template` SET `ScriptName`='npc_icicle_pit_of_saron' WHERE `entry`=36847;

-- DB Correction
DELETE FROM creature_template_addon WHERE entry = 36477; -- delete Krick auras

-- Add auras for Exploding Orb
DELETE FROM creature_template_addon WHERE entry = 36610;
INSERT INTO creature_template_addon (entry, auras) VALUES 
(36610, '69017 69020');

-- trigger
DELETE FROM scripted_areatrigger WHERE entry IN (5580, 5581, 5598, 5599);
INSERT INTO scripted_areatrigger VALUES
(5580, 'at_pit_of_saron'),
(5581, 'at_pit_of_saron'),
(5598, 'at_pit_of_saron'),
(5599, 'at_pit_of_saron');

-- Spells:
DELETE FROM spell_script_target WHERE entry = 69753 ;
INSERT INTO spell_script_target VALUES
(69753, 1, 37584),
(69753, 1, 37587),
(69753, 1, 37588),
(69753, 1, 37496),
(69753, 1, 37497),
(69753, 1, 37498);

DELETE FROM spell_script_target WHERE entry = 70521;
INSERT INTO spell_script_target VALUES
(70521, 1 , 37578),
(70521, 1 , 37577),
(70521, 1 , 37579),
(70521, 1 , 37572),
(70521, 1 , 37575),
(70521, 1 , 37576),
(70521, 1 , 37591),
(70521, 1 , 37580),
(70521, 1 , 37592),
(70521, 1 , 37581);
-- ACID:
-- Exploding Orb
UPDATE creature_template SET Scriptname = '', AIName ='EventAI' WHERE entry=36610;
DELETE FROM creature_ai_scripts WHERE creature_id=36610;
INSERT INTO creature_ai_scripts VALUES 
('3661001','36610','11','0','100','7','0','0','0','0','21','0','1','0','20','0','0','0','11','69017','0','0','Disable CombatMovement, Autohits'),
('3661002','36610','29','0','100','6','5000','6000','0','0','11','69019','0','0','41','1000','0','0','0','0','0','0','Cast Explosive Barrage');

-- Quest for slaves
DELETE FROM dbscripts_on_go_template_use WHERE dbscripts_on_go_template_use.id = 202168;
INSERT INTO dbscripts_on_go_template_use (id, delay, command, datalong, datalong2, comments) VALUES
(202168, 1, 8, 36764, 1, 'killcredit alliance'),
(202168, 1, 8, 36770, 1, 'killcredit horde');
UPDATE gameobject_template SET data2 = 202168 WHERE entry IN (202168);


-- mixed data
-- add vehicleid for rimefang (h)
UPDATE creature_template SET vehicle_id = 535 WHERE entry = 38220;

-- DELETE at begin (Jaina and soldiers)
DELETE FROM creature WHERE map = 658 AND id IN
(36993, 37774, 37582, 37498, 37496 , 37497);
