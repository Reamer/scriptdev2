/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: instance_oculus
SD%Complete: 70%
SDComment:
SDAuthor: Check Azure Ring Captain handling in Varos encounter. (We have only one entry for this NPC, 4 Scripted EventId's
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

/* The Occulus encounters:
0 - Drakos the Interrogator
1 - Varos Cloudstrider
2 - Mage-Lord Urom
3 - Ley-Guardian Eregos */

instance_oculus::instance_oculus(Map* pMap) : ScriptedInstance(pMap),
    m_uiCentrifugeDead(0)
{
    Initialize();
}


void instance_oculus::Initialize()
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        m_auiEncounter[i] = NOT_STARTED;
}

void instance_oculus::OnObjectCreate(GameObject* pGO)
{
    switch(pGO->GetEntry())
    {
        case GO_DRAGON_CAGE_DOOR:
        case GO_EREGOS_CACHE:
        case GO_EREGOS_CACHE_H:
        case GO_SPOTLIGHT:
        case GO_ORB_OF_NEXUS:
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGO->GetEntry()] = pGO->GetObjectGuid();
}

void instance_oculus::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_VAROS:
        case NPC_UROM:
        case NPC_EREGOS:
            pCreature->SetActiveObjectState(true);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            /* no break */
        case NPC_DRAKOS:
        case NPC_BALGAR_IMAGE:
            break;
        case NPC_VAROS_CORE:
            m_lVarosCoreList.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_AZURE_RING_CAPTAIN:
        {
            AzureRingCaptain azureRingCaptain;
            azureRingCaptain.guid = pCreature->GetObjectGuid();
            azureRingCaptain.SummonBeamTimer = 0;
            m_lAzureRingCaptainList.push_back(azureRingCaptain);
            return;
        }
        default:
            return;
    }
    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_oculus::OnCreatureDeath(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_CENTRIFUGE_CONSTRUCT:
        {
            ++m_uiCentrifugeDead;            
            CheckVarosCentrifugeConstructState();
            break;
        }
        default:
            break;
    }
}

void instance_oculus::OnPlayerEnter(Player* pPlayer)
{
    CheckVarosCentrifugeConstructState();
}

void instance_oculus::SetData(uint32 uiType, uint32 uiData)
{
    m_auiEncounter[uiType] = uiData;
    switch(uiType)
    {
        case TYPE_DRAKOS:
            if (uiData == DONE)
            {
                if (!instance->IsRegularDifficulty())
                    DoStartTimedAchievement(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEV_MAKE_IT_COUNT);
                CheckVarosCentrifugeConstructState();
            }
            break;
        case TYPE_VAROS:
            if (uiData == DONE)
            {
                if (Creature* pUrom = GetSingleCreatureFromStorage(NPC_UROM))
                {
                    pUrom->InterruptNonMeleeSpells(false);
                    pUrom->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }
            break;
        case TYPE_UROM:
            if (uiData == DONE)
            {
                if (Creature* pEregos = GetSingleCreatureFromStorage(NPC_EREGOS))
                {
                    pEregos->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }
            break;
        case TYPE_EREGOS:
            if (uiData == DONE)
            {
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_EREGOS_CACHE : GO_EREGOS_CACHE_H, 30*MINUTE);
                DoRespawnGameObject(GO_SPOTLIGHT, 30*MINUTE);
            }
            break;        
        default:
            break;
    }

    // Saving also SPECIAL for this instance
    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_oculus::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];
    return 0;
}

void instance_oculus::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

Creature* instance_oculus::GetFarestVaromOrb()
{
    Creature* pFarestAwayCore = NULL;
    float distance = 100.0f;
    if (Creature* pVaros = GetSingleCreatureFromStorage(NPC_VAROS))
    {
        for (GuidList::const_iterator itr = m_lVarosCoreList.begin(); itr != m_lVarosCoreList.end(); ++itr)
        {
            if (Creature* pCore = instance->GetCreature(*itr))
            {
                float tempDistance = pVaros->GetDistance(pCore);
                if (tempDistance < distance)
                {
                    pFarestAwayCore = pCore;
                    distance = tempDistance;
                }
            }
        }
    }
    return pFarestAwayCore;
}

