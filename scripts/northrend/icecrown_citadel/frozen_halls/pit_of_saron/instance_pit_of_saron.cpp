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
    {783.565f, 112.559f, 509.461f},     // Relocation Point for Jaina or Sylvanas
    {860.649f, 124.863f, 536.019f},     // MovePoint for Tyrannus at Krick
};

static LocationsXYZ SummonLocForSlaves[]=
{
    {719.812f, -167.183f, 526.721f},
    {698.703f, -165.497f, 527.464f},
    {671.455f, -167.968f, 526.741f},
};

static LocationsXYZ MoveLocForSlaves[]=
{
    {677.445f, -186.521f, 526.702f},
    {708.190f, -194.619f, 526.805f},
    {687.257f, -193.644f, 526.717f},
};

static LocationsXYZ SummonLoc[]=
{
    {1060.955f, 107.274f, 628.424f},
    {1052.122f, 103.916f, 628.454f},
    {1068.363f, 110.432f, 629.009f},
};

static LocationsXYZ MoveLoc[]=
{
    {1019.006f, 129.684f, 628.156f}, 
    {1003.889f, 159.652f, 628.159f},
    {1015.389f, 183.650f, 628.156f},
    {1065.827f, 210.836f, 628.156f},
    {1072.659f, 204.432f, 628.156f},
};

struct sIntoEventNpcSpawnLocations
{
    uint32 uiEntryHorde, uiEntryAlliance;
    float fSpawnX, fSpawnY, fSpawnZ, fSpawnO;
};

const sIntoEventNpcSpawnLocations aEventBeginLocations[] =
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

const sIntoEventNpcSpawnLocations aEventEndLocations[]=
{
    {NPC_MARTIN_VICTUS_END, NPC_GORKUN_IRONSKULL_END          , 1060.983f, 94.954f, 630.997f, 2.247f},
    {NPC_JAINA_END        , NPC_SILVANA_END                   , 1065.983f, 94.954f, 630.997f, 2.247f},
};

enum
{
    // Intro
    SAY_SPEECH_SYLVANAS1    = -1658003,
    SAY_SPEECH_SYLVANAS2    = -1658007,
    SAY_SPEECH_SYLVANAS4    = -1658011,
    SAY_SPEECH_SYLVANAS5    = -1658013,

    SAY_SPEECH_JAINA1       = -1658002,
    SAY_SPEECH_JAINA2       = -1658006,
    SAY_SPEECH_JAINA3       = -1658009,
    SAY_SPEECH_JAINA4       = -1658010,
    SAY_SPEECH_JAINA5       = -1658012,

    SAY_TYRANNUS1           = -1658001,
    SAY_TYRANNUS2           = -1658004,
    SAY_TYRANNUS3           = -1658005,
    SAY_TYRANNUS4           = -1658008,

    // Garfrost
    SAY_FREE_SLAVE_HORDE                = -1658013,
    SAY_FREE_SLAVE_ALLY                 = -1658012,
    SAY_TYRANNUS_OUTRO                  = -1658020,

    // Ick and Krick
    SAY_OUTRO1              = -1658035,
    SAY_OUTRO2_ALLY         = -1658036,
    SAY_OUTRO2_HORDE        = -1658037,
    SAY_OUTRO3              = -1658038,
    SAY_OUTRO4_ALLY         = -1658039,
    SAY_OUTRO4_HORDE        = -1658040,
    SAY_OUTRO5              = -1658041,
    SAY_OUTRO6_TYRANNUS     = -1658042,
    SAY_OUTRO7              = -1658043,
    SAY_OUTRO8_TYRANNUS     = -1658044,
    SAY_OUTRO9_ALLY         = -1658045,
    SAY_OUTRO9_HORDE        = -1658046,

};

