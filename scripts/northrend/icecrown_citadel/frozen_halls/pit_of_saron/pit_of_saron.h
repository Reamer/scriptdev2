/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_PIT_H
#define DEF_ICECROWN_PIT_H

enum
{
    TYPE_GARFROST                   = 0,
    TYPE_KRICK                      = 1,
    TYPE_TYRANNUS                   = 2,
    TYPE_GAUNTLET                   = 3,
    TYPE_INTRO                      = 4,
    MAX_ENCOUNTER                   = 5,

    NPC_TYRANNUS_INTRO              = 36794,
    NPC_GARFROST                    = 36494,
    NPC_KRICK                       = 36477,
    NPC_KRICK_EVENT                 = 37629,
    NPC_ICK                         = 36476,
    NPC_TYRANNUS                    = 36658,
    NPC_RIMEFANG                    = 36661,
    NPC_SINDRAGOSA                  = 37755,

    NPC_SILVANA_BEGIN               = 36990,
    NPC_SILVANA_END                 = 38189,
    NPC_JAINA_BEGIN                 = 36993,
    NPC_JAINA_END                   = 38188,
    NPC_DARK_RANGER_KALIRA          = 37583,
    NPC_DARK_RANGER_LORALEN         = 37779,
    NPC_ARCHMAGE_ELANDRA            = 37774,
    NPC_ARCHMAGE_KORELN             = 37582,
    NPC_COLISEUM_CHAMPION_A_P       = 37498,                    // Alliance Paladin
    NPC_COLISEUM_CHAMPION_A_F       = 37496,                    // Alliance Footman
    NPC_COLISEUM_CHAMPION_A_M       = 37497,                    // Alliance Mage
    NPC_COLISEUM_CHAMPION_H_F       = 37584,                    // Horde Footman
    NPC_COLISEUM_CHAMPION_H_T       = 37587,                    // Horde Taure
    NPC_COLISEUM_CHAMPION_H_M       = 37588,                    // Horde Mage

    NPC_SLAVE_1                     = 36770,

    NPC_SLAVE_HORDE_1               = 37578,
    NPC_SLAVE_HORDE_2               = 37577,
    NPC_SLAVE_HORDE_3               = 37579,
    NPC_SLAVE_ALLY_1                = 37572,
    NPC_SLAVE_ALLY_2                = 37575,
    NPC_SLAVE_ALLY_3                = 37576,
    NPC_MARTIN_VICTUS_SLAVE         = 37591, 
    NPC_MARTIN_VICTUS_END           = 37580,
    NPC_GORKUN_IRONSKULL_SLAVE      = 37592,
    NPC_GORKUN_IRONSKULL_END        = 37581,

    NPC_GEIST_AMBUSHER              = 36886,

    GO_BALL_AND_CHAIN               = 201969,
    GO_ICEWALL                      = 201885,               // open after gafrost/krick
    GO_HALLS_OF_REFLECT_PORT        = 201848,               // unlocked by jaina/sylvanas at last outro

    SPELL_STRANGULATING             = 69413,
    SPELL_NECROTIC_POWER            = 69347,
    SPELL_FROST_BOMB                = 70521,
    SPELL_SUMMON_ICICLE             = 69424,
};

enum PitOfSaronEvent
{
    EVENT_INTRO                     = 1,
    EVENT_GARFROST                  = 2,
    EVENT_KRICK                     = 3,
    EVENT_GAUNTLET                  = 4,
    EVENT_TYRANNUS_START            = 5,
    EVENT_TYRANNUS_END              = 6,
};

class MANGOS_DLL_DECL instance_pit_of_saron : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_pit_of_saron(Map* pMap);
        ~instance_pit_of_saron() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); };
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);
        Team GetFaction() {return m_Team; };

    protected:
        void JustDidDialogueStep(int32 iEntry);
        void ProcessEventNpcs(Player* pPlayer, PitOfSaronEvent uiEvent);
        void SendTyrannusToMiddle();
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        GuidList m_lSoldiersGuids;
        GuidList m_lEventGuids;
        GuidList m_lSlaveGuids;
        GuidList m_lGeistAmbusher[2];
        Team m_Team;

        uint32 m_uiIcicleTimer;

};
 

#endif
