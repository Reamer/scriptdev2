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
SDName: boss_rotface
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    SPELL_BERSERK               = 47008,

    // Mutated Infection
    SPELL_MUTATED_INFECTION_1   = 70090, // periodic trigger auras
    SPELL_MUTATED_INFECTION_2   = 70003,
    SPELL_MUTATED_INFECTION_3   = 70004,
    SPELL_MUTATED_INFECTION_4   = 70005,
    SPELL_MUTATED_INFECTION_5   = 70006,

    // Slime Spray
    SPELL_SLIME_SPRAY_SUMMON    = 70882,
    SPELL_SLIME_SPRAY           = 69508,

    // Ooze Flood
    SPELL_OOZE_FLOOD_PERIODIC   = 70069, // periodically trigger ooze flood
    SPELL_OOZE_FLOOD_REMOVE     = 70079,

    // Little Ooze
    SPELL_STICKY_OOZE           = 69774,
    SPELL_STICKY_AURA           = 69776,
    SPELL_WEAK_RADIATING_OOZE   = 69750,
    SPELL_LITTLE_OOZE_COMBINE   = 69537, // periodic check
    SPELL_MERGE                 = 69889,

    // Big Ooze
    SPELL_UNSTABLE_OOZE         = 69558, // stacking buff
    SPELL_RADIATING_OOZE        = 69760,
    SPELL_BIG_OOZE_COMBINE      = 69552, // periodic check
    SPELL_BIG_OOZE_BUFF_COMB    = 69611, // periodic check

    // others
    NPC_LITTLE_OOZE             = 36897,
    NPC_BIG_OOZE                = 36899,

    MAX_MUTATE_INFACTION_STEPS  = 5,

    // Vile Gas (heroic)
    SPELL_VILE_GAS              = 69240,

};

const static LOCATION PutricideRotfacePoint = {4417.302246f, 3188.219971f, 389.332520f, 5.102f};  // 2 Putricide Rotface say o=5.102

struct MutatedInfections
{
    uint32 spellid;
    uint8 amountOfCast;
    uint32 durationOfOneTick;
};
static MutatedInfections uiMutatedInfections[5] =
{
    {SPELL_MUTATED_INFECTION_1, 4, 14*IN_MILLISECONDS},
    {SPELL_MUTATED_INFECTION_2, 4, 12*IN_MILLISECONDS},
    {SPELL_MUTATED_INFECTION_3, 4, 10*IN_MILLISECONDS},
    {SPELL_MUTATED_INFECTION_4, 6,  8*IN_MILLISECONDS},
    {SPELL_MUTATED_INFECTION_5, 99, 6*IN_MILLISECONDS}
};

//talks
enum
{
    SAY_PRECIOUS_DIES           = -1631070,
    SAY_AGGRO                   = -1631071,
    SAY_SLIME_SPRAY             = -1631072,
    SAY_OOZE_EXPLODE            = -1631073,
    SAY_SLIME_FLOW_1            = -1631074,
    SAY_SLIME_FLOW_2            = -1631075,
    SAY_SLAY_1                  = -1631076,
    SAY_SLAY_2                  = -1631077,
    SAY_BERSERK                 = -1631078,
    SAY_DEATH                   = -1631079,
};

// Rotface
struct MANGOS_DLL_DECL boss_rotfaceAI : public ScriptedAI
{
    boss_rotfaceAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    uint32 m_uiSlimeSprayTimer;
    uint32 m_uiMutatedInfectionTimer;
    uint32 m_uiInfectionsRate;
    uint32 m_uiVileGasTimer;

    void Reset() override
    {
        m_uiSlimeSprayTimer = urand(17000, 23000);
        m_uiInfectionsRate = 0;
        m_uiVileGasTimer = urand(10000, 15000);
        m_uiMutatedInfectionTimer = uiMutatedInfections[m_uiInfectionsRate].durationOfOneTick * uiMutatedInfections[m_uiInfectionsRate].amountOfCast + 1000;
        ++m_uiInfectionsRate;
    }

