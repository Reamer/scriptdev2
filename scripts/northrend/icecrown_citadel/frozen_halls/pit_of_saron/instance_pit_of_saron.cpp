/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_pit_of_saron
SD%Complete: 50%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

struct LocationsXYZ
{
    float x, y, z;
};

static LocationsXYZ GeneralLoc[]=
{
    {724.221f, 31.042f, 602.419f},      // Middle of Instance
    {440.350f, 213.938f,528.032f},      // Entrance
    {520.755f, 221.055f,528.708f},      // MovePoint for Soldiers
    {695.46f, -156.31f, 546.061f},      // MovePoint for Tyrannus at Garfrost
    {860.649f, 124.863f, 536.019f},     // MovePoint for Tyrannus at Krick
};

struct sIntoEventNpcSpawnLocations
{
    uint32 uiEntryHorde, uiEntryAlliance;
    float fSpawnX, fSpawnY, fSpawnZ, fSpawnO;
};

const sIntoEventNpcSpawnLocations aEventIntroLocations[] =
{
    {NPC_SILVANA_BEGIN        , NPC_JAINA_BEGIN          , 441.397f, 213.320f, 528.710f, 0.104f},
    {NPC_DARK_RANGER_KALIRA   , NPC_ARCHMAGE_ELANDRA     , 439.269f, 215.891f, 528.710f, 0.022f},
    {NPC_DARK_RANGER_LORALEN  , NPC_ARCHMAGE_KORELN      , 439.544f, 210.319f, 528.708f, 0.034f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 424.148f, 212.043f, 529.962f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 424.523f, 213.236f, 529.962f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 422.715f, 212.288f, 529.963f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 423.957f, 212.905f, 529.962f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 424.729f, 211.861f, 529.962f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 422.698f, 212.861f, 529.961f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 428.069f, 212.260f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 427.760f, 213.200f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_F, NPC_COLISEUM_CHAMPION_A_F, 427.474f, 211.417f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_M, NPC_COLISEUM_CHAMPION_A_M, 425.774f, 211.542f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_M, NPC_COLISEUM_CHAMPION_A_M, 424.854f, 211.017f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_M, NPC_COLISEUM_CHAMPION_A_M, 425.974f, 212.984f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_M, NPC_COLISEUM_CHAMPION_A_M, 425.417f, 214.118f, 530.651f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_T, NPC_COLISEUM_CHAMPION_A_P, 423.391f, 212.675f, 530.609f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_T, NPC_COLISEUM_CHAMPION_A_P, 422.651f, 213.917f, 530.609f, 0.0f},
    {NPC_COLISEUM_CHAMPION_H_T, NPC_COLISEUM_CHAMPION_A_P, 422.503f, 211.469f, 530.609f, 0.0f},
};

struct sEventNpcLocationsWithEndpoint
{
    uint32 uiEntryHorde, uiEntryAlliance;
    float fStartO, fEndO;                                   // Orientation for Spawning
    float fSpawnX, fSpawnY, fSpawnZ;
    float fEndX, fEndY, fEndZ;
};

const sEventNpcLocationsWithEndpoint aEventTyrannusLocations[]=
{
    // Horde Entry              Ally Entry                 O_Spawn     O_End      SpawnPos                             EndPos
    {NPC_SILVANA_END         ,  NPC_JAINA_END,             2.247f,     0.0f,      1065.983f, 94.954f, 630.997f,        1068.709f, 208.378f, 628.156f},
    {NPC_DARK_RANGER_LORALEN,   NPC_ARCHMAGE_KORELN,       0.0f,       0.0f,      1060.955f, 107.274f, 628.424f,       1065.827f, 210.836f, 628.156f},
    {NPC_DARK_RANGER_KALIRA,    NPC_ARCHMAGE_ELANDRA,      0.0f,       0.0f,      1052.122f, 103.916f, 628.454f,       1072.659f, 204.432f, 628.156f},
};

