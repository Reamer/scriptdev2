/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Copyright (C) 2011 MangosR2
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

#include "precompiled.h"
#include "culling_of_stratholme.h"
#include "escort_ai.h"
#include "WorldPacket.h"

/*###
## npc_mike
###*/

enum
{
   SAY_MIKE01                = -1597270,
   SAY_FORRESTER02           = -1597271,
   SAY_JAMES03               = -1597272,
   SAY_SIABI04               = -1597273,
   SAY_MIKE05                = -1597274,
   SAY_CORICKS06             = -1597275,
   SAY_GRIAN07               = -1597276,
   SAY_CORICKS08             = -1597277,
   SAY_JAMES09               = -1597278,
   SAY_FORRESTER10           = -1597279,

   EMOTE_SHOT                = 5,
   EMOTE_TALK                = 1,
   EMOTE_POINT               = 25,
   EMOTE_NO                  = 274,
   EMOTE_LAUGH               = 11
};

struct MANGOS_DLL_DECL npc_mikeAI : public ScriptedAI
{
    npc_mikeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    instance_culling_of_stratholme* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
        if (m_uiPhase != 2)
        {
            m_uiStep = 0;
            m_uiStepTimer = 100;
            m_uiPhase = 0;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!who)
            return;

        if (!m_pInstance) return;

        if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 15 && who->GetPositionZ() > 99.50f && m_uiPhase == 0)
        {
            m_creature->SetGuidValue(UNIT_FIELD_TARGET, who->GetObjectGuid());
            m_uiPhase = 1;
        }
    }

    void TavernEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_MIKE01, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 1:
              m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
              m_uiStepTimer = 5000;
              break;
           case 2:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_HEARTHSINGER_FORRESTEN))
                 DoScriptText(SAY_FORRESTER02, pTemp);
              m_uiStepTimer = 6000;
              break;
           case 3:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FOOTMAN_JAMES))
                 DoScriptText(SAY_JAMES03, pTemp);
              m_uiStepTimer = 5000;
              break;
           case 4:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FRAS_SIABI))
                 DoScriptText(SAY_SIABI04, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 5:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FRAS_SIABI))
                 pTemp->HandleEmoteCommand(EMOTE_SHOT);
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 5000;
              break;
           case 6:
              m_creature->GetMotionMaster()->MovePoint(0, 1554.849f, 588.465f, 99.775f);
              m_uiStepTimer = 3000;
              break;
           case 7:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FRAS_SIABI))
                 pTemp->HandleEmoteCommand(EMOTE_LAUGH);
              m_uiStepTimer = 3000;
              break;
           case 8:
              DoScriptText(SAY_MIKE05, m_creature);
              m_uiStepTimer = 2000;
              break;
           case 9:
              m_creature->HandleEmoteCommand(EMOTE_SHOT);
              m_uiStepTimer = 1000;
              break;
           case 10:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_MAL_CORRICKS))
                 DoScriptText(SAY_CORICKS06, pTemp);
              m_uiStepTimer = 4000;
              break;
           case 11:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_MAL_CORRICKS))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_GRYAN_STOUTMANTLE))
                 DoScriptText(SAY_GRIAN07, pTemp);
              m_uiStepTimer = 11000;
              break;
           case 12:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_MAL_CORRICKS))
                 DoScriptText(SAY_CORICKS08, pTemp);
              m_creature->GetMotionMaster()->MovePoint(0, 1549.609f, 575.544f, 100.052f);
              m_uiStepTimer = 2000;
              break;
           case 13:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FOOTMAN_JAMES))
                 DoScriptText(SAY_JAMES09, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 14:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_FOOTMAN_JAMES))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              m_uiStepTimer = 5000;
              break;
           case 15:
              if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_HEARTHSINGER_FORRESTEN))
                 DoScriptText(SAY_FORRESTER10, pTemp);
              m_uiPhase = 2;
              break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if (m_uiPhase == 1)
       {
            if (m_uiStepTimer < uiDiff)
            {
               TavernEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }
    }
};

/*###
## npc_bartleby
###*/

struct MANGOS_DLL_DECL npc_bartleby_csAI : public ScriptedAI
{
    npc_bartleby_csAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        m_uiPhase = 0;
        Reset();
    }

    instance_culling_of_stratholme* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 4)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!who)
            return;

        if (!m_pInstance) return;

        if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 20 && m_uiPhase == 0)
        {
            m_uiPhase = 1;
        }
    }

    void SpeechEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_BARTLEBY01, m_creature);
              m_uiStepTimer = 5000;
              break;
           case 1:
              DoScriptText(SAY_BARTLEBY02, m_creature);
              if (m_uiPhase == 1)
                 m_uiPhase = 2;
              break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if (m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               SpeechEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }
    }
};

CreatureAI* GetAI_npc_mike(Creature* pCreature)
{
    return new npc_mikeAI(pCreature);
}

CreatureAI* GetAI_npc_bartleby_cs(Creature* pCreature)
{
    return new npc_bartleby_csAI(pCreature);
}

void AddSC_culling_of_stratholmeAI()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_mike";
    pNewScript->GetAI = &GetAI_npc_mike;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_bartleby_cs";
    pNewScript->GetAI = &GetAI_npc_bartleby_cs;
    pNewScript->RegisterSelf();

}