static const DialogueEntryTwoSide aPoSDialogues[] =
{
    // Intro Event
    {TYPE_INTRO, 0, 0, 0,                         4000},
    {SAY_TYRANNUS1, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_SPEECH_JAINA1, NPC_JAINA_BEGIN, SAY_SPEECH_SYLVANAS1, NPC_SILVANA_BEGIN,     5000},
    {SAY_TYRANNUS2, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_TYRANNUS3, NPC_TYRANNUS_INTRO, 0, 0,     5000},
    {SAY_SPEECH_JAINA2, NPC_JAINA_BEGIN, SAY_SPEECH_SYLVANAS2, NPC_SILVANA_BEGIN,     5000},
    {SAY_TYRANNUS4, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_SPEECH_JAINA3, NPC_JAINA_BEGIN, 0, 0,    4000}, // extra text for jaina Proudmoore
    {SAY_SPEECH_JAINA4, NPC_JAINA_BEGIN, SAY_SPEECH_SYLVANAS4, NPC_SILVANA_BEGIN,     7000},
    {SAY_SPEECH_JAINA5, NPC_JAINA_BEGIN, SAY_SPEECH_SYLVANAS5, NPC_SILVANA_BEGIN,     0},
    {0, 0, 0, 0, 0},
    {TYPE_GARFROST, 0, 0, 0,                      6000},
    {SAY_FREE_SLAVE_ALLY, NPC_MARTIN_VICTUS_SLAVE, SAY_FREE_SLAVE_HORDE, NPC_GORKUN_IRONSKULL_SLAVE, 13000},
    {SAY_TYRANNUS_OUTRO, NPC_TYRANNUS_INTRO, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {TYPE_KRICK, 0, 0, 0,                         3000},
    {SAY_OUTRO1, NPC_KRICK, 0, 0,                 15000},
    {SAY_OUTRO2_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO2_HORDE, NPC_SILVANA_BEGIN, 5000},
    {SAY_OUTRO3, NPC_KRICK, 0, 0,                 15000},
    {SAY_OUTRO4_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO4_HORDE, NPC_SILVANA_BEGIN, 5000},
    {SAY_OUTRO5, NPC_KRICK, 0, 0,                  4000},
    {SAY_OUTRO6_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0, 4000},
    {SAY_OUTRO7, NPC_KRICK, 0, 0,                  3000},
    {SAY_OUTRO8_TYRANNUS, NPC_TYRANNUS_INTRO, 0, 0, 10000},
    {SAY_OUTRO9_ALLY, NPC_JAINA_BEGIN, SAY_OUTRO9_HORDE, NPC_SILVANA_BEGIN, 0},
    {0, 0, 0, 0, 0},
};

instance_pit_of_saron::instance_pit_of_saron(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aPoSDialogues),
    m_Team(TEAM_NONE)
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
            pCreature->SetActiveObjectState(true);
            /* no break */
        case NPC_GARFROST:
        case NPC_KRICK:
        case NPC_KRICK_EVENT:
        case NPC_ICK:
        case NPC_TYRANNUS:
        case NPC_RIMEFANG:
        case NPC_SINDRAGOSA:
        case NPC_SLAVE_1:
        case NPC_JAINA_BEGIN:
        case NPC_JAINA_END:
        case NPC_SILVANA_BEGIN:
        case NPC_SILVANA_END:
        case NPC_DARK_RANGER_KALIRA:
        case NPC_ARCHMAGE_ELANDRA:
        case NPC_DARK_RANGER_LORALEN:
        case NPC_ARCHMAGE_KORELN:
        case NPC_MARTIN_VICTUS_SLAVE:
        case NPC_MARTIN_VICTUS_END:
        case NPC_GORKUN_IRONSKULL_SLAVE:
        case NPC_GORKUN_IRONSKULL_END:
           break;
        case NPC_COLISEUM_CHAMPION_A_P:
        case NPC_COLISEUM_CHAMPION_A_F:
        case NPC_COLISEUM_CHAMPION_A_M:
        case NPC_COLISEUM_CHAMPION_H_F:
        case NPC_COLISEUM_CHAMPION_H_T:
        case NPC_COLISEUM_CHAMPION_H_M:
            m_lGuardGuid.push_back(pCreature->GetObjectGuid());
            return;
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
    if (m_Team)
        return;

    if (!m_Team)                                          // very first player to enter
    {
        m_Team = pPlayer->GetTeam();
        SetDialogueSide(m_Team == ALLIANCE);
        ProcessEventNpcs(pPlayer, false);
        if (GetData(TYPE_INTRO) == NOT_STARTED)
            StartNextDialogueText(TYPE_INTRO);
    }
}

void instance_pit_of_saron::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GARFROST:
            if (uiData == DONE)
            {
                StartNextDialogueText(TYPE_GARFROST);
                if (GetData(TYPE_KRICK) == DONE)
                {
                    DoUseDoorOrButton(GO_ICEWALL);
                }
            }
            break;
        case TYPE_KRICK:
            if (uiData == DONE)
            {
                StartNextDialogueText(TYPE_KRICK);
                if (GetData(TYPE_GARFROST) == DONE)
                    DoUseDoorOrButton(GO_ICEWALL);
            }            
            break;
        case TYPE_TYRANNUS:
            if (uiData == DONE)
            {
                StartNextDialogueText(TYPE_TYRANNUS);
                ProcessEventNpcs(GetPlayerInMap(), true);
            }
            break;
        case TYPE_GAUNTLET:
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
    switch(uiType)
    {
        case TYPE_GARFROST:
        case TYPE_KRICK:
        case TYPE_TYRANNUS:
        case TYPE_INTRO:
        case TYPE_GAUNTLET:
            return m_auiEncounter[uiType];
            break;
        default:
            return 0;
    }
}

void instance_pit_of_saron::ProcessEventNpcs(Player* pPlayer, bool bChanged)
{
    if (!pPlayer)
        return;

    if (GetData(TYPE_KRICK) != DONE || GetData(TYPE_GARFROST) != DONE)
    {
        // Spawn Begin Mobs
        for (uint8 i = 0; i < sizeof(aEventBeginLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
        {
            if (Creature* pSummon = pPlayer->SummonCreature(m_Team == HORDE ? aEventBeginLocations[i].uiEntryHorde : aEventBeginLocations[i].uiEntryAlliance,
                                                            aEventBeginLocations[i].fSpawnX, aEventBeginLocations[i].fSpawnY, aEventBeginLocations[i].fSpawnZ, aEventBeginLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS))
                m_lEventMobGUIDs.push_back(pSummon->GetObjectGuid());
        }
    }
    else
    {
        // if bChanged, despawn Begin Mobs, spawn End Mobs at Spawn, else spawn EndMobs at End
        if (bChanged)
        {
            for (GuidList::const_iterator itr = m_lEventMobGUIDs.begin(); itr != m_lEventMobGUIDs.end(); ++itr)
            {
                if (Creature* pSummoned = instance->GetCreature(*itr))
                    pSummoned->ForcedDespawn();
            }

            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
            {
                pPlayer->SummonCreature(m_Team == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance, aEventEndLocations[i].fSpawnX, aEventEndLocations[i].fSpawnY, aEventEndLocations[i].fSpawnZ, aEventEndLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);

                // TODO: Let the NPCs Move along their paths
            }
        }
        else
        {   // Summon at end, without event
            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
            {
                pPlayer->SummonCreature(m_Team == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance, aEventEndLocations[i].fSpawnX, aEventEndLocations[i].fSpawnY, aEventEndLocations[i].fSpawnZ, aEventEndLocations[i].fSpawnO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);

                // TODO: Let the NPCs Move along their paths
            }
        }
    }
}

void instance_pit_of_saron::JustDidDialogueStep(int32 iEntry)
{
    switch (iEntry)
    {
        case TYPE_INTRO:
            SetData(TYPE_INTRO, IN_PROGRESS);
            break;
        case SAY_SPEECH_JAINA1:
        case SAY_SPEECH_SYLVANAS1:
            for (GuidList::iterator i = m_lGuardGuid.begin(); i != m_lGuardGuid.end(); ++i)
            {
                if (Creature *pTemp = instance->GetCreature(*i))
                {
                    pTemp->SetWalk(false);
                    pTemp->GetMotionMaster()->MovePoint(0, GeneralLoc[2].x + irand(-10, 10), GeneralLoc[2].y + irand(-10, 10), GeneralLoc[2].z);
                }
            }
            break;
        case SAY_TYRANNUS3:
            for (GuidList::iterator i = m_lGuardGuid.begin(); i != m_lGuardGuid.end(); ++i)
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
        case SAY_TYRANNUS4:
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
            {
                pTyrannus->CastSpell(pTyrannus, SPELL_NECROTIC_POWER, false);
            }
            for (GuidList::iterator i = m_lGuardGuid.begin(); i != m_lGuardGuid.end(); ++i)
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
        case SAY_SPEECH_JAINA5:
        case SAY_SPEECH_SYLVANAS5:
            SetData(TYPE_INTRO,DONE);
            SendTyrannusToMiddle();
            break;
        case TYPE_GARFROST:
        {
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                pTyrannus->GetMotionMaster()->MovePoint(0, GeneralLoc[3].x, GeneralLoc[3].y, GeneralLoc[3].z, false);
            SummonSlaves();
            break;
        }
        case SAY_TYRANNUS_OUTRO:
            SendTyrannusToMiddle();
            break;
        case TYPE_KRICK:
        {
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
                    instance->CreatureRelocation(pJainaOrSylvanas, GeneralLoc[4].x, GeneralLoc[4].y, GeneralLoc[4].z, 0.0f);
                    pJainaOrSylvanas->GetMotionMaster()->MovePoint(0, pKrick->GetPositionX() - 15, pKrick->GetPositionY(), pKrick->GetPositionZ());
                }
            }
            break;
        }
        case SAY_OUTRO3:
        {
            if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
                pTyrannus->GetMotionMaster()->MovePoint(0, GeneralLoc[5].x, GeneralLoc[5].y, GeneralLoc[5].z, false);
            break;
        }
        case SAY_OUTRO9_ALLY:
        case SAY_OUTRO9_HORDE:
        {
            if (Creature* pKrick = GetSingleCreatureFromStorage(NPC_KRICK))
            {
                pKrick->DealDamage(pKrick, pKrick->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            break;
        }
        case TYPE_TYRANNUS:
        {
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
    }
}

void instance_pit_of_saron::SummonSlaves()
{
    if (Creature* pTyrannus = GetSingleCreatureFromStorage(NPC_TYRANNUS_INTRO))
    {
        if (m_Team == ALLIANCE)
        {
            pTyrannus->SummonCreature(NPC_MARTIN_VICTUS_SLAVE, 695.46f, -156.31f, 528.061f, 4.77f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
            for (uint8 i = 0; i < 5; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_ALLY_1, SummonLocForSlaves[0].x + urand(0, 20), SummonLocForSlaves[0].y + urand(0, 20), SummonLocForSlaves[0].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[0].x + urand(0, 20), MoveLocForSlaves[0].y + urand(0, 20), MoveLocForSlaves[0].z);
            }

            for (uint8 i = 5; i < 10; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_ALLY_2, SummonLocForSlaves[1].x + urand(0, 10), SummonLocForSlaves[1].y - urand(0, 10), SummonLocForSlaves[1].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[2].x + urand(0, 20), MoveLocForSlaves[2].y - urand(0, 20), MoveLocForSlaves[2].z);
            }

            for (uint8 i = 10; i < 15; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_ALLY_3, SummonLocForSlaves[2].x - urand(0, 20), SummonLocForSlaves[2].y - urand(0, 20), SummonLocForSlaves[2].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[1].x - urand(0, 20), MoveLocForSlaves[1].y - urand(0, 20), MoveLocForSlaves[1].z);
            }
        }
        else
        {
            pTyrannus->SummonCreature(NPC_GORKUN_IRONSKULL_SLAVE, 695.46f, -156.31f, 528.061f, 4.77f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
            for (uint8 i = 0; i < 5; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_HORDE_1, SummonLocForSlaves[0].x + urand(0, 20), SummonLocForSlaves[0].y + urand(0, 20), SummonLocForSlaves[0].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[0].x + urand(0, 20), MoveLocForSlaves[0].y + urand(0, 20), MoveLocForSlaves[0].z);
            }

            for (uint8 i = 5; i < 10; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_HORDE_2, SummonLocForSlaves[1].x + urand(0, 10), SummonLocForSlaves[1].y - urand(0, 10), SummonLocForSlaves[1].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[2].x + urand(0, 20), MoveLocForSlaves[2].y - urand(0, 20), MoveLocForSlaves[2].z);
            }

            for (uint8 i = 10; i < 15; i++)
            {
                if (Creature *pTemp = pTyrannus->SummonCreature(NPC_SLAVE_HORDE_3, SummonLocForSlaves[2].x - urand(0, 20), SummonLocForSlaves[2].y - urand(0, 20), SummonLocForSlaves[2].z, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLocForSlaves[1].x - urand(0, 20), MoveLocForSlaves[1].y - urand(0, 20), MoveLocForSlaves[1].z);
            }
        }
    }
}

void instance_pit_of_saron::SummonSlavesEnd()
{
    if (Player* pPlayer = GetPlayerInMap())
    {
        if (m_Team == HORDE)
        {
             Creature *pLoralen = pPlayer->SummonCreature(NPC_DARK_RANGER_LORALEN, SummonLoc[0].x, SummonLoc[0].y, SummonLoc[0].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pLoralen)
                pLoralen->GetMotionMaster()->MovePoint(0, MoveLoc[3].x, MoveLoc[3].y, MoveLoc[3].z);
            Creature *pKelira = pPlayer->SummonCreature(NPC_DARK_RANGER_KALIRA, SummonLoc[1].x, SummonLoc[1].y, SummonLoc[1].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pKelira)
                pKelira->GetMotionMaster()->MovePoint(0, MoveLoc[4].x, MoveLoc[4].y, MoveLoc[4].z);

            for (uint8 i = 0; i < 5; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_HORDE_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }

            for (uint8 i = 5; i < 10; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_HORDE_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }

            for (uint8 i = 10; i < 15; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_HORDE_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }
        }
        else
        {
            Creature *pElandra = pPlayer->SummonCreature(NPC_ARCHMAGE_ELANDRA, SummonLoc[0].x, SummonLoc[0].y, SummonLoc[0].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pElandra)
                pElandra->GetMotionMaster()->MovePoint(0, MoveLoc[3].x, MoveLoc[3].y, MoveLoc[3].z);
            Creature *pKoreln = pPlayer->SummonCreature(NPC_ARCHMAGE_KORELN, SummonLoc[1].x, SummonLoc[1].y, SummonLoc[1].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pKoreln)
                pKoreln->GetMotionMaster()->MovePoint(0, MoveLoc[4].x, MoveLoc[4].y, MoveLoc[4].z);

            for (uint8 i = 0; i < 5; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_ALLY_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }

            for (uint8 i = 5; i < 10; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_ALLY_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }

            for (uint8 i = 10; i < 15; i++)
            {
                Creature *pTemp = pPlayer->SummonCreature(NPC_SLAVE_ALLY_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
                if (pTemp)
                {
                    pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
                    m_lGuards.push_back(pTemp->GetObjectGuid());
                }
            }
        }
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