// TODO: More Slaves ~ 10 more
const sEventNpcLocationsWithEndpoint aEventGarfrostLocations[]=
{
    // Horde Entry              Ally Entry                 O_Spawn     O_End      SpawnPos                             EndPos
    {NPC_GORKUN_IRONSKULL_SLAVE,NPC_MARTIN_VICTUS_SLAVE,   4.72f,      1.66f,     694.794f, -104.905f, 513.736f,       695.460f, -156.310f, 528.061f},
    {NPC_SLAVE_HORDE_1,         NPC_SLAVE_ALLY_1,          4.29f,      5.09f,     710.903f, -107.531f, 513.734f,       721.655f, -204.756f, 527.768f},
    {NPC_SLAVE_HORDE_2,         NPC_SLAVE_ALLY_2,          5.16f,      2.39f,     682.399f, -107.326f, 513.736f,       703.481f, -214.786f, 526.769f},
    {NPC_SLAVE_HORDE_3,         NPC_SLAVE_ALLY_3,          4.52f,      4.89f,     702.850f, -106.059f, 513.734f,       722.479f, -203.190f, 527.343f},
};

const sIntoEventNpcSpawnLocations aEventKrickLocations[]=
{
    {NPC_SILVANA_BEGIN,         NPC_JAINA_BEGIN,           783.565f, 112.559f, 509.461f, 2.247f},
//    {NPC_DARK_RANGER_LORALEN,   NPC_ARCHMAGE_KORELN,       0.0f,       0.0f,      783.565f, 112.559f, 509.461f,        0.0f, 0.0f, 0.0f},
//    {NPC_DARK_RANGER_KALIRA,    NPC_ARCHMAGE_ELANDRA,      0.0f,       0.0f,      783.565f, 112.559f, 509.461f,        0.0f, 0.0f, 0.0f},
};

// TODO: More Slaves ~ 10 more
const sEventNpcLocationsWithEndpoint aEventGaunletLocations[]=
{
    {NPC_GORKUN_IRONSKULL_END,  NPC_MARTIN_VICTUS_END,     2.247f,     0.0f,      1060.983f, 94.954f, 630.997f,        1014.670f, 158.714f, 628.156f},
    {NPC_SLAVE_HORDE_1,         NPC_SLAVE_ALLY_1,          0.0f,       0.0f,      1060.955f, 107.274f, 628.424f,       1019.006f, 129.684f, 628.156f},
    {NPC_SLAVE_HORDE_2,         NPC_SLAVE_ALLY_2,          0.0f,       0.0f,      1052.122f, 103.916f, 628.454f,       1015.389f, 183.650f, 628.156f},
    {NPC_SLAVE_HORDE_3,         NPC_SLAVE_ALLY_3,          0.0f,       0.0f,      1068.363f, 110.432f, 629.009f,       1003.889f, 159.652f, 628.159f},
};

enum
{
    // Intro
    SAY_INTRO_1_TYRANNUS    = -1658001,
    SAY_INTRO_2_JAINA       = -1658002,
    SAY_INTRO_2_SYLVANAS    = -1658003,
    SAY_INTRO_3_TYRANNUS    = -1658004,
    SAY_INTRO_4_TYRANNUS    = -1658005,
    SAY_INTRO_5_JAINA       = -1658006,
    SAY_INTRO_5_SYLVANAS    = -1658007,
    SAY_INTRO_6_TYRANNUS    = -1658008,
    SAY_INTRO_7_JAINA       = -1658009,
    SAY_INTRO_8_JAINA       = -1658010,
    SAY_INTRO_8_SYLVANAS    = -1658011,
    SAY_INTRO_9_JAINA       = -1658012,
    SAY_INTRO_9_SYLVANAS    = -1658013,

    // Garfrost
    SAY_GARFROST_TYRANNUS_OUTRO                = -1658020,
    SAY_GARFROST_FREE_SLAVE                    = -1658021,

    // Ick and Krick
    SAY_OUTRO_KRICK_1                          = -1658035,
    SAY_OUTRO_KRICK_2_ALLY                     = -1658036,
    SAY_OUTRO_KRICK_2_HORDE                    = -1658037,
    SAY_OUTRO_KRICK_3                          = -1658038,
    SAY_OUTRO_KRICK_4_ALLY                     = -1658039,
    SAY_OUTRO_KRICK_4_HORDE                    = -1658040,
    SAY_OUTRO_KRICK_5                          = -1658041,
    SAY_OUTRO_KRICK_6_TYRANNUS                 = -1658042,
    SAY_OUTRO_KRICK_7                          = -1658043,
    SAY_OUTRO_KRICK_8_TYRANNUS                 = -1658044,
    SAY_OUTRO_KRICK_9_ALLY                     = -1658045,
    SAY_OUTRO_KRICK_9_HORDE                    = -1658046,

    // Gauntlet
    SAY_GAUNTLET_1                             = -1658049,

    // Tyrannus Start
    SAY_GAUNTLET_PREFIGHT_1                    = -1658050,
    SAY_GAUNTLET_GENERAL_TRASH                 = -1658051,
    SAY_GAUNTLET_PREFIGHT_2                    = -1658052,

    // Tyrannus
    SAY_OUTRO_TYRANNUS_1_SLAVE          = -1658061,
    SAY_OUTRO_TYRANNUS_2_SLAVE          = -1658062,
    SAY_OUTRO_TYRANNUS_3_ALLY           = -1658063,
    SAY_OUTRO_TYRANNUS_3_HORDE          = -1658064,
    SAY_OUTRO_TYRANNUS_4_ALLY           = -1658065,
    SAY_OUTRO_TYRANNUS_4_HORDE          = -1658067,
    SAY_OUTRO_TYRANNUS_5_ALLY           = -1658066,

};

