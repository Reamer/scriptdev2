/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TRIAL_OF_THE_CRUSADER_H
#define DEF_TRIAL_OF_THE_CRUSADER_H

enum Achievments
{
    CRITERIA_ACHIEV_UPPER_BACK_PAIN_10N             = 11779, // Gormok
    CRITERIA_ACHIEV_UPPER_BACK_PAIN_25N             = 11780,

    CRITERIA_ACHIEV_SIXTY_PAIN_SPIKE_10N            = 11838, // Lord Jaraxxus
    CRITERIA_ACHIEV_SIXTY_PAIN_SPIKE_25N            = 11839,

    CRITERIA_ACHIEV_TRIBUTE_TO_SKILL_10             = 12344,
    CRITERIA_ACHIEV_TRIBUTE_TO_SKILL_25             = 12338,
    CRITERIA_ACHIEV_TRIBUTE_TO_MAD_SKILL_10         = 12347,
    CRITERIA_ACHIEV_TRIBUTE_TO_MAD_SKILL_25         = 12341,
    CRITERIA_ACHIEV_TRIBUTE_TO_INSANITY_10          = 12349,
    CRITERIA_ACHIEV_TRIBUTE_TO_INSANITY_25          = 12343,
    CRITERIA_ACHIEV_TRIBUTE_TO_IMMORTALITY_HORDE    = 12247,
    CRITERIA_ACHIEV_TRIBUTE_TO_IMMORTALITY_ALLY     = 12359,
    CRITERIA_ACHIEV_TRIBUTE_TO_DEDICATED_INSANITY   = 12360,

    ACHIEV_START_VALYKYR_ID                         = 21853,

//    TYPE_UPPER_BACK_PAIN        = 0,
    TYPE_SIXTY_PAIN_SPIKE       = 1,
    TYPE_SKILL                  = 2,
    TYPE_MAD_SKILL              = 3,
    TYPE_INSANITY               = 4,
    TYPE_DEDICATED_INSANITY     = 5,
    TYPE_IMMORTALITY            = 6,
    MAX_SPECIAL_ACHIEV_CRITS    = 7,

};

struct Locations
{
    float x;
    float y;
    float z;
};

static const Locations SpawnLoc[]=
{
    {559.257996f, 90.266197f,  395.122986f}, // 0 Barrent
    {563.672974f, 139.571f,    393.837006f}, // 1 Center
    {563.833008f, 187.244995f, 394.5f},      // 2 Backdoor
    {577.347839f, 185.338888f, 395.14f},     // 3 - Right
    {550.955933f, 185.338888f, 395.14f},     // 4 - Left
    {575.042358f, 195.260727f, 395.137146f}, // 5
    {552.248901f, 195.331955f, 395.132658f}, // 6
    {573.342285f, 195.515823f, 395.135956f}, // 7
    {554.239929f, 195.825577f, 395.137909f}, // 8
    {571.042358f, 195.260727f, 395.137146f}, // 9
    {556.720581f, 195.015472f, 395.132658f}, // 10
    {569.534119f, 195.214478f, 395.139526f}, // 11
    {569.231201f, 195.941071f, 395.139526f}, // 12
    {558.811610f, 195.985779f, 394.671661f}, // 13
    {567.641724f, 195.351501f, 394.659943f}, // 14
    {560.633972f, 195.391708f, 395.137543f}, // 15
    {565.816956f, 195.477921f, 395.136810f}, // 16
    {563.549f,    152.474f,    394.393f},    // 17 - Lich king start
    {563.547f,    141.613f,    393.908f},    // 18 - Lich king end
    {787.932556f, 133.28978f,  142.612152f}, // 19 - Anub'arak start location
    {618.157898f, 132.640869f, 139.559769f}, // 20 - Anub'arak move point location
    {563.813660f, 182.842819f, 395.128052f}, // 21 - Fizzlebang start location
    {586.060242f, 117.514809f, 394.314026f}, // 22 - Dark essence 1
    {541.602112f, 161.879837f, 394.587952f}, // 23 - Dark essence 2
    {541.021118f, 117.262932f, 395.314819f}, // 24 - Light essence 1
    {586.200562f, 162.145523f, 394.626129f}, // 25 - Light essence 2
    {563.833008f, 195.244995f, 394.585561f}, // 26 - outdoor
    {563.938477f, 149.210571f, 394.321838f}, // 27 - Fizzlebang end
    {563.710999f, 131.058319f, 394.321838f}, // 28 - Jaraxxus
    {750.78f,     158.48f,     142.68f    }, // 29 - Frost Sphere 1
    {749.69f,     129.47f,     142.68f    }, // 30 - Frost Sphere 2
    {702.69f,     126.47f,     142.68f    }, // 31 - Frost Sphere 3
    {702.53f,     154.69f,     142.68f    }, // 32 - Frost Sphere 4
    {530.80f,     108.18f,     395.14f    }, // 33 - Unleashed light 1
    {530.49f,     172.75f,     395.14f    }, // 34 - Unleashed light 2
    {591.52f,     167.06f,     395.14f    }, // 35 - Unleashed light 3
    {589.40f,     114.05f,     395.14f    }, // 36 - Unleashed light 4
    {516.64f,     139.74f,     395.14f    }, // 37 - Unleashed dark 1
    {563.88f,     169.17f,     395.14f    }, // 38 - Unleashed dark 2
    {604.99f,     139.63f,     395.14f    }, // 39 - Unleashed dark 3
    {563.65f,     103.28f,     395.14f    }, // 40 - Unleashed dark 4
    {519.75f,     119.29f,     395.14f    }, // 41 - Unleashed light 1 extra (25)
    {548.70f,     170.04f,     395.14f    }, // 42 - Unleashed light 2 extra (25)
    {575.52f,     144.13f,     395.14f    }, // 43 - Unleashed light 3 extra (25)
    {570.38f,     122.28f,     395.14f    }, // 44 - Unleashed light 4 extra (25)
    {550.53f,     145.74f,     395.14f    }, // 45 - Unleashed dark 1 extra (25)
    {577.57f,     174.08f,     395.14f    }, // 46 - Unleashed dark 2 extra (25)
    {605.72f,     119.60f,     395.14f    }, // 47 - Unleashed dark 3 extra (25)
    {554.94f,     123.42f,     395.14f    }, // 48 - Unleashed dark 4 extra (25)
    {697.73f,     118.48f,     142.68f    }, // 49 - Frost Sphere 5
    {733.33f,     153.44f,     142.68f    }, // 50 - Frost Sphere 6
    {731.40f,     90.90f,      142.68f    }, // 51 - Frost Sphere 7
    {794.34f,     150.48f,     142.68f    }, // 52 - Frost Sphere 8
};

enum
{
    MAX_ENCOUNTER               = 6,
    MAX_WIPES_ALLOWED           = 50,

    TYPE_WIPE_COUNT             = 0,
    TYPE_NORTHREND_BEASTS       = 1,
    TYPE_JARAXXUS               = 2,
    TYPE_FACTION_CHAMPIONS      = 3,
    TYPE_CRUSADERS_COUNT        = 31,
    TYPE_TWIN_VALKYR            = 4,
    TYPE_ANUBARAK               = 5,

    EVENT_OPEN_PORTAL           = 6,
    EVENT_KILL_FIZZLEBANG       = 7,
    EVENT_JARAXXUS_START_ATTACK = 8,
    EVENT_SUMMON_FACTION_CHAMPIONS = 9,
    EVENT_SUMMON_TWINS          = 10,
    EVENT_MAKE_TWINS_ATTACKABLE = 11,
    EVENT_TWINS_KILLED          = 12,
    EVENT_ARTHAS_PORTAL         = 13,
    EVENT_SUMMON_THE_LICHKING   = 14,
    EVENT_DESTROY_FLOOR         = 15,

    FACTION_CHAMPIONS_HEALER_AMOUNT_10  = 3,
    FACTION_CHAMPIONS_HEALER_AMOUNT_25  = 2,
    FACTION_CHAMPIONS_MELEE_DD_AMOUNT_10= 2,
    FACTION_CHAMPIONS_MELEE_DD_AMOUNT_25= 2,
    FACTION_CHAMPIONS_MAGIC_DD_AMOUNT_10= 2,
    FACTION_CHAMPIONS_MAGIC_DD_AMOUNT_25= 2,

    NPC_BEAST_COMBAT_STALKER    = 36549,
    NPC_GORMOK                  = 34796,
    NPC_SNOBOLD_VASSAL          = 34800,
    NPC_ACIDMAW                 = 35144,
    NPC_DREADSCALE              = 34799,
    NPC_ICEHOWL                 = 34797,
    NPC_JARAXXUS                = 34780,
    NPC_MISTRESS                = 34826,
    NPC_LIGHT_FJOLA             = 34497,
    NPC_DARK_EYDIS              = 34496,
    NPC_ANUBARAK                = 34564,

    // Allianz Champions
    NPC_CRUSADER_1_1            = 34460, //Druid
    NPC_CRUSADER_1_2            = 34463, //Shaman
    NPC_CRUSADER_1_3            = 34461, //DK
    NPC_CRUSADER_1_4            = 34472, //Rogue
    NPC_CRUSADER_1_5            = 34475, //Warrior
    NPC_CRUSADER_1_6            = 34471, //Retro pal
    NPC_CRUSADER_1_7            = 34473, //Shadow priest
    NPC_CRUSADER_1_8            = 34468, //Mage
    NPC_CRUSADER_1_9            = 34467, //Hunter
    NPC_CRUSADER_1_10           = 34474, //Warlock
    NPC_CRUSADER_1_11           = 34470, //Enh shaman
    NPC_CRUSADER_1_12           = 34466, //Priest
    NPC_CRUSADER_1_13           = 34465, //Holy paladin
    NPC_CRUSADER_1_14           = 34469, //Moonkin

    // Horde Champions
    NPC_CRUSADER_2_1            = 34451, //Druid
    NPC_CRUSADER_2_2            = 34455, //Shaman
    NPC_CRUSADER_2_3            = 34458, //DK
    NPC_CRUSADER_2_4            = 34454, //Rogue
    NPC_CRUSADER_2_5            = 34453, //Warrior
    NPC_CRUSADER_2_6            = 34456, //Retro pal
    NPC_CRUSADER_2_7            = 34441, //Shadow Priest
    NPC_CRUSADER_2_8            = 34449, //Mage
    NPC_CRUSADER_2_9            = 34448, //Hunter
    NPC_CRUSADER_2_10           = 34450, //Warlock
    NPC_CRUSADER_2_11           = 34444, //Enh shaman
    NPC_CRUSADER_2_12           = 34447, //Priest
    NPC_CRUSADER_2_13           = 34445, //Holy paladin
    NPC_CRUSADER_2_14           = 34459, //Moonkin

    NPC_CRUSADER_0_1            = 35465,
    NPC_CRUSADER_0_2            = 35610,

    NPC_THRALL                  = 34994,
    NPC_PROUDMOORE              = 34992,
    NPC_TRIGGER                 = 22517,
    NPC_WILFRED_PORTAL          = 35651,

    NPC_TIRION_A                = 34996,
    NPC_TIRION_B                = 36095,                    // Summoned after his text (Champions, you're alive! Not only have you defeated every challenge of the Trial of the Crusader, but also thwarted Arthas' plans! Your skill and cunning will prove to be a powerful weapon against the Scourge. Well done! Allow one of the Crusade's mages to transport you to the surface!) is said..
    NPC_ARGENT_MAGE             = 36097,                    // Summoned along with Tirion B
    NPC_VARIAN                  = 34990,
    NPC_GARROSH                 = 34995,
    NPC_FIZZLEBANG              = 35458,
    NPC_OPEN_PORTAL_TARGET      = 17965,
    NPC_WORLD_TRIGGER_LARGE     = 22517,                    // Used for Lich King summon event
    NPC_THE_LICHKING            = 16980,
    NPC_THE_LICHKING_VISUAL     = 35877,
    NPC_RAMSEY_1                = 34816,
    NPC_RAMSEY_2                = 35035,
    NPC_RAMSEY_3                = 35766,
    NPC_RAMSEY_4                = 35770,
    NPC_RAMSEY_5                = 35771,
    NPC_RAMSEY_6                = 35895,                    // Unknown what these three NPCs are used for, maybe horde events?
    NPC_RAMSEY_7                = 35909,
    NPC_RAMSEY_8                = 35910,

    NPC_PURPLE_RUNE             = 35651,

    NPC_LIGHT_ESSENCE           = 34568,
    NPC_DARK_ESSENCE            = 34567,

    GO_MAIN_GATE                = 195647,
    GO_WEST_GATE                = 195648,                   // entrance gate
    GO_SOUTH_GATE               = 195649,                   // south and north doors are used to allow the Champions to enter the arena
    GO_NORTH_GATE               = 195650,
    GO_COLISEUM_FLOOR           = 195527,
    GO_WEB_DOOR                 = 195485,

    GO_CRUSADERS_CACHE_10       = 195631,
    GO_CRUSADERS_CACHE_25       = 195632,
    GO_CRUSADERS_CACHE_10_H     = 195633,
    GO_CRUSADERS_CACHE_25_H     = 195635,

    GO_TRIBUTE_CHEST_10H_01     = 195665,
    GO_TRIBUTE_CHEST_10H_25     = 195666,
    GO_TRIBUTE_CHEST_10H_45     = 195667,
    GO_TRIBUTE_CHEST_10H_50     = 195668,

    GO_TRIBUTE_CHEST_25H_01     = 195669,
    GO_TRIBUTE_CHEST_25H_25     = 195670,
    GO_TRIBUTE_CHEST_25H_45     = 195671,
    GO_TRIBUTE_CHEST_25H_50     = 195672,

    SPELL_OPEN_PORTAL           = 67864,
    SPELL_FEL_LIGHTNING_KILL    = 67888,
    SPELL_WILFRED_PORTAL        = 68424,
    SPELL_ARTHAS_PORTAL         = 51807,
    SPELL_FROSTNOVA             = 68198,
    SPELL_CORPSE_TELEPORT       = 69016, // NYI
    SPELL_DESTROY_FLOOR_KNOCKUP = 68193,

    DISPLAYID_DESTROYED_FLOOR   = 9060,
    POINT_COMBAT_POSITION       = 10,

    WORLD_STATE_WIPES           = 4390,
    WORLD_STATE_WIPES_COUNT     = 4389,
};

static const float aRamsayPositions[2][4] =
{
    {559.1528f, 90.55729f, 395.2734f, 5.078908f},           // Summon Position
    {563.556f,  78.72571f, 395.2125f, 0.0f}                 // Movement Position
};

static const float aSpawnPositions[][4] =
{
    {563.8941f, 137.3333f, 405.8467f, 0.0f},                // Beast combat stalker (Summoned when SAY_VARIAN_BEAST_1)
    {563.9358f, 229.8299f, 394.8061f, 4.694936f},           // Gormok (vehicle) (Summoned when SAY_VARIAN_BEAST_1)
    {564.3301f, 232.1549f, 394.8188f, 1.621917f},           // Dreadscale (Summoned when Tirion says SAY_TIRION_BEAST_2)
    {549.5139f, 170.1389f, 394.7965f, 5.009095f},           // Acidmaw (Summoned(?) 14s after Dreadscale)
    {563.6081f, 228.1491f, 394.7057f, 4.664022f},           // Icehowl (Summoned when SAY_TIRION_BEAST_3)
    {563.6007f, 208.5278f, 395.2696f, 4.729842f},           // Fizzlebang
    {563.8264f, 140.6563f, 393.9861f, 4.694936f},           // Jaraxxus
    {571.684f,  204.9028f, 399.263f,  4.590216f},           // Fjola
    {555.4514f, 205.8889f, 399.2634f, 4.886922f},           // Eydis
    {563.6996f, 175.9826f, 394.5042f, 4.694936f},           // World Trigger Large
    {563.5712f, 174.8351f, 394.4954f, 4.712389f},           // Lich King
    {563.6858f, 139.4323f, 393.9862f, 4.694936f},           // Purple Rune / Center Position
    {648.9169f, 131.0209f, 141.6159f, 0.0f},                // Tirion B
    {649.1610f, 142.0399f, 141.3060f, 0.0f},                // Argent mage
};

static const float aMovePositions[][3] =
{
    {563.748f, 179.766f, 394.4862f},    // Gormok
    {576.5347f, 168.9514f, 394.7064f},  // Dreadscale
    {563.8577f, 176.5885f, 394.4417f},  // Icehowl
    {563.7223f, 131.2344f, 393.9901f},  // Jaraxxus
    {563.8137f, 149.3506f, 393.9902f},  // Fizzlebang
    {583.7379f, 170.8314f, 394.7381f},  // Light Fjola
    {543.2705f, 170.5823f, 394.7373f},  // Dark Eydis
};

static const float aEssencePositions[4][4] =
{
    { 541.021118f, 117.262932f, 395.314819f, 0.0f }, // Light essence
    { 586.200562f, 162.145523f, 394.626129f, 0.0f }, // Light essence
    { 586.060242f, 117.514809f, 394.314026f, 0.0f }, // Dark essence
    { 541.602112f, 161.879837f, 394.587952f, 0.0f }, // Dark essence
};

enum DummyNPC
{
    NPC_VALKYR_TWINS_BULLET_STALKER_DARK    = 34704,
    NPC_VALKYR_TWINS_BULLET_STALKER_LIGHT   = 34720,
    NPC_NERUBIAN_BURROW                     = 34862,
};

class MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_trial_of_the_crusader(Map* pMap);
        ~instance_trial_of_the_crusader() {}

        void Initialize();
        bool IsEncounterInProgress() const;


        void UpdateWorldState();

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureDeath(Creature * pCreature);
        void OnObjectCreate(GameObject *pGo);

        void OnPlayerEnter(Player *pPlayer);
        void OnPlayerDeath(Player *pPlayer);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);
        void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        // Difficulty wrappers
        bool IsHeroicDifficulty() { return instance->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC; }
        bool Is25ManDifficulty() { return instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL || instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC; }

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff) { DialogueUpdate(uiDiff); }

    private:
        void DoSummonRamsey(uint32 uiEntry);
        void JustDidDialogueStep(int32 iEntry);
        void SummonFactionChampion();
        void DoHandleEventEpilogue();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        Team m_uiTeam;

        bool m_bAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];

        GuidList m_lNerubianBurrow;
        GuidList m_lFactionChampion;

        uint8 m_uiSnoboldVassalLifeCounter;
        uint8 m_uiMistressLifeCounter;
};

#endif
