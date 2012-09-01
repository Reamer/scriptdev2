-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_ick' WHERE `entry`=36476;
UPDATE `creature_template` SET `ScriptName`='boss_krick' WHERE `entry`=36477;
-- UPDATE `creature_template` SET `ScriptName`='boss_rimefang' WHERE `entry`=36661;
-- UPDATE `creature_template` SET `ScriptName`='npc_colapsing_icicle' WHERE `entry`=36847;
-- UPDATE `creature_template` SET `ScriptName`='boss_scourgelord_tyrannus', `AIName` ='' WHERE `entry`=36658;
UPDATE `creature_template` SET `ScriptName`='boss_forgemaster_garfrost', `AIName` ='' WHERE `entry`=36494;


-- npc
-- UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_start' WHERE `entry` in (36993, 36990);
-- UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_end' WHERE `entry` in (38188, 38189);
-- UPDATE `creature_template` SET `ScriptName`='npc_martin_gorkun' WHERE `entry` in (37591, 37592);


-- DB Correction
DELETE FROM creature_template_addon WHERE entry = 36477; -- delete Krick auras

-- trigger
DELETE FROM scripted_areatrigger WHERE entry IN (5580, 5581)
INSERT INTO scripted_areatrigger VALUES
(5580, at_pit_of_saron),
(5581, at_pit_of_saron);
(5598, at_pit_of_saron);
(5599, at_pit_of_saron);

-- Spells:

DELETE FROM spell_script_target WHERE entry = 69753 ;
INSERT INTO spell_script_target VALUES
(69753, 1, 37584),
(69753, 1, 37587),
(69753, 1, 37588),
(69753, 1, 37496),
(69753, 1, 37497),
(69753, 1, 37498);




-- ACID:
-- Exploding Orb
UPDATE creature_template SET Scriptname = '', AIName ='EventAI' WHERE entry=36610;
DELETE FROM creature_ai_scripts WHERE creature_id=36610;
INSERT INTO creature_ai_scripts VALUES 
('3661001','36610','11','0','100','6','0','0','0','0','21','0','1','0','20','0','0','0','11','69017','0','0','Disable CombatMovement, Autohits and cast dummy visual aura'),
('3661002','36610','0','0','100','2','3000','3100','0','0','11','69019','0','0','0','0','0','0','0','0','0','0','Cast Explosive Barrage - nonhero'),
('3661003','36610','0','0','100','4','3000','3100','0','0','11','70433','0','0','0','0','0','0','0','0','0','0','Cast Explosive Barrage - hero');