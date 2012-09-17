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
SDName: pit_of_saron
SD%Complete: 0
SDComment:
SDCategory: Pit of Saron
EndScriptData */

/* ContentData
EndContentData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SPELL_FALLING_ICICLE            = 69426,
    SPELL_FALLING_ICICLE_DUMMY      = 69428,
};

struct MANGOS_DLL_DECL npc_icicle_pit_of_saronAI: public ScriptedAI
{
    npc_icicle_pit_of_saronAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;
    uint32 m_uiActionTimer;

    void Reset()
    {
        m_creature->SetDisplayId(28470);
        SetCombatMovement(false);
        m_uiActionTimer = 3000;
    }

    void AttackStart(Unit* pWho) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiActionTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FALLING_ICICLE, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_FALLING_ICICLE_DUMMY);
            m_uiActionTimer = 30000;
        }
        else
            m_uiActionTimer -= uiDiff;
    }

};

CreatureAI* GetAI_npc_icicle_pit_of_saron(Creature* pCreature)
{
    return new npc_icicle_pit_of_saronAI (pCreature);
}

bool AreaTrigger_at_pit_of_Saron_gaunlet(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (instance_pit_of_saron* pInstance = (instance_pit_of_saron*)pPlayer->GetInstanceData())
    {
        switch (pAt->id)
        {
            case 5581:                                                  // gaunlet end
                if (pInstance->GetData(TYPE_GAUNTLET) != DONE)
                    pInstance->SetData(TYPE_GAUNTLET, DONE);            // stop Ice Falling
                break;
            case 5580:                                                  // gaunlet begin
                if (pInstance->GetData(TYPE_GAUNTLET) == NOT_STARTED)
                {
                    pInstance->SetData(TYPE_GAUNTLET, IN_PROGRESS);     // start Ice Falling
                }
                break;
            case 5599:
            case 5598:
                // TODO: make JumpEvent
                break;
            default:
                break;
        }
    }

    return false;
}

void AddSC_pit_of_saron()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "at_pit_of_saron";
    pNewScript->pAreaTrigger = &AreaTrigger_at_pit_of_Saron_gaunlet;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_icicle_pit_of_saron";
    pNewScript->GetAI = &GetAI_npc_icicle_pit_of_saron;
    pNewScript->RegisterSelf();
}