    void Aggro(Unit *pWho) override
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, IN_PROGRESS);
            if (Creature* pPutricide = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
            {
                pPutricide->CastSpell(pPutricide, SPELL_OOZE_FLOOD_PERIODIC, true);
                pPutricide->GetMotionMaster()->MovePoint(0, PutricideRotfacePoint.x, PutricideRotfacePoint.y, PutricideRotfacePoint.z);
            }
        }

        DoScriptText(SAY_AGGRO, m_creature);

        DoCastSpellIfCan(m_creature, SPELL_MUTATED_INFECTION_1, CAST_TRIGGERED);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, FAIL);
            if (Creature* pPutricide = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pPutricide->AI()->EnterEvadeMode();
        }

        DoCastSpellIfCan(m_creature, SPELL_OOZE_FLOOD_REMOVE, CAST_TRIGGERED);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature, pVictim);
    }

    void JustDied(Unit *pKiller) override
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ROTFACE, DONE);
            if (Creature* pPutricide = m_pInstance->GetSingleCreatureFromStorage(NPC_PROFESSOR_PUTRICIDE))
                pPutricide->AI()->EnterEvadeMode();
        }

        DoCastSpellIfCan(m_creature, SPELL_OOZE_FLOOD_REMOVE, CAST_TRIGGERED);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Slime Spray
        if (m_uiSlimeSprayTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SLIME_SPRAY_SUMMON, CAST_TRIGGERED) == CAST_OK)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SLIME_SPRAY) == CAST_OK)
                {
                    DoScriptText(SAY_SLIME_SPRAY, m_creature);
                    m_uiSlimeSprayTimer = urand(17000, 23000);
                }
            }
        }
        else
            m_uiSlimeSprayTimer -= uiDiff;

        // Mutated Infection - faster with time
        // implemented this instead of phases
        if (m_uiInfectionsRate < MAX_MUTATE_INFACTION_STEPS)
        {
            if (m_uiMutatedInfectionTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, uiMutatedInfections[m_uiInfectionsRate].spellid, CAST_TRIGGERED) == CAST_OK)
                {
                    m_creature->RemoveAurasDueToSpell(uiMutatedInfections[m_uiInfectionsRate - 1].spellid);
                    m_uiMutatedInfectionTimer = uiMutatedInfections[m_uiInfectionsRate].durationOfOneTick * uiMutatedInfections[m_uiInfectionsRate].amountOfCast + 1000;
                    ++m_uiInfectionsRate;
                }
            }
            else
                m_uiMutatedInfectionTimer -= uiDiff;
        }

        if (m_pInstance && m_pInstance->IsHeroicDifficulty())
        {
            if (m_uiVileGasTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_VILE_GAS) == CAST_OK)
                    m_uiVileGasTimer = urand(10000, 15000);
            }
            else
                m_uiVileGasTimer -= uiDiff;            
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rotface(Creature* pCreature)
{
    return new boss_rotfaceAI(pCreature);
}

struct MANGOS_DLL_DECL mob_little_oozeAI : public ScriptedAI
{
    mob_little_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiStickyOozeTimer;

    void Reset()
    {
        m_uiStickyOozeTimer = 5000;
    }

    void EnterEvadeMode()
    {
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *pWho)
    {
        m_creature->AddThreat(pWho, 500000.0f);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
        DoCastSpellIfCan(m_creature, SPELL_WEAK_RADIATING_OOZE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_LITTLE_OOZE_COMBINE, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStickyOozeTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STICKY_OOZE) == CAST_OK)
                m_uiStickyOozeTimer = urand(10000, 15000);
        }
        else
            m_uiStickyOozeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_little_ooze(Creature* pCreature)
{
    return new mob_little_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_big_oozeAI : public ScriptedAI
{
    mob_big_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiStickyOozeTimer;
    uint32 m_uiCheckTimer;
    bool m_bHasSaid;

    void Reset()
    {
        m_uiStickyOozeTimer = 5000;
        m_uiCheckTimer = 1000;
        m_bHasSaid = false;
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *pWho)
    {
        m_creature->AddThreat(pWho, 500000.0f);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
        DoCastSpellIfCan(m_creature, SPELL_RADIATING_OOZE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_BIG_OOZE_COMBINE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_BIG_OOZE_BUFF_COMB, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Unstable Ooze
        if (!m_bHasSaid)
        {
            if (m_uiCheckTimer <= uiDiff)
            {
                if (SpellAuraHolderPtr holder= m_creature->GetSpellAuraHolder(SPELL_UNSTABLE_OOZE))
                {
                    if (holder->GetStackAmount() >= 5)
                    {
                        if (m_pInstance)
                        {
                            if (Creature *pRotface = m_pInstance->GetSingleCreatureFromStorage(NPC_ROTFACE))
                            {
                                DoScriptText(SAY_OOZE_EXPLODE, pRotface);
                                m_bHasSaid = true;
                            }
                        }
                    }
                }
                m_uiCheckTimer = 1000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        // Sticky Ooze
        if (m_uiStickyOozeTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STICKY_OOZE) == CAST_OK)
                m_uiStickyOozeTimer = urand(10000, 15000);
        }
        else
            m_uiStickyOozeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_big_ooze(Creature* pCreature)
{
    return new mob_big_oozeAI(pCreature);
}

// Ooze explosion stalker
struct MANGOS_DLL_DECL mob_ooze_explosion_stalkerAI : public ScriptedAI
{
    mob_ooze_explosion_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pCreature->ForcedDespawn(10000);
    }
    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_ooze_explosion_stalker(Creature* pCreature)
{
    return new mob_ooze_explosion_stalkerAI(pCreature);
}

struct MANGOS_DLL_DECL mob_sticky_oozeAI : public ScriptedAI
{
    mob_sticky_oozeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        pCreature->CastSpell(pCreature, SPELL_STICKY_AURA, true);
    }

    ScriptedInstance *m_pInstance;

    void Reset(){}
    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        uiDamage = 0;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }
    }
};

CreatureAI* GetAI_mob_sticky_ooze(Creature* pCreature)
{
    return new mob_sticky_oozeAI(pCreature);
}

void AddSC_boss_rotface()
{
    Script* pNewscript;
    pNewscript = new Script;
    pNewscript->Name = "boss_rotface";
    pNewscript->GetAI = &GetAI_boss_rotface;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_little_ooze";
    pNewscript->GetAI = &GetAI_mob_little_ooze;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_big_ooze";
    pNewscript->GetAI = &GetAI_mob_big_ooze;
    pNewscript->RegisterSelf();
}
