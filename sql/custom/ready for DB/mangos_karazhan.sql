-- Attumen
UPDATE creature_template SET Scriptname = 'boss_attumen' WHERE entry = 16152;

-- Moroes Guest
UPDATE creature_template Set MovementType = 0 WHERE entry IN (17007, 19872, 19873, 19874, 19875, 19876);

-- Terestian Illhoof
UPDATE creature_template SET AIName = 'NullAI', Scriptname = '' WHERE entry = 17248;
UPDATE creature_template SET AIName = 'NullAI', Scriptname = '' WHERE entry = 17265;

-- Prince Malchezaar
UPDATE creature_template SET AIName = 'NullAI', Scriptname = '' WHERE entry = 17646;
