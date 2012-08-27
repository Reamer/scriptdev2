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
SDName: boss_forgemaster_garfrost
SD%Complete: 70
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_AGGRO                           = -1658014,
    SAY_SLAY_1                          = -1658015,
    SAY_SLAY_2                          = -1658019,
    SAY_BOULDER_HIT                     = -1658022,
    SAY_DEATH                           = -1658017,

    SAY_FORGE_1                         = -1658018,
    SAY_FORGE_2                         = -1658019,

    EMOTE_THROW_SARONITE                = -1658022,
    EMOTE_DEEP_FREEZE                   = -1658023,

    SPELL_PERMAFROST                    = 70326,
    SPELL_THROW_SARONITE                = 68788,
    SPELL_THUNDERING_STOMP              = 68771,
    SPELL_FORGE_FROZEN_BLADE            = 68774,
    SPELL_CHILLING_WAVE                 = 68778,
    SPELL_FORGE_FROSTBORN_MACE          = 68785,
    SPELL_DEEP_FREEZE                   = 70381,
    SPELL_FORGE_MACE                    = 68785,
    SPELL_FORGE_BLADE                   = 68774,

    PHASE_NO_ENCHANTMENT                = 1,
    PHASE_BLADE_ENCHANTMENT             = 2,
    PHASE_MACE_ENCHANTMENT              = 3,

    ACHIEV_DOESNT_GO_TO_ELEVEN          = 4524,
};
//Positional defines 
struct LocationsXYZ
{
    float x, y, z;
};

static LocationsXYZ aGarfrostMoveLocs[] =
{
    {719.785f, -230.227f, 527.033f},
    {657.539f, -203.564f, 526.691f},
};

struct MANGOS_DLL_DECL boss_forgemaster_garfrostAI : public ScriptedAI
{
    boss_forgemaster_garfrostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;

    uint32 m_uiThrowSaroniteTimer;
    uint32 m_uiPhase;
    uint32 m_uiChillingWaveTimer;
    uint32 m_uiDeepFreezeTimer;

    void Reset()
    {
        m_uiThrowSaroniteTimer = 13000;
        m_uiChillingWaveTimer = 10000;
        m_uiDeepFreezeTimer = 10000;
        SetCombatMovement(true);
        m_uiPhase = PHASE_NO_ENCHANTMENT;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GARFROST, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature, pWho);
        DoCastSpellIfCan(m_creature, SPELL_PERMAFROST);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature, pKiller);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GARFROST, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0,1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != EFFECT_MOTION_TYPE)
            return;

        // Cast and say expected spell
        DoCastSpellIfCan(m_creature, m_uiPhase == PHASE_BLADE_ENCHANTMENT ? SPELL_FORGE_FROZEN_BLADE : SPELL_FORGE_FROSTBORN_MACE);
        DoScriptText(m_uiPhase == PHASE_BLADE_ENCHANTMENT ? SAY_FORGE_1 : SAY_FORGE_2, m_creature);

        m_uiThrowSaroniteTimer += 5000;                     // Delay next Saronit
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Casted in every phase
        if (m_uiThrowSaroniteTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_THROW_SARONITE, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_THROW_SARONITE) == CAST_OK)
                {
                    DoScriptText(SAY_BOULDER_HIT, m_creature);
                    DoScriptText(EMOTE_THROW_SARONITE, m_creature, pTarget);
                    m_uiThrowSaroniteTimer = 16000;
                }
            }
        }
        else
            m_uiThrowSaroniteTimer -= uiDiff;

        switch (m_uiPhase)
        {
            case PHASE_NO_ENCHANTMENT:
            {
                if (m_creature->GetHealthPercent() < 66.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP) == CAST_OK)
                    {
                        m_creature->GetMotionMaster()->MoveJump(aGarfrostMoveLocs[0].x, aGarfrostMoveLocs[0].y, aGarfrostMoveLocs[0].z, 25.0f, 15.0f);
                        m_uiPhase = PHASE_BLADE_ENCHANTMENT;

                        // Stop further action
                        return;
                    }
                }
                break;
            }
            case PHASE_BLADE_ENCHANTMENT:
            {
                if (m_creature->GetHealthPercent() < 33.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP) == CAST_OK)
                    {
                        m_creature->GetMotionMaster()->MoveJump(aGarfrostMoveLocs[1].x, aGarfrostMoveLocs[1].y, aGarfrostMoveLocs[1].z, 25.0f, 15.0f);
                        m_uiPhase = PHASE_MACE_ENCHANTMENT;

                        // Stop further action
                        return;
                    }
                }

                if (m_uiChillingWaveTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHILLING_WAVE) == CAST_OK)
                        m_uiChillingWaveTimer = 14000;
                }
                else
                    m_uiChillingWaveTimer -= uiDiff;

                break;
            }
            case PHASE_MACE_ENCHANTMENT:
            {
                if (m_uiDeepFreezeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_DEEP_FREEZE) == CAST_OK)
                        {
                            DoScriptText(EMOTE_DEEP_FREEZE, m_creature, pTarget);
                            m_uiDeepFreezeTimer = 20000;
                        }
                    }
                }
                else
                    m_uiDeepFreezeTimer -= uiDiff;

                break;
            }
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_forgemaster_garfrost(Creature* pCreature)
{
    return new boss_forgemaster_garfrostAI(pCreature);
}

void AddSC_boss_garfrost()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_forgemaster_garfrost";
    pNewScript->GetAI = &GetAI_boss_forgemaster_garfrost;
    pNewScript->RegisterSelf();
}
