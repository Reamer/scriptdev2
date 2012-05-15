/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 - 2012 MangosR2 <http://github.com/mangosR2/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/* ScriptData
SDName: instance_culling_of_stratholme
SD%Complete: %
SDComment:
EndScriptData */

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H

enum Data
{
    GO_CRATE_LIGHT              = 190117,

    NPC_KNIGHT              = 28612,
    NPC_INVIS_STALKER       = 20562,
    WORLD_STATE_COS_WAVE_COUNT    = 3504,

    MALGANIS_KC_BUNNY      = 31006,

///-> end

    NPC_CHROMI03            = 30997,

    NPC_INFINITE_ADVERSARY  = 27742,
    NPC_INFINITE_HUNTER     = 27743,
    NPC_INFINITE_AGENT      = 27744,
    NPC_TIME_RIFT           = 28409,
    NPC_TIME_RIFT_2         = 28439,
    
    NPC_STALKER             = 28199,  ///->??? might be typo for invis_stalker
    NPC_INFINITE_CORRUPTOR  = 32273,

    GO_MALGANIS_GATE1      = 187711,
    GO_MALGANIS_GATE2      = 187723,
    GO_MALGANIS_CHEST      = 190663,
    GO_MALGANIS_CHEST_H    = 193597,
    GO_EXIT                = 191788,


    WORLD_STATE_COS_TIME_ON       = 3932,
    WORLD_STATE_COS_TIME_COUNT    = 3931,

    RIGHT               = 0,
    LEFT                = 1,
};

enum
{
    MAX_WAVE_MOB                    = 10,
    MAX_ENCOUNTER                   = 9,

    TYPE_GRAIN_EVENT                = 0,                    // crates with plagued grain identified
    TYPE_ARTHAS_INTRO_EVENT         = 1,                    // Arhas Speech and Walk to Gates and short intro with MalGanis
    TYPE_MEATHOOK_EVENT             = 2,                    // Waves 1-5
    TYPE_SALRAMM_EVENT              = 3,                    // Waves 6-10
    TYPE_EPOCH_EVENT                = 4,                    // Townhall Event, Boss Killed
    TYPE_ARTHAS_ESCORT_EVENT        = 5,                    // Townhall to Malganis
    TYPE_MALGANIS_EVENT             = 6,                    // Malganis
    TYPE_INFINITE_CORRUPTER_TIME    = 7,                    // Time for 25min Timer
    TYPE_INFINITE_CORRUPTER         = 8,

    // Main Encounter NPCs
    NPC_CHROMIE_INN                 = 26527,
    NPC_CHROMIE_ENTRANCE            = 27915,
    NPC_CHROMIE_END                 = 30997,
    // NPC_HOURGLASS                = 28656,
    NPC_ARTHAS                      = 26499,
    NPC_JAINA_PROUDMOORE            = 26497,
    NPC_UTHER_THE_LIGHTBRINGER      = 26528,
    NPC_MEATHOOK                    = 26529,
    NPC_SALRAMM_THE_FLESHCRAFTER    = 26530,
    NPC_CHRONO_LORD_EPOCH           = 26532,
    NPC_MALGANIS                    = 26533,
    NPC_INFINITE_CORRUPTER          = 32273,
    NPC_LORDAERON_CRIER             = 27913,
    NPC_ZOMBIE                      = 27737,

    // Wave NPC
    NPC_ENTAGING_GHOUL              = 27729,
    NPC_DEVOURING_GHOUL             = 28249,
    NPC_DARK_NECROMANCER            = 28200,
    NPC_CRYPT_FIEND                 = 27734,
    NPC_TOMB_STALKER                = 28199,
    NPC_ACOLYTE                     = 27731,
    NPC_BILE_GOLEM                  = 28201,
    NPC_PATCHWORK_CONSTRUCT         = 27736,