static const DialogueEntryTwoSide aPoSDialogues[] =
{
    // Intro Event
    {EVENT_INTRO, 0, 0, 0,                         4000},
    {SAY_INTRO_1_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_INTRO_2_JAINA, NPC_JAINA_BEGIN, SAY_INTRO_2_SYLVANAS, NPC_SILVANA_BEGIN,     5000},
    {SAY_INTRO_3_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_INTRO_4_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0,     5000},
    {SAY_INTRO_5_JAINA, NPC_JAINA_BEGIN, SAY_INTRO_5_SYLVANAS, NPC_SILVANA_BEGIN,     5000},
    {SAY_INTRO_6_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_INTRO_7_JAINA, NPC_JAINA_BEGIN, 0, 0,    4000}, // extra text for Jaina Proudmoore
    {SAY_INTRO_8_JAINA, NPC_JAINA_BEGIN, SAY_INTRO_8_SYLVANAS, NPC_SILVANA_BEGIN,     7000},
    {SAY_INTRO_9_JAINA, NPC_JAINA_BEGIN, SAY_INTRO_9_SYLVANAS, NPC_SILVANA_BEGIN,     0},
    {EVENT_GARFROST, 0, 0, 0,                      6000},
    {SAY_GARFROST_FREE_SLAVE, NPC_MARTIN_VICTUS_SLAVE, SAY_GARFROST_FREE_SLAVE, NPC_GORKUN_IRONSKULL_SLAVE, 13000},
    {SAY_GARFROST_TYRANNUS_OUTRO, NPC_TYRANNUS_INTRO, 0, 0, 0},
    {EVENT_KRICK, 0, 0, 0,                         3000},
    {SAY_OUTRO_KRICK_1, NPC_KRICK, 0, 0,                 15000},
    {SAY_OUTRO_KRICK_2_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO_KRICK_2_HORDE, NPC_SILVANA_BEGIN, 5000},
    {SAY_OUTRO_KRICK_3, NPC_KRICK, 0, 0,                 15000},
    {SAY_OUTRO_KRICK_4_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO_KRICK_4_HORDE, NPC_SILVANA_BEGIN, 5000},
    {SAY_OUTRO_KRICK_5, NPC_KRICK, 0, 0,                  4000},
    {SAY_OUTRO_KRICK_6_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0, 4000},
    {SAY_OUTRO_KRICK_7, NPC_KRICK, 0, 0,                  3000},
    {SAY_OUTRO_KRICK_8_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0, 10000},
    {SAY_OUTRO_KRICK_9_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO_KRICK_9_HORDE, NPC_SILVANA_BEGIN, 0},
    {EVENT_GAUNTLET, 0, 0, 0, 1000},
    {SAY_GAUNTLET_1, NPC_TYRANNUS, 0, 0, 0},
    {EVENT_TYRANNUS_START, 0, 0, 0, 1000},
    {SAY_GAUNTLET_PREFIGHT_1, NPC_TYRANNUS, 0, 0, 5000},
    {SAY_GAUNTLET_GENERAL_TRASH, NPC_MARTIN_VICTUS_END, SAY_GAUNTLET_GENERAL_TRASH, NPC_GORKUN_IRONSKULL_END, 4000},
    {SAY_GAUNTLET_PREFIGHT_2, NPC_TYRANNUS, 0, 0, 0},
    {EVENT_TYRANNUS_END, 0, 0, 0, 2000},
    {SAY_OUTRO_TYRANNUS_1_SLAVE, NPC_MARTIN_VICTUS_SLAVE, SAY_OUTRO_TYRANNUS_1_SLAVE, NPC_GORKUN_IRONSKULL_SLAVE, 18000},
    {SAY_OUTRO_TYRANNUS_2_SLAVE, NPC_MARTIN_VICTUS_SLAVE, SAY_OUTRO_TYRANNUS_2_SLAVE, NPC_GORKUN_IRONSKULL_SLAVE, 13000},
    {SAY_OUTRO_TYRANNUS_3_ALLY, NPC_JAINA_END, SAY_OUTRO_TYRANNUS_3_HORDE, NPC_SILVANA_END, 5000},
    {SAY_OUTRO_TYRANNUS_4_ALLY, NPC_JAINA_END, SAY_OUTRO_TYRANNUS_4_HORDE, NPC_SILVANA_END, 7000},
    {SAY_OUTRO_TYRANNUS_5_ALLY, NPC_JAINA_END, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

instance_pit_of_saron::instance_pit_of_saron(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aPoSDialogues),
    m_Team(TEAM_NONE), m_uiIcicleTimer(3000)
{
    Initialize();
}

void instance_pit_of_saron::Initialize()
{
    InitializeDialogueHelper(this);
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        m_auiEncounter[i] = NOT_STARTED;
}

void instance_pit_of_saron::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_TYRANNUS_INTRO:
        case NPC_SINDRAGOSA:
            pCreature->SetActiveObjectState(true);
            /* no break */
        case NPC_GARFROST:
        case NPC_KRICK:
        case NPC_KRICK_EVENT:
        case NPC_ICK:
        case NPC_TYRANNUS:
        case NPC_RIMEFANG:
        case NPC_SLAVE_1:
            break;
        case NPC_JAINA_BEGIN:
        case NPC_JAINA_END:
        case NPC_SILVANA_BEGIN:
        case NPC_SILVANA_END:
        case NPC_DARK_RANGER_KALIRA:
        case NPC_ARCHMAGE_ELANDRA:
        case NPC_DARK_RANGER_LORALEN:
        case NPC_ARCHMAGE_KORELN:
            m_lEventGuids.push_back(pCreature->GetObjectGuid());
            break;
        case NPC_COLISEUM_CHAMPION_A_P:
        case NPC_COLISEUM_CHAMPION_A_F:
        case NPC_COLISEUM_CHAMPION_A_M:
        case NPC_COLISEUM_CHAMPION_H_F:
        case NPC_COLISEUM_CHAMPION_H_T:
        case NPC_COLISEUM_CHAMPION_H_M:
            m_lSoldiersGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_MARTIN_VICTUS_SLAVE:
        case NPC_MARTIN_VICTUS_END:
        case NPC_GORKUN_IRONSKULL_SLAVE:
        case NPC_GORKUN_IRONSKULL_END:
        case NPC_SLAVE_HORDE_1:
        case NPC_SLAVE_HORDE_2:
        case NPC_SLAVE_HORDE_3:
        case NPC_SLAVE_ALLY_1:
        case NPC_SLAVE_ALLY_2:
        case NPC_SLAVE_ALLY_3:
            m_lSlaveGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_GEIST_AMBUSHER:
            if (pCreature->GetPositionX() < 830.0f)
                m_lGeistAmbusher[0].push_back(pCreature->GetObjectGuid());
            else
                m_lGeistAmbusher[1].push_back(pCreature->GetObjectGuid());
            break;
        default:
           return;

    }
   m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_pit_of_saron::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ICEWALL:
            if (m_auiEncounter[TYPE_GARFROST] == DONE && m_auiEncounter[TYPE_KRICK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_HALLS_OF_REFLECT_PORT:
            break;
        default:
           return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_pit_of_saron::OnPlayerEnter(Player* pPlayer)
{
    if (!m_Team)                                          // very first player to enter
    {
        m_Team = pPlayer->GetTeam();
        SetDialogueSide(m_Team == ALLIANCE);
        if (GetData(TYPE_INTRO) == NOT_STARTED)
            StartNextDialogueText(EVENT_INTRO);
    }
}

void instance_pit_of_saron::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GARFROST:
            if (uiData == DONE)
            {
                StartNextDialogueText(EVENT_GARFROST);
                if (GetData(TYPE_KRICK) == DONE)
                    DoUseDoorOrButton(GO_ICEWALL);
            }
            break;
        case TYPE_KRICK:
            if (uiData == DONE)
            {
                StartNextDialogueText(EVENT_KRICK);
                if (GetData(TYPE_GARFROST) == DONE)
                    DoUseDoorOrButton(GO_ICEWALL);
            }
            break;
        case TYPE_TYRANNUS:
            if (uiData == SPECIAL)
            {
                StartNextDialogueText(EVENT_TYRANNUS_START);
            }
            if (uiData == DONE)
            {
                StartNextDialogueText(EVENT_TYRANNUS_END);
            }
            break;
        case TYPE_GAUNTLET:
            if (uiData == IN_PROGRESS)
            {
                m_uiIcicleTimer = 3000;
                StartNextDialogueText(EVENT_GAUNTLET);
            }
            break;
        case TYPE_INTRO:
            break;
        default:
            return;
    }
    m_auiEncounter[uiType] = uiData;

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_pit_of_saron::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_pit_of_saron::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];
    return 0;
}