void instance_oculus::DoProcessCallDragonEvent()
{
    for (std::list<AzureRingCaptain>::iterator itr = m_lAzureRingCaptainList.begin(); itr != m_lAzureRingCaptainList.end(); ++itr)
    {
        AzureRingCaptain azureRingCaptain = *itr;
        if (!azureRingCaptain.SummonBeamTimer)
        {
            if (Creature* pDragon = instance->GetCreature(azureRingCaptain.guid))
            {
                pDragon->GetMotionMaster()->MovePoint(1, VAROS_MIDDLE_X + irand(-30, 30), VAROS_MIDDLE_Y + irand(-30, 30), VAROS_MIDDLE_Z + 30.0f, false);
                azureRingCaptain.SummonBeamTimer = 10000;
            }
        }
    }
}

void instance_oculus::ResetDragons()
{
    for (std::list<AzureRingCaptain>::iterator itr = m_lAzureRingCaptainList.begin(); itr != m_lAzureRingCaptainList.end(); ++itr)
    {
        AzureRingCaptain azureRingCaptain = *itr;
        azureRingCaptain.SummonBeamTimer = 0;
        if (Creature* pDragon = instance->GetCreature(azureRingCaptain.guid))
        {
            pDragon->AI()->EnterEvadeMode();
        }
    }
}

void instance_oculus::Update(const uint32 uiDiff)
{
    for (std::list<AzureRingCaptain>::iterator itr = m_lAzureRingCaptainList.begin(); itr != m_lAzureRingCaptainList.end(); ++itr)
    {
        AzureRingCaptain azureRingCaptain = *itr;
        if (!azureRingCaptain.SummonBeamTimer)
        {
            if (azureRingCaptain.SummonBeamTimer < uiDiff)
            {
                if (Creature* pDragon = instance->GetCreature(azureRingCaptain.guid))
                {
                    pDragon->CastSpell(pDragon, SPELL_SUMMON_BEAM, false);
                    azureRingCaptain.SummonBeamTimer = 15000;
                }
            }
            else
                azureRingCaptain.SummonBeamTimer -= uiDiff;
        }
    }
}

void instance_oculus::CheckVarosCentrifugeConstructState()
{
    if (GetData(TYPE_DRAKOS) == DONE && GetData(TYPE_VAROS) != DONE)
    {
        DoUpdateWorldState(UI_STATE_CONSTRUCT_SHOW, 1);
        DoUpdateWorldState(UI_STATE_CONSTRUCT_COUNT, MAX_CONSTRUCTS - m_uiCentrifugeDead);

        if (Creature* pVaros = GetSingleCreatureFromStorage(NPC_VAROS))
        {
            if (MAX_CONSTRUCTS - m_uiCentrifugeDead)
            {
                pVaros->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (pVaros->HasAura(SPELL_ARCANE_SHIELD))
                    pVaros->CastSpell(pVaros, SPELL_ARCANE_SHIELD, false);
            }
            else
            {
                pVaros->RemoveAurasDueToSpell(SPELL_ARCANE_SHIELD);
                pVaros->InterruptNonMeleeSpells(false);
                pVaros->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoScriptText(SAY_SHIELD_DOWN, pVaros);
            }
        }
    }
}

InstanceData* GetInstanceData_instance_oculus(Map* pMap)
{
    return new instance_oculus(pMap);
}

/*### 
# Oculus Orb 
-####*/ 
bool GOUse_go_oculus_portal(Player* pPlayer, GameObject* pGo) 
{ 
    switch(pGo->GetEntry()) 
    {
    case GO_ORB_OF_NEXUS: 
        pPlayer->TeleportTo(571,3876.159912f,6984.439941f,106.32f,6.279f); 
        return true; 
    } 
    return false; 
}


void AddSC_instance_oculus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_oculus";
    newscript->GetInstanceData = &GetInstanceData_instance_oculus;
    newscript->RegisterSelf();

    newscript = new Script; 
    newscript->Name = "go_oculus_portal"; 
    newscript->pGOUse = GOUse_go_oculus_portal; 
    newscript->RegisterSelf();
}