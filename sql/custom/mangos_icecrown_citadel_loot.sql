

DELETE FROM gameobject_loot_template WHERE entry IN (28058, 28046, 28074, 28088);
INSERT INTO gameobject_loot_template VALUES
-- 10 non hero - gameobject 202239 loot entry 28046
(28046, 800003, 100, 0, -800003, 2, 0),
(28046, 49426, 100, 0, 2, 2, 0), -- frost emblem
(28046, 49908,  17, 0, 1, 1, 0), -- Primordial Saronite
-- 10 hero - gamobject 202238 loot entry 28058
(28058, 800004, 100, 0, -800004, 2, 0),
(28058, 49426, 100, 0, 2, 2, 0), -- frost emblem
(28058, 45103, 100, 0, -45103, 1, 0), -- Conqueror's Mark of Sanctification
(28058, 49908,  17, 0, 1, 1, 0), -- Primordial Saronite
(28058, 50274,  10, 0, 1, 1, 0), -- Shadowfrost Shard
-- 25 nonhero - gameobject 202240 loot entry 28074
(28074, 800005, 100, 0, -800005, 2, 0),
(28074, 49426, 100, 0, 2, 2, 0), -- frost emblem
(28074, 45103, 100, 0, -45103, 1, 0), -- Conqueror's Mark of Sanctification
(28074, 49908,  17, 0, 1, 1, 0), -- Primordial Saronite
(28074, 50274,  10, 0, 1, 1, 0), -- Shadowfrost Shard
-- 25 hero - gameobject 202241 loot entry 28088
(28088, 800005, 100, 0, -800005, 2, 0),
(28088, 49426, 100, 0, 2, 2, 0), -- frost emblem
(28088, 45103, 100, 0, -45103, 1, 0), -- Conqueror's Mark of Sanctification
(28088, 49908,  17, 0, 1, 1, 0), -- Primordial Saronite
(28088, 50274,  35, 0, 1, 1, 0); -- Shadowfrost Shard


DELETE FROM reference_loot_template WHERE entry IN (800003, 800004, 800005, 800006);
INSERT INTO reference_loot_template VALUES
-- 10 non hero
(800003, 50798, 0, 1, 1, 1, 0),
(800003, 50799, 0, 1, 1, 1, 0),
(800003, 50800, 0, 1, 1, 1, 0),
(800003, 50801, 0, 1, 1, 1, 0),
(800003, 50802, 0, 1, 1, 1, 0),
(800003, 50803, 0, 1, 1, 1, 0),
(800003, 50804, 0, 1, 1, 1, 0),
(800003, 50805, 0, 1, 1, 1, 0),
(800003, 50806, 0, 1, 1, 1, 0),
(800003, 50807, 0, 1, 1, 1, 0),
(800003, 50808, 0, 1, 1, 1, 0),
(800003, 50809, 0, 1, 1, 1, 0),
-- 10 hero
(800004, 51894, 0, 1, 1, 1, 0),
(800004, 51895, 0, 1, 1, 1, 0),
(800004, 51896, 0, 1, 1, 1, 0),
(800004, 51897, 0, 1, 1, 1, 0),
(800004, 51898, 0, 1, 1, 1, 0),
(800004, 51899, 0, 1, 1, 1, 0),
(800004, 51900, 0, 1, 1, 1, 0),
(800004, 51901, 0, 1, 1, 1, 0),
(800004, 51902, 0, 1, 1, 1, 0),
(800004, 51903, 0, 1, 1, 1, 0),
(800004, 51904, 0, 1, 1, 1, 0),
(800004, 51905, 0, 1, 1, 1, 0),
-- 25 non hero
(800005, 50362, 0, 1, 1, 1, 0),
(800005, 50412, 0, 1, 1, 1, 0),
(800005, 50333, 0, 1, 1, 1, 0),
(800005, 50014, 0, 1, 1, 1, 0),
(800005, 50015, 0, 1, 1, 1, 0),
-- 25 hero
(800006, 50363, 0, 1, 1, 1, 0),
(800006, 50672, 0, 1, 1, 1, 0),
(800006, 50668, 0, 1, 1, 1, 0),
(800006, 50670, 0, 1, 1, 1, 0),
(800006, 50671, 0, 1, 1, 1, 0);