void instance_pit_of_saron::ProcessEventNpcs(Player* pPlayer, PitOfSaronEvent uiEvent)
{
    if (!pPlayer)
        return;

    switch (uiEvent)
    {
        case EVENT_INTRO:
        {
            // Spawn Begin Mobs
            for (uint8 i = 0; i < sizeof(aEventIntroLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
            {
                Creature* pSummon = pPlayer->SummonCreature(m_Team == HORDE ? aEventIntroLocations[i].uiEntryHorde : aEventIntroLocations[i].uiEntryAlliance, aEventIntroLocations[i].fSpawnX, aEventIntroLocations[i].fSpawnY, aEventIntroLocations[i].fSpawnZ, aEventIntroLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);
            }
            break;
        }
        case EVENT_GARFROST:
        {
            for (GuidList::const_iterator itr = m_lSlaveGuids.begin(); itr != m_lSlaveGuids.end(); ++itr)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }
            for (uint8 i = 0; i < sizeof(aEventGarfrostLocations)/sizeof(sEventNpcLocationsWithEndpoint); ++i)
            {
                if (Creature* pSummon = pPlayer->SummonCreature(m_Team == HORDE ? aEventGarfrostLocations[i].uiEntryHorde : aEventGarfrostLocations[i].uiEntryAlliance, aEventGarfrostLocations[i].fSpawnX, aEventGarfrostLocations[i].fSpawnY, aEventGarfrostLocations[i].fSpawnZ, aEventGarfrostLocations[i].fStartO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS))
                    pSummon->GetMotionMaster()->MovePoint(0, aEventGarfrostLocations[i].fEndX, aEventGarfrostLocations[i].fEndY, aEventGarfrostLocations[i].fEndZ);
            }
            break;
        }
        case EVENT_KRICK:
        {
            for (GuidList::const_iterator itr = m_lEventGuids.begin(); itr != m_lSoldiersGuids.end(); ++itr)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }
            for (uint8 i = 0; i < sizeof(aEventKrickLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
            {
                /*Creature* pSummon = */pPlayer->SummonCreature(m_Team == HORDE ? aEventKrickLocations[i].uiEntryHorde : aEventKrickLocations[i].uiEntryAlliance, aEventKrickLocations[i].fSpawnX, aEventKrickLocations[i].fSpawnY, aEventKrickLocations[i].fSpawnZ, aEventKrickLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);
            }
            break;
        }
        case EVENT_GAUNTLET:
        {
            // Nothing at the moment
            break;
        }
        case EVENT_TYRANNUS_START:
        {
            for (GuidList::const_iterator itr = m_lSlaveGuids.begin(); itr != m_lSlaveGuids.end(); ++itr)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }
            for (uint8 i = 0; i < sizeof(aEventGaunletLocations)/sizeof(sEventNpcLocationsWithEndpoint); ++i)
            {
                if (Creature* pSummon = pPlayer->SummonCreature(m_Team == HORDE ? aEventGaunletLocations[i].uiEntryHorde : aEventGaunletLocations[i].uiEntryAlliance, aEventGaunletLocations[i].fSpawnX, aEventGaunletLocations[i].fSpawnY, aEventGaunletLocations[i].fSpawnZ, aEventGaunletLocations[i].fStartO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS))
                    pSummon->GetMotionMaster()->MovePoint(0, aEventGaunletLocations[i].fEndX, aEventGaunletLocations[i].fEndY, aEventGaunletLocations[i].fEndZ);
            }
            break;
        }
        case EVENT_TYRANNUS_END:
        {
            for (GuidList::const_iterator itr = m_lEventGuids.begin(); itr != m_lSoldiersGuids.end(); ++itr)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }

            for (uint8 i = 0; i < sizeof(aEventTyrannusLocations)/sizeof(sEventNpcLocationsWithEndpoint); ++i)
            {
                pPlayer->SummonCreature(m_Team == HORDE ? aEventTyrannusLocations[i].uiEntryHorde : aEventTyrannusLocations[i].uiEntryAlliance, aEventTyrannusLocations[i].fSpawnX, aEventTyrannusLocations[i].fSpawnY, aEventTyrannusLocations[i].fSpawnZ, aEventTyrannusLocations[i].fStartO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);

                // TODO: Let the NPCs Move along their paths
            }
            break;
        }
        default:
            pPlayer->MonsterSay("Debug: Unknown Pit of Saron Event", LANG_UNIVERSAL);
            break;
    }
}

