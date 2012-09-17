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
SDName: Storm_Peaks
SD%Complete: 0
SDComment: Placeholder
SDCategory: Storm Peaks
EndScriptData */

/* ContentData
quest support: 12856, 12983
EndContentData */

#include "precompiled.h"

enum{
    SPELL_RIDE = 55074,
    SPELL_SUMMON_FREE_PRISONER  = 55073,
    NPC_ICE_PRISONER = 29639
};

#define END_X 7077.414551f
#define END_Y -1910.627808f
#define END_Z 784.244629f
#define END_2_X 6827.765137f
#define END_2_Y -1702.224854f
#define END_2_Z 820.558655f

struct MANGOS_DLL_DECL npc_freeDragonAI : public ScriptedAI
{
    npc_freeDragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiCheckRangeTimer;

    uint8 boarded;


    void Reset()
    {
        boarded = 0;
        m_uiCheckRangeTimer = 2000;
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
    }

    void EnterEvadeMode()
    {
    }

    void AttackStart()
    {
    }

    void JustSummoned(Creature* pSummoned)
    {
        boarded++;
        pSummoned->EnterVehicle(m_creature->GetVehicleKit());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCheckRangeTimer < uiDiff)
        {
            if (m_creature->GetDistance(END_X, END_Y, END_Z) < 30)
            {
                if( VehicleKitPtr vehicle = m_creature->GetVehicleKit())
                {
                    if (Unit* pPlayer = vehicle->GetPassenger(0))
                    {
                        if (pPlayer->GetTypeId() == TYPEID_PLAYER)
                        {
                            for (int8 i = 1; i < 4; ++i)
                            {
                                if (Unit* pPrisoner = vehicle->GetPassenger(i))
                                {
                                    pPlayer->DealDamage(pPrisoner, pPrisoner->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                                }
                            }
                            pPlayer->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
                    }
                }
            }
            else
            {
                if (boarded < 3)
                {
                    std::list<Creature*> pPrisonerList;
                    GetCreatureListWithEntryInGrid(pPrisonerList, m_creature, NPC_ICE_PRISONER, 30.0f);
                    if (!pPrisonerList.empty())
                    {
                        for(std::list<Creature*>::iterator iter = pPrisonerList.begin(); iter != pPrisonerList.end(); ++iter)
                        {
                            if (Creature* pPrisoner = (*iter))
                            {
                                if (pPrisoner->isAlive())
                                {
                                    DoCast(pPrisoner, SPELL_SUMMON_FREE_PRISONER);
                                    pPrisoner->ForcedDespawn();
                                }
                            }
                        }
                    }
                }
            }
            m_uiCheckRangeTimer = 2000;
        }
        else
            m_uiCheckRangeTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_freeDragon(Creature* pCreature)
{
    return new npc_freeDragonAI(pCreature);
}

struct MANGOS_DLL_DECL npc_lastIceBearAI : public ScriptedAI
{
    npc_lastIceBearAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    ObjectGuid pPlayerGuid;
    uint32 m_uiCheckRangeTimer;

    void Reset()
    {
        m_uiCheckRangeTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCheckRangeTimer < uiDiff)
        {
            if (m_creature->GetDistance(END_2_X, END_2_Y, END_2_Z) < 20)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(pPlayerGuid))
                {
                    pPlayer->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }
            m_uiCheckRangeTimer = 2000;
        }
        else
            m_uiCheckRangeTimer -= uiDiff;
    }
};

bool GossipHello_npc_lastIceBear(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(12983) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ich will dich retten! Folge mir bitte.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    }
    return true;
}

bool GossipSelect_npc_lastIceBear(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pCreature->GetMotionMaster()->MoveFollow(pPlayer, 3.0f, 0.0f);
        if (npc_lastIceBearAI* plastIceBearAI = dynamic_cast<npc_lastIceBearAI*> (pCreature->AI()))
            plastIceBearAI->pPlayerGuid = pPlayer->GetObjectGuid();
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}


CreatureAI* GetAI_npc_lastIceBear(Creature* pCreature)
{
    return new npc_lastIceBearAI(pCreature);
}

void AddSC_storm_peaks()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_freeDragon";
    pNewScript->GetAI = &GetAI_npc_freeDragon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lastIceBear";
    pNewScript->GetAI = &GetAI_npc_lastIceBear;
    pNewScript->pGossipHello = &GossipHello_npc_lastIceBear;
    pNewScript->pGossipSelect = &GossipSelect_npc_lastIceBear;
    pNewScript->RegisterSelf();

}
