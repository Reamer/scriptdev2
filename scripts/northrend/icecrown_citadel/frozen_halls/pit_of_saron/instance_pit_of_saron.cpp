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
    {695.46f, -156.31f, 546.061f}       // MovePoint for Tyrannus at Garfrost
};

static LocationsXYZ SummonLocForSlaves[]=
{
    {719.812f, -167.183f, 526.721f,},
    {698.703f, -165.497f, 527.464f,},
    {671.455f, -167.968f, 526.741f,},
};

static LocationsXYZ MoveLocForSlaves[]=
{
    {677.445f, -186.521f, 526.702f},
    {708.190f, -194.619f, 526.805f},
    {687.257f, -193.644f, 526.717f},
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

};

static const DialogueEntryTwoSide aPoSDialogues[] =
{
    // Intro Event
    {TYPE_INTRO, 0, 0, 0,                         4000},
    {SAY_TYRANNUS1, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_SPEECH_JAINA1, NPC_JAINA_PART1, SAY_SPEECH_SYLVANAS1, NPC_SYLVANAS_PART1,     5000},
    {SAY_TYRANNUS2, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_TYRANNUS3, NPC_TYRANNUS_INTRO, 0, 0,     5000},
    {SAY_SPEECH_JAINA2, NPC_JAINA_PART1, SAY_SPEECH_SYLVANAS2, NPC_SYLVANAS_PART1,     5000},
    {SAY_TYRANNUS4, NPC_TYRANNUS_INTRO, 0, 0,     7000},
    {SAY_SPEECH_JAINA3, NPC_JAINA_PART1, 0, 0,    4000}, // extra text for jaina Proudmoore
    {SAY_SPEECH_JAINA4, NPC_JAINA_PART1, SAY_SPEECH_SYLVANAS4, NPC_SYLVANAS_PART1,     7000},
    {SAY_SPEECH_JAINA5, NPC_JAINA_PART1, SAY_SPEECH_SYLVANAS5, NPC_SYLVANAS_PART1,     0},
    {0, 0, 0, 0, 0},
    {TYPE_GARFROST, 0, 0, 0,                      6000},
    {SAY_FREE_SLAVE_ALLY, NPC_MARTIN_VICTUS_SLAVE, SAY_FREE_SLAVE_HORDE, NPC_GORKUN_IRONSKULL_SLAVE, 13000},
    {SAY_TYRANNUS_OUTRO, NPC_TYRANNUS_INTRO, 0, 0, 0},
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
        case NPC_JAINA_PART1:
        case NPC_JAINA_PART2:
        case NPC_SYLVANAS_PART1:
        case NPC_SYLVANAS_PART2:
        case NPC_KALIRA:
        case NPC_ELANDRA:
        case NPC_LORALEN:
        case NPC_KORELN:
        case NPC_MARTIN_VICTUS_SLAVE:
        case NPC_MARTIN_VICTUS_END:
        case NPC_GORKUN_IRONSKULL_SLAVE:
        case NPC_GORKUN_IRONSKULL_END:
           break;
        case NPC_CHAMPION_1_HORDE:
        case NPC_CHAMPION_2_HORDE:
        case NPC_CHAMPION_3_HORDE:
        case NPC_CHAMPION_1_ALLIANCE:
        case NPC_CHAMPION_2_ALLIANCE:
        case NPC_CHAMPION_3_ALLIANCE:
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

    m_Team = pPlayer->GetTeam();
    // TODO: Spawn Horde NPC and then change this here
    // SetDialogueSide(m_uiTeam == ALLIANCE);
    SetDialogueSide(true);
    if (GetData(TYPE_INTRO) == NOT_STARTED)
        StartNextDialogueText(TYPE_INTRO);
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
            if (uiData == DONE && m_auiEncounter[TYPE_GARFROST] == DONE)
                DoUseDoorOrButton(GO_ICEWALL);
            break;
        case TYPE_TYRANNUS:
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