void instance_pit_of_saron::Update(uint32 const uiDiff)
{
    DialogueUpdate(uiDiff);

    if (GetData(TYPE_GAUNTLET) == IN_PROGRESS)
    {
        if (m_uiIcicleTimer < uiDiff)
        {
            if (Creature* pRimefang = GetSingleCreatureFromStorage(NPC_RIMEFANG))
            {
                Map::PlayerList const& pPlayers = instance->GetPlayers();
                if (!pPlayers.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        Unit* pTarget = itr->getSource();
                        if (pTarget && pTarget->isAlive())
                        {
                            if (roll_chance_i(50))
                                pRimefang->CastSpell(pTarget, SPELL_SUMMON_ICICLE, false);
                        }
                    }
                }
            }
            m_uiIcicleTimer = 3000;
        }
        else
            m_uiIcicleTimer -= uiDiff;
    }
}

void instance_pit_of_saron::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case EVENT_INTRO:
            ProcessEventNpcs(GetPlayerInMap(), EVENT_INTRO);
            SetData(TYPE_INTRO, IN_PROGRESS);
            break;
        case SAY_INTRO_2_JAINA:
        case SAY_INTRO_2_SYLVANAS:
            for (GuidList::const_iterator i = m_lSoldiersGuids.begin(); i != m_lSoldiersGuids.end(); ++i)
            {
                if (Creature* pSoldier = instance->GetCreature(*i))
                {
                    pSoldier->SetWalk(false);
                    pSoldier->GetMotionMaster()->MovePoint(0, GeneralLoc[2].x + irand(-10, 10), GeneralLoc[2].y + irand(-10, 10), GeneralLoc[2].z);
                }
            }
            break;
        case SAY_INTRO_4_TYRANNUS:
            for (GuidList::iterator i = m_lSoldiersGuids.begin(); i != m_lSoldiersGuids.end(); ++i)
            {
                if (Creature *pTemp = instance->GetCreature(*i))
                {
                    float x, y, z;
                    pTemp->GetPosition(x, y, z);
                    pTemp->GetMotionMaster()->MovePoint(0, x, y, z + 5.0f);
                    pTemp->CastSpell(pTemp, SPELL_STRANGULATING, false);
                }
            }
            break;
        case SAY_INTRO_6_TYRANNUS:
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
            {
                pTyrannus->CastSpell(pTyrannus, SPELL_NECROTIC_POWER, false);
            }
            for (GuidList::iterator i = m_lSoldiersGuids.begin(); i != m_lSoldiersGuids.end(); ++i)
            {
                if (Creature *pTemp = instance->GetCreature(*i))
                {
                    float x, y, z;
                    pTemp->setFaction(21);
                    pTemp->SetDisplayId(9785);
                    pTemp->GetPosition(x, y, z);
                    pTemp->GetMotionMaster()->MovePoint(0, x, y, z - 5.0f);
                    pTemp->SetSpeedRate(MOVE_RUN, 0.8f);
                    pTemp->GetMotionMaster()->MovePoint(0, GeneralLoc[1].x + irand(-10, 10), GeneralLoc[1].y + irand(-10, 10), GeneralLoc[1].z, true, true);
                    pTemp->RemoveAurasDueToSpell(SPELL_STRANGULATING);
                }
            }
            break;
        case SAY_INTRO_9_JAINA:
        case SAY_INTRO_9_SYLVANAS:
            SetData(TYPE_INTRO,DONE);
            SendTyrannusToMiddle();
            break;
        case EVENT_GARFROST:
        {
            ProcessEventNpcs(GetPlayerInMap(), EVENT_GARFROST);
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                pTyrannus->GetMotionMaster()->MovePoint(0, GeneralLoc[3].x, GeneralLoc[3].y, GeneralLoc[3].z, false);
            break;
        }
        case SAY_GARFROST_TYRANNUS_OUTRO:
            SendTyrannusToMiddle();
            break;
        case EVENT_KRICK:
        {
            ProcessEventNpcs(GetPlayerInMap(), EVENT_KRICK);
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                pKrick->RemoveAllAuras();
                pKrick->DeleteThreatList();
                pKrick->CombatStop(true);
                pKrick->InterruptNonMeleeSpells(false);
                pKrick->GetMotionMaster()->MoveIdle();
                pKrick->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (Creature* pJainaOrSylvanas = GetSingleCreatureFromStorage(m_Team == ALLIANCE ? NPC_JAINA_BEGIN : NPC_SILVANA_BEGIN))
                {
                    pJainaOrSylvanas->GetMotionMaster()->MovePoint(0, pKrick->GetPositionX(), pKrick->GetPositionY(), pKrick->GetPositionZ());
                }
            }
            break;
        }
        case SAY_OUTRO_KRICK_3:
        {
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                pTyrannus->GetMotionMaster()->MovePoint(0, GeneralLoc[4].x, GeneralLoc[4].y, GeneralLoc[4].z, false);
            break;
        }
        case SAY_OUTRO_KRICK_6_TYRANNUS:
        {
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                {
                    pTyrannus->CastSpell(pKrick, SPELL_STRANGULATING, false);
                }
            }
            break;
        }
        case SAY_OUTRO_KRICK_9_ALLY:
        case SAY_OUTRO_KRICK_9_HORDE:
        {
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                pKrick->DealDamage(pKrick, pKrick->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            break;
        }
        case EVENT_TYRANNUS_START:
        {
            ProcessEventNpcs(GetPlayerInMap(), EVENT_TYRANNUS_START);
            break;
        }
        case SAY_GAUNTLET_PREFIGHT_2:
        {
            if (Creature* pRimefang = GetSingleCreatureFromStorage(NPC_RIMEFANG))
            {
                if (pRimefang->GetVehicleKit())
                    pRimefang->GetVehicleKit()->RemoveAllPassengers();
                // TODO: Maybe move Rimefang some meters into sky
            }
            break;
        }
        case EVENT_TYRANNUS_END:
        {
            ProcessEventNpcs(GetPlayerInMap(), EVENT_TYRANNUS_END);
            // TODO: Check Fly Away from Rimefang
            if (Creature* pRimefang = m_pInstance->GetSingleCreatureFromStorage(NPC_RIMEFANG))
            {
                pRimefang->GetMotionMaster()->Clear();
                pRimefang->GetMotionMaster()->MovePoint(0, 844.752f, 358.993f, 645.330f);
                pRimefang->setFaction(35);
                pRimefang->DeleteThreatList();
                pRimefang->RemoveAllAuras();
                pRimefang->ForcedDespawn(10000);
            }
            //TODO: Move Sindragosa
            break;
        }
        case SAY_OUTRO_TYRANNUS_3_ALLY:
        case SAY_OUTRO_TYRANNUS_3_HORDE:
        {
            //TODO: teleport
            if (Creature* pSindragosa = GetSingleCreatureFromStorage(NPC_SINDRAGOSA))
                pSindragosa->CastSpell(pSindragosa, SPELL_FROST_BOMB, false);
            break;
        }
        default:
            break;
    }
}

void instance_pit_of_saron::SendTyrannusToMiddle()
{
    if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
    {
        pTyrannus->MonsterYell("Ich fliege zur Mitte ihr Schwächlinge", LANG_UNIVERSAL);
        pTyrannus->GetMotionMaster()->MovePoint(0, GeneralLoc[0].x, GeneralLoc[0].y, GeneralLoc[0].z, false);
        if (GetData(TYPE_KRICK) == DONE && GetData(TYPE_GARFROST) == DONE)
            pTyrannus->ForcedDespawn(30*IN_MILLISECONDS);
    }
}

InstanceData* GetInstanceData_instance_pit_of_saron(Map* pMap)
{
    return new instance_pit_of_saron(pMap);
}

void AddSC_instance_pit_of_saron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_pit_of_saron";
    pNewScript->GetInstanceData = &GetInstanceData_instance_pit_of_saron;
    pNewScript->RegisterSelf();
}
