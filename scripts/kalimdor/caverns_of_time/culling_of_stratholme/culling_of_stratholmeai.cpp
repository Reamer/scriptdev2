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

CreatureAI* GetAI_npc_bartleby_cs(Creature* pCreature)
{
    return new npc_bartleby_csAI(pCreature);
}

void AddSC_culling_of_stratholmeAI()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_bartleby_cs";
    pNewScript->GetAI = &GetAI_npc_bartleby_cs;
    pNewScript->RegisterSelf();

}
