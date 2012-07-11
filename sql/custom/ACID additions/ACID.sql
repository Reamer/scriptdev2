-- Ahnkaheb
UPDATE creature_template SET AIName = "EventAI" WHERE entry = 30178;
DELETE FROM creature_ai_scripts WHERE creature_id = 30178;
INSERT INTO creature_ai_scripts (id, creature_id, event_type, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, comment) VALUES 
(3017801,30178,0,100,7,0,500,25000,40000,11,56354,0,2,"Ahn'kahar Swarmer - Cast Sprint");

-- Gundrak
DELETE FROM creature_ai_scripts WHERE creature_id = 29680;
INSERT INTO creature_ai_scripts VALUES 
(2968001, 29680, 0, 0, 100, 3, 0, 4000, 7000, 11000, 11, 54978, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Normal)'),
(2968002, 29680, 0, 0, 100, 5, 0, 4000, 7000, 11000, 11, 58996, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Heroic)');

-- ****************** CUSTOM ******************************++
-- Azjol Nerub
DELETE FROM creature_ai_scripts WHERE creature_id=105000;
INSERT INTO creature_ai_scripts (id, creature_id, event_type, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, comment) VALUES
(10500001, 105000, 0, 100, 2, 2500, 2700, 50000, 60000, 11, 53454, 0, 1, 'Impale trigger - Cast Impale (normal mode)'),
(10500002, 105000, 0, 100, 4, 2500, 2700, 50000, 60000, 11, 59446, 0, 1, 'Impale trigger - Cast Impale (heroic mode)'),
(10500003, 105000, 1, 100, 2, 2500, 2700, 50000, 60000, 11, 53454, 0, 1, 'Impale trigger - Cast Impale OOC (normal mode)'),
(10500004, 105000, 1, 100, 4, 2500, 2700, 50000, 60000, 11, 59446, 0, 1, 'Impale trigger - Cast Impale OOC (heroic mode)'),
(10500005, 105000, 1, 100, 6, 100, 150, 50000, 60000, 11, 42373, 0, 1, 'Impale trigger - Burrow OOC'),
(10500006, 105000, 0, 100, 6, 100, 150, 50000, 60000, 11, 42373, 0, 1, 'Impale trigger - Burrow'),
(10500007, 105000, 0, 100, 6, 100, 150, 0, 0, 20, 0, 0, 0, 'Impale trigger - Set no auto attack'),
(10500008, 105000, 0, 100, 6, 100, 150, 0, 0, 21, 0, 0, 0, 'Impale trigger - Set no combat movement');

-- Cyber Türsteher
update creature_template set AIName = 'EventAI' where entry in (670070);
REPLACE INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment) VALUES 
(67007001,670070,4,0,100,6,0,0,0,0,1,-71,0,0,11,55799,0,6,0,0,0,0,'Arthur - Yell and Frost Aura'),
(67007002,670070,9,0,100,7,0,20,1000,3000,0,0,0,0,11,53534,4,1,0,0,0,0,'Arthur - Yell and Ice Chains'),
(67007003,670070,0,0,100,7,5000,10000,10000,15000,1,-931,0,0,11,66019,1,4,0,0,0,0,'Arthur - Yell and Death Coil'),
(67007004,670070,0,0,100,7,25000,30000,25000,30000,1,-918,0,0,11,45657,0,4,0,0,0,0,'Arthur - Yell and Ultimate');