    // Inn Event related NPC
    NPC_MICHAEL_BELFAST             = 30571,
    NPC_HEARTHSINGER_FORRESTEN      = 30551,
    NPC_FRAS_SIABI                  = 30552,
    NPC_FOOTMAN_JAMES               = 30553,
    NPC_MAL_CORRICKS                = 31017,
    NPC_GRYAN_STOUTMANTLE           = 30561,

    // Grain Event NPCs
    NPC_ROGER_OWENS                 = 27903,
    NPC_SERGEANT_MORIGAN            = 27877,
    NPC_SILVIO_PERELLI              = 27876,
    NPC_JENA_ANDERSON               = 27885,
    NPC_MARTHA_GOSLIN               = 27884,
    NPC_MALCOM_MOORE                = 27891,                // Not (yet?) spawned
    NPC_SCRUFFY                     = 27892,                // Not (yet?) spawned
    NPC_BARTLEBY_BATTSON            = 27907,
    NPC_CRATES_BUNNY                = 30996,

    // Intro Event NPCs
    NPC_LORDAERON_FOOTMAN           = 27745,
    NPC_STRATHOLME_CITIZEN          = 28167,
    NPC_STRATHOLME_RESIDENT         = 28169,

    // Mobs in Stratholme (to despawn) -- only here for sake of completeness handling remains open (mangos feature)
    NPC_MAGISTRATE_BARTHILAS        = 30994,
    NPC_STEPHANIE_SINDREE           = 31019,
    NPC_LEEKA_TURNER                = 31027,
    NPC_SOPHIE_AAREN                = 31021,
    NPC_ROBERT_PIERCE               = 31025,
    NPC_GEORGE_GOODMAN              = 31022,

    // Others NPCs in Stratholme
    NPC_EMERY_NEILL                 = 30570,
    NPC_EDWARD_ORRICK               = 31018,
    NPC_OLIVIA_ZENITH               = 31020,

    // Townhall Event NPCs
    NPC_AGIATED_STRATHOLME_CITIZEN  = 31126,
    NPC_AGIATED_STRATHOLME_RESIDENT = 31127,
    NPC_PATRICIA_O_REILLY           = 31028,
    NPC_STRATHOLME_RESIDENT_HOUSE   = 28341,

    // Gameobjects
    GO_DOOR_BOOKCASE                = 188686,
    GO_DARK_RUNED_CHEST             = 190663,
    GO_DARK_RUNED_CHEST_H           = 193597,

    // World States
    WORLD_STATE_CRATES              = 3479,
    WORLD_STATE_CRATES_COUNT        = 3480,
    WORLD_STATE_WAVE                = 3504,
    WORLD_STATE_TIME                = 3932,
    WORLD_STATE_TIME_COUNTER        = 3931,

    SPELL_MALGANIS_KILL_CREDIT      = 58124,
    // Areatrigger
    AREATRIGGER_INN                 = 5291,
    /*
    5085 before bridge - could be Uther SpawnPos
    5148 ini entrance
    5181 ini exit
    5249 fras siabis store
    5250 leeking shields...(store)
    5251 bar in stratholme
    5252 Aaren flowers
    5253 Angelicas boutique
    5256 townhall
    5291 Inn */
};
enum eInstancePosition
{
    POS_ARTHAS_INTRO        = 1,
    POS_ARTHAS_WAVES        = 2,
    POS_ARTHAS_TOWNHALL     = 3,
    POS_ARTHAS_ESCORTING    = 4,
    POS_ARTHAS_MALGANIS     = 5,
    POS_INSTANCE_FINISHED   = 6
};

enum
{
    SAY_ROGER01                     = -1597280,
    SAY_ROGER02                     = -1597281,
    SAY_ROGER03                     = -1597282,
    SAY_ROGER04                     = -1597283,
};

