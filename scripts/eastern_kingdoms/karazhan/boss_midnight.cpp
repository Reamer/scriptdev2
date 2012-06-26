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
SDName: Boss_Midnight
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    SAY_MIDNIGHT_KILL           = -1532000,
    SAY_APPEAR1                 = -1532001,
    SAY_APPEAR2                 = -1532002,
    SAY_APPEAR3                 = -1532003,
    SAY_MOUNT                   = -1532004,
    SAY_KILL1                   = -1532005,
    SAY_KILL2                   = -1532006,
    SAY_DISARMED                = -1532007,
    SAY_DEATH                   = -1532008,
    SAY_RANDOM1                 = -1532009,
    SAY_RANDOM2                 = -1532010,

    // Spells
    // Midnight
    SPELL_KNOCKDOWN             = 29711,
    SPELL_SUMMON_ATTUMEN        = 29714,
    // Attumen
    SPELL_MOUNT                 = 29770, // not used
    SPELL_SHADOWCLEAVE          = 29832,
    SPELL_INTANGIBLE_PRESENCE   = 29833,
    SPELL_SUMMON_ATTUMEN_MOUNTED= 29799,
    // additions when mounted
    SPELL_BERSERKER_CHARGE      = 26561
};

struct MANGOS_DLL_DECL boss_midnightAI : public ScriptedAI
{
    boss_midnightAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;
    uint8  m_uiPhase;

    uint32 m_uiKnockDown;


    void Reset()
    {
        m_uiPhase = 1;
        m_uiKnockDown = urand(6000,9000);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (m_uiPhase == 2)
        {
            if (Creature* pAttumen = m_pInstance->GetSingleCreatureFromStorage(NPC_ATTUMEN))
                DoScriptText(SAY_MIDNIGHT_KILL, pAttumen);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ATTUMEN, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ATTUMEN)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_APPEAR1, pSummoned); break;
                case 1: DoScriptText(SAY_APPEAR2, pSummoned); break;
                case 2: DoScriptText(SAY_APPEAR3, pSummoned); break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_uiPhase)
        {
            case 1:
                if (m_creature->GetHealthPercent() < 95.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_ATTUMEN) == CAST_OK)
                    {
                        m_uiPhase = 2;
                    }
                }
                break;
            case 2:
                if (m_pInstance->GetData(TYPE_ATTUMEN) == SPECIAL)
                {
                    m_creature->ForcedDespawn();
                }
                else if (m_creature->GetHealthPercent() < 25.0f)
                {
                    m_pInstance->SetData(TYPE_ATTUMEN, SPECIAL);
                }
                break;
            default:
                break;
        }
        if (m_uiKnockDown < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_KNOCKDOWN) == CAST_OK)
                m_uiKnockDown = urand(6000,9000);
        }
        else
            m_uiKnockDown -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_midnight(Creature* pCreature)
{
    return new boss_midnightAI(pCreature);
}

struct MANGOS_DLL_DECL boss_attumenAI : public ScriptedAI
{
    boss_attumenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiCleaveTimer;
    uint32 m_uiCurseTimer;
    uint32 m_uiRandomYellTimer;
    uint32 m_uiChargeTimer;                                 //only when mounted

    bool m_bHasSummonRider;


    void Reset()
    {
        m_uiCleaveTimer = urand(10000, 16000);
        m_uiCurseTimer = 30000;
        m_uiRandomYellTimer = urand(30000, 60000);          //Occasionally yell
        m_uiChargeTimer = 20000;

        m_bHasSummonRider = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void SpellHit(Unit* pSource, const SpellEntry* pSpell)
    {
        if (pSpell->Mechanic == MECHANIC_DISARM)
            DoScriptText(SAY_DISARMED, m_creature);
    }

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ATTUMEN, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE))
            return;

        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWCLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(10000, 16000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        if (m_uiCurseTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_INTANGIBLE_PRESENCE) == CAST_OK)
                m_uiCurseTimer = 30000;
        }
        else
            m_uiCurseTimer -= uiDiff;

        if (m_uiRandomYellTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_RANDOM1 : SAY_RANDOM2, m_creature);
            m_uiRandomYellTimer = urand(30000, 60000);
        }
        else
            m_uiRandomYellTimer -= uiDiff;

        if (m_creature->GetEntry() == NPC_ATTUMEN_MOUNTED)
        {
            if (m_uiChargeTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_BERSERKER_CHARGE, SELECT_FLAG_NOT_IN_MELEE_RANGE | SELECT_FLAG_IN_LOS))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_BERSERKER_CHARGE) == CAST_OK)
                        m_uiChargeTimer = 20000;
                }
                else
                    m_uiChargeTimer = 2000;
            }
            else
                m_uiChargeTimer -= uiDiff;
        }
        else
        {
            if (m_pInstance->GetData(TYPE_ATTUMEN) == SPECIAL && !m_bHasSummonRider)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_ATTUMEN_MOUNTED) == CAST_OK)
                {
                    m_creature->ForcedDespawn(1000);
                    m_bHasSummonRider = true;
                }
            }
            else if (m_creature->GetHealthPercent() < 25.0f)
            {
                m_pInstance->SetData(TYPE_ATTUMEN, SPECIAL);
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_attumen(Creature* pCreature)
{
    return new boss_attumenAI(pCreature);
}

void AddSC_boss_attumen()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_attumen";
    pNewScript->GetAI = &GetAI_boss_attumen;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_midnight";
    pNewScript->GetAI = &GetAI_boss_midnight;
    pNewScript->RegisterSelf();
}
