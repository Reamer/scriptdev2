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
SDName: instance_forge_of_souls
SD%Complete: 90%
SDComment: TODO: Movement of the extro-event is missing, implementation unclear!
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    SAY_JAINA_FS01             = -1632040,
    SAY_JAINA_FS02             = -1632041,
    SAY_JAINA_FS03             = -1632042,
    SAY_JAINA_FS04             = -1632043,
    SAY_JAINA_FS05             = -1632044,
    SAY_JAINA_FS06             = -1632045,
    SAY_JAINA_FS07             = -1632046,
    SAY_JAINA_FS08             = -1632047,
    
    SAY_SYLVANA_FS01           = -1632050,
    SAY_SYLVANA_FS02           = -1632051,
    SAY_SYLVANA_FS03           = -1632052,
    SAY_SYLVANA_FS04           = -1632053,
    SAY_SYLVANA_FS05           = -1632054,
    SAY_SYLVANA_FS06           = -1632055,

    SAY_JAINA_FS09_EXTRO       = -1632029,
    SAY_SYLVANA_FS07_EXTRO     = -1632030,

    GOSSIP_SPEECHINTRO         = 13525,
};

static const DialogueEntryTwoSide aHoSDialogues[] =
{
    // Intro Event
    {TYPE_INTRO, 0, 0, 0,                         4000},
    {SAY_JAINA_FS01, NPC_JAINA_BEGIN, SAY_SYLVANA_FS01, NPC_SILVANA_BEGIN, 7000},
    {SAY_JAINA_FS02, NPC_JAINA_BEGIN, SAY_SYLVANA_FS02, NPC_SILVANA_BEGIN, 12000},
    {SAY_JAINA_FS03, NPC_JAINA_BEGIN, SAY_SYLVANA_FS03, NPC_SILVANA_BEGIN, 11000},
    {SAY_JAINA_FS04, NPC_JAINA_BEGIN, SAY_SYLVANA_FS04, NPC_SILVANA_BEGIN, 11000},
    {SAY_JAINA_FS05, NPC_JAINA_BEGIN, SAY_SYLVANA_FS05, NPC_SILVANA_BEGIN, 12000},
    {SAY_JAINA_FS06, NPC_JAINA_BEGIN, SAY_SYLVANA_FS06, NPC_SILVANA_BEGIN, 12000},
    {SAY_JAINA_FS07, NPC_JAINA_BEGIN, 0, 0, 7000},
    {SAY_JAINA_FS08, NPC_JAINA_BEGIN, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {TYPE_OUTRO, 0, 0, 0,                         3000},
    {SAY_JAINA_FS09_EXTRO, NPC_JAINA_END, SAY_SYLVANA_FS07_EXTRO, NPC_SILVANA_END, 0},
    {0, 0, 0, 0, 0},
};

instance_forge_of_souls::instance_forge_of_souls(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aHoSDialogues),
    m_bCriteriaPhantomBlastFailed(false),
    m_uiTeam(0)
{
    Initialize();
}

void instance_forge_of_souls::Initialize()
{
    InitializeDialogueHelper(this);
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        m_auiEncounter[i] = NOT_STARTED;
}

void instance_forge_of_souls::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_DEVOURER_OF_SOULS:
        case NPC_BRONJAHM:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;

        case NPC_CORRUPTED_SOUL_FRAGMENT:
            m_luiSoulFragmentAliveGUIDs.push_back(pCreature->GetObjectGuid());
            break;
    }
}

void instance_forge_of_souls::OnPlayerEnter(Player* pPlayer)
{
    if (!m_uiTeam)                                          // very first player to enter
    {
        m_uiTeam = pPlayer->GetTeam();
        SetDialogueSide(m_uiTeam == ALLIANCE);
        ProcessEventNpcs(pPlayer, false);
    }
}