enum
{
    SAY_MORIGAN01                     = -1597284,
    SAY_PERELLI02                     = -1597285,
    SAY_MORIGAN03                     = -1597286,
    SAY_PERELLI04                     = -1597287,
    SAY_MORIGAN05                     = -1597288,
    SAY_PERELLI06                     = -1597289,
    SAY_MORIGAN07                     = -1597290,
};

enum
{
    SAY_JENA01           = -1597291,
    SAY_MARTHA02         = -1597292,
    SAY_JENA03           = -1597293,
    SAY_JENA04           = -1597294,
    SAY_MARTHA05         = -1597295,

    SPELL_USE_WRENCH      = 58925,

};

enum
{
    SOUND_ID_DOG_GROWL    = 1108,
    SOUND_ID_DOG_HOWL     = 1018,
    EMOTE_DOG_HOWL        = 393,

    SAY_MALCOLM01         = -1597296,
    SAY_MALCOLM02         = -1597297,
    SAY_MALCOLM03         = -1597298,
    SAY_MALCOLM04         = -1597299,
};

enum
{
    SAY_BARTLEBY01         = -1597300,
    SAY_BARTLEBY02         = -1597301,
    SAY_BARTLEBY03         = -1597302,
    SAY_BARTLEBY04         = -1597303,
    SAY_BARTLEBY05         = -1597304,
};
class MANGOS_DLL_DECL instance_culling_of_stratholme : public ScriptedInstance
{
    public:
        instance_culling_of_stratholme(Map* pMap);
        ~instance_culling_of_stratholme() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);
        void StartWaveEvent();
        void SummonWave(); 

        // crates event
        bool StartCratesEvent(uint32 npcEntry);
        uint32 RogerOwensEvent();
        uint32 SergeantMoriganEvent();
        uint32 JenaAndersonEvent();
        uint32 MalcomMooreEvent();
        uint32 BartlebyBattsonEvent();

        GUIDList GetStratAgiatedCitizenList(){ return m_lAgiatedCitizenGUIDList; };
        GUIDList GetStratAgiatedResidentList(){ return m_lAgiatedResidentGUIDList; };
        void ConvertCityToDeath();
        void TeleportMalganisAndSpawnIfNeeded(bool entrance);

        void GetCratesBunnyOrderedList(std::list<Creature*> &lList);
        Creature* GetStratIntroFootman();
        void GetResidentOrderedList(std::list<Creature*> &lList);
        void DoSpawnArthasIfNeeded();
        void DoSpawnChromieIfNeeded();
        void DoSpawnUtherAndJainaIfNeeded();
        void DoSpawnMalcomAndScruffyIfNeed();
        uint8 GetInstancePosition();
        void ArthasJustDied();

    protected:
        void OnPlayerEnter(Player* pPlayer);
        void DoChromieHurrySpeech();

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint8 m_uiGrainCrateCount;
        uint32 m_uiRemoveCrateStateTimer;
        uint32 m_uiArthasRespawnTimer;

        uint32 m_uiWaveSummonTimer;
        uint32 m_uiWaveSummonCounter;

        // Genreal
        uint32 m_uiZombieConvertTimer;

        // crates event
        uint32 m_uiRogerOwensEvent;
        uint32 m_uiRogerOwensEventCounter;
        uint32 m_uiSergeantMoriganEvent;
        uint32 m_uiSergeantMoriganEventCounter;
        uint32 m_uiJenaAndersonEvent;
        uint32 m_uiJenaAndersonEventCounter;
        uint32 m_uiMalcomMooreEvent;
        uint32 m_uiMalcomMooreEventCounter;
        uint32 m_uiBartlebyBattsonEvent;
        uint32 m_uiBartlebyBattsonEventCounter;

        GUIDList m_luiCratesBunnyGUIDs;
        GUIDList m_luiFootmanGUIDs;
        GUIDList m_luiResidentGUIDs;

        GUIDList m_lAgiatedCitizenGUIDList;
        GUIDList m_lAgiatedResidentGUIDList;
};
#endif
