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
SDName: Boss_Curator
SD%Complete: 80%
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    SAY_AGGRO                   = -1532057,
    SAY_SUMMON1                 = -1532058,
    SAY_SUMMON2                 = -1532059,
    SAY_EVOCATE                 = -1532060,
    SAY_ENRAGE                  = -1532061,
    SAY_KILL1                   = -1532062,
    SAY_KILL2                   = -1532063,
    SAY_DEATH                   = -1532064,

    // Flare
    NPC_ASTRAL_FLARE            = 17096,
    SPELL_ASTRAL_FLARE_PASSIVE  = 30234,

    // The Curator
    SPELL_HATEFUL_BOLT          = 30383,
    SPELL_EVOCATION             = 30254,
    SPELL_ENRAGE                = 30403,
    SPELL_BERSERK               = 26662
};

struct MANGOS_DLL_DECL boss_curatorAI : public ScriptedAI
{
    boss_curatorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_karazhan*) pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiFlareTimer;
    uint32 m_uiHatefulBoltTimer;
    uint32 m_uiBerserkTimer;

    bool m_bIsEnraged;

    void Reset()
    {
        m_uiFlareTimer = 10000;
        m_uiHatefulBoltTimer = 15000;                       // This time may be wrong
        m_uiBerserkTimer = 12*MINUTE*IN_MILLISECONDS;
        m_bIsEnraged = false;

        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CURATOR, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CURATOR, FAIL);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CURATOR, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ASTRAL_FLARE)
        {
            // Flare start with aggro on it's target, should be immune to arcane
            pSummoned->CastSpell(pSummoned, SPELL_ASTRAL_FLARE_PASSIVE, false);
            pSummoned->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ARCANE, true);
            pSummoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // always decrease BerserkTimer
        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                // break evocation if we are under it's effect
                if (m_creature->HasAura(SPELL_EVOCATION))
                    m_creature->RemoveAurasDueToSpell(SPELL_EVOCATION);

                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                {
                    // ScriptText needs confirmation
                    DoScriptText(SAY_ENRAGE, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        // not supposed to do anything while evocate
        if (m_creature->HasAura(SPELL_EVOCATION))
            return;

        if (!m_creature->HasAura(SPELL_ENRAGE))
        {
            if (m_uiFlareTimer < uiDiff)
            {
                m_uiFlareTimer = 10000;

                // summon Astral Flare - no Summon Spell found
                DoSpawnCreature(NPC_ASTRAL_FLARE, rand()%37, rand()%37, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);

                // reduce mana by 10% of maximum
                if (int32 iMana = m_creature->GetMaxPower(POWER_MANA))
                {
                    m_creature->ModifyPower(POWER_MANA, -(iMana/10));

                    //if this get's us below 10%, then we evocate (the 10th should be summoned now
                    if (m_creature->GetPower(POWER_MANA)*10 < m_creature->GetMaxPower(POWER_MANA))
                    {
                        DoScriptText(SAY_EVOCATE, m_creature);
                        DoCastSpellIfCan(m_creature, SPELL_EVOCATION, CAST_INTERRUPT_PREVIOUS | CAST_FORCE_TARGET_SELF);

                        //this small delay should make first flare appear fast after evocate, and also prevent possible spawn flood
                        m_uiFlareTimer = 1000;
                        return;
                    }
                    else
                    {
                        switch(urand(0, 3))
                        {
                            case 0: DoScriptText(SAY_SUMMON1, m_creature); break;
                            case 1: DoScriptText(SAY_SUMMON2, m_creature); break;
                        }
                    }
                }
            }
            else
                m_uiFlareTimer -= uiDiff;
        }

        if (m_creature->GetHealthPercent() < 15.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE, CAST_AURA_NOT_PRESENT) == CAST_OK)
            {
                DoScriptText(SAY_ENRAGE, m_creature);
            }
        }

        if (m_uiHatefulBoltTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_HATEFUL_BOLT) == CAST_OK)
                {
                    m_uiHatefulBoltTimer = m_bIsEnraged ? 7000 : 15000;
                }
            }
        }
        else
            m_uiHatefulBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_curator(Creature* pCreature)
{
    return new boss_curatorAI(pCreature);
}

void AddSC_boss_curator()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_curator";
    pNewScript->GetAI = &GetAI_boss_curator;
    pNewScript->RegisterSelf();
}