void instance_forge_of_souls::ProcessEventNpcs(Player* pPlayer, bool bChanged)
{
    if (!pPlayer)
        return;

    if (m_auiEncounter[0] != DONE || m_auiEncounter[1] != DONE)
    {
        // Spawn Begin Mobs
        for (uint8 i = 0; i < sizeof(aEventBeginLocations)/sizeof(sIntoEventNpcSpawnLocations); ++i)
        {
            if (Creature* pSummon = pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventBeginLocations[i].uiEntryHorde : aEventBeginLocations[i].uiEntryAlliance,
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

            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sExtroEventNpcLocations); ++i)
            {
                pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance,
                                        aEventEndLocations[i].fSpawnX, aEventEndLocations[i].fSpawnY, aEventEndLocations[i].fSpawnZ, aEventEndLocations[i].fStartO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);

                // TODO: Let the NPCs Move along their paths
            }
        }
        else
        {   // Summon at end, without event
            for (uint8 i = 0; i < sizeof(aEventEndLocations)/sizeof(sExtroEventNpcLocations); ++i)
            {
                pPlayer->SummonCreature(m_uiTeam == HORDE ? aEventEndLocations[i].uiEntryHorde : aEventEndLocations[i].uiEntryAlliance,
                                        aEventEndLocations[i].fEndX, aEventEndLocations[i].fEndY, aEventEndLocations[i].fEndZ, aEventEndLocations[i].fEndO, TEMPSUMMON_DEAD_DESPAWN, 24*HOUR*IN_MILLISECONDS);
            }
        }
    }
}

bool instance_forge_of_souls::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_SOUL_POWER:
            return m_luiSoulFragmentAliveGUIDs.size() >= 4;
        case ACHIEV_CRIT_PHANTOM_BLAST:
            return !m_bCriteriaPhantomBlastFailed;
        default:
            return 0;
    }
}

void instance_forge_of_souls::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_INTRO:
        case TYPE_OUTRO:
            if (uiData == SPECIAL)
                StartNextDialogueText(uiType);
            break;
        case TYPE_BRONJAHM:
            m_auiEncounter[0] = uiData;

            // Despawn remaining adds and clear list
            for (GuidList::const_iterator itr = m_luiSoulFragmentAliveGUIDs.begin(); itr != m_luiSoulFragmentAliveGUIDs.end(); ++itr)
            {
                if (Creature* pFragment = instance->GetCreature(*itr))
                    pFragment->ForcedDespawn();
            }
            m_luiSoulFragmentAliveGUIDs.clear();
            break;
        case TYPE_DEVOURER_OF_SOULS:
            m_auiEncounter[1] = uiData;
            if (uiData == DONE)
                ProcessEventNpcs(GetPlayerInMap(), true);
            break;
        case TYPE_ACHIEV_PHANTOM_BLAST:
            m_bCriteriaPhantomBlastFailed = (uiData == FAIL);
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_forge_of_souls::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_forge_of_souls::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_BRONJAHM:
            return m_auiEncounter[0];
        case TYPE_DEVOURER_OF_SOULS:
            return m_auiEncounter[1];
        default:
            return 0;
    }
}

void instance_forge_of_souls::SetData64(uint32 uiType, uint64 uiData)
{
    if (uiType == DATA_SOULFRAGMENT_REMOVE)
        m_luiSoulFragmentAliveGUIDs.remove(ObjectGuid(uiData));
}

void instance_forge_of_souls::JustDidDialogueStep(int32 iEntry)
{
    switch(iEntry)
    {
        case TYPE_INTRO:
        {
            SetData(TYPE_INTRO, IN_PROGRESS);
            break;
        }
        case SAY_JAINA_FS08:
        case SAY_SYLVANA_FS06:
        {
            SetData(TYPE_INTRO, DONE);
            break;
        }
        default:
            break;
    }
}

InstanceData* GetInstanceData_instance_forge_of_souls(Map* pMap)
{
    return new instance_forge_of_souls(pMap);
}

void AddSC_instance_forge_of_souls()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_forge_of_souls";
    pNewScript->GetInstanceData = &GetInstanceData_instance_forge_of_souls;
    pNewScript->RegisterSelf();
}
