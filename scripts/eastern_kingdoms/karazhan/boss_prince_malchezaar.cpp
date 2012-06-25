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
SDName: Boss_Prince_Malchezzar
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    SAY_AGGRO               = -1532091,
    SAY_AXE_TOSS1           = -1532092,
    SAY_AXE_TOSS2           = -1532093,
    SAY_SPECIAL1            = -1532094,
    SAY_SPECIAL2            = -1532095,
    SAY_SPECIAL3            = -1532096,
    SAY_SLAY1               = -1532097,
    SAY_SLAY2               = -1532098,
    SAY_SLAY3               = -1532099,
    SAY_SUMMON1             = -1532100,
    SAY_SUMMON2             = -1532101,
    SAY_DEATH               = -1532102,
};

// 18 Coordinates for Infernal spawns
struct InfernalPoint
{
    float x,y;
};

#define INFERNAL_Z  275.5

const static InfernalPoint InfernalPoints[] =
{
    {-10922.8f, -1985.2f},
    {-10916.2f, -1996.2f},
    {-10932.2f, -2008.1f},
    {-10948.8f, -2022.1f},
    {-10958.7f, -1997.7f},
    {-10971.5f, -1997.5f},
    {-10990.8f, -1995.1f},
    {-10989.8f, -1976.5f},
    {-10971.6f, -1973.0f},
    {-10955.5f, -1974.0f},
    {-10939.6f, -1969.8f},
    {-10958.0f, -1952.2f},
    {-10941.7f, -1954.8f},
    {-10943.1f, -1988.5f},
    {-10948.8f, -2005.1f},
    {-10984.0f, -2019.3f},
    {-10932.8f, -1979.6f},
    {-10935.7f, -1996.0f}
};

struct InfernalPointWithInfernal
{
    InfernalPoint point;
    bool hasInfernal;
};

struct EnfeebleVictim
{
    ObjectGuid guid;
    uint32 health;
};

enum
{
    TOTAL_INFERNAL_POINTS       = 18,

//Enfeeble is supposed to reduce hp to 1 and then heal player back to full when it ends
//Along with reducing healing and regen while enfeebled to 0%
//This spell effect will only reduce healing

    SPELL_ENFEEBLE              = 30843,                    // Enfeeble during m_uiPhase 1 and 2
    SPELL_ENFEEBLE_EFFECT       = 41624,

    SPELL_SHADOWNOVA            = 30852,                    // Shadownova used during all phases
    SPELL_SW_PAIN               = 30854,                    // Shadow word pain during m_uiPhase 1 and 3 (different targeting rules though)
    SPELL_THRASH_PASSIVE        = 12787,                    // Extra attack chance during m_uiPhase 2
    SPELL_SUNDER_ARMOR          = 30901,                    // Sunder armor during m_uiPhase 2
    SPELL_THRASH_AURA           = 12787,                    // Passive proc chance for thrash
    SPELL_SUMMON_AXES           = 30891,                    // Summon One NPC - visual: two Axes
    SPELL_EQUIP_AXES            = 30857,                    // Visual for axe equiping
    SPELL_AMPLIFY_DAMAGE        = 39095,                    // Amplifiy during m_uiPhase 3
    SPELL_CLEAVE                = 30131,                    // Same as Nightbane
    SPELL_SUMMON_INFERNAL       = 30836,                    // Summon Infernal
    SPELL_HELLFIRE              = 30859,                    // Infernals' hellfire aura
    NPC_NETHERSPITE_INFERNAL    = 17646,                    // The netherspite infernal creature
    NPC_MALCHEZARS_AXE          = 17650,                    // Malchezar's m_aAxeGuid (creatures), summoned during m_uiPhase 3

    INFERNAL_MODEL_INVISIBLE    = 11686,                    // Infernal Effects
    SPELL_INFERNAL_RELAY        = 30834,

    EQUIP_ID_AXE                = 33542,                    // Axes info
};

struct MANGOS_DLL_DECL boss_malchezaarAI : public ScriptedAI
{
    boss_malchezaarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_karazhan*) pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiEnfeebleTimer;
    uint32 m_uiEnfeebleResetTimer;
    uint32 m_uiShadowNovaTimer;
    uint32 m_uiSWPainTimer;
    uint32 m_uiSunderArmorTimer;
    uint32 m_uiAmplifyDamageTimer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiInfernalTimer;
    uint32 m_uiAxesTargetSwitchTimer;
    uint32 m_uiInfernalCleanupTimer;


    ObjectGuid m_Axe;
    std::list<InfernalPointWithInfernal> m_InfernalPositions;
    std::list<EnfeebleVictim> m_EnfeebleVictimList;


    uint8 m_uiPhase;

    void Reset()
    {
        AxesCleanup();
        ClearWeapons();
        m_EnfeebleVictimList.clear();

        m_InfernalPositions.clear();
        for(int i = 0; i < TOTAL_INFERNAL_POINTS; ++i)
        {
            InfernalPointWithInfernal infernal;
            infernal.point.x = InfernalPoints[i].x;
            infernal.point.y = InfernalPoints[i].y;
            infernal.hasInfernal = false;
            m_InfernalPositions.push_back(infernal);
        }

        m_uiEnfeebleTimer = 30000;
        m_uiEnfeebleResetTimer = 38000;
        m_uiShadowNovaTimer = 35500;
        m_uiSWPainTimer = 20000;
        m_uiAmplifyDamageTimer = 5000;
        m_uiCleave_Timer = 8000;
        m_uiInfernalTimer = 45000;
        m_uiInfernalCleanupTimer = 47000;
        m_uiAxesTargetSwitchTimer = urand(7500, 20000);
        m_uiSunderArmorTimer = urand(5000, 10000);
        m_uiPhase = 1;
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALCHEZZAR, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALCHEZZAR, FAIL);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_NETHERSPITE_INFERNAL:
                DoCastSpellIfCan(pSummoned, SPELL_INFERNAL_RELAY, CAST_TRIGGERED);
                break;
            case NPC_MALCHEZARS_AXE:
                m_Axe = pSummoned->GetObjectGuid();
                if (Unit* pVictim = m_creature->getVictim())
                {
                    pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pSummoned->AI()->AttackStart(pVictim);
                }
                break;
            default:
                break;
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        switch (pSpell->Id)
        {
            case SPELL_INFERNAL_RELAY:
            {
                if (pTarget->GetEntry() == NPC_NETHERSPITE_INFERNAL)
                {
                    pTarget->CastSpell(pTarget, SPELL_HELLFIRE, false);
                    pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
                break;
            }
            case SPELL_ENFEEBLE:
            {
                EnfeebleVictim Victim;
                Victim.guid = pTarget->GetObjectGuid();
                Victim.health = pTarget->GetHealth();
                m_EnfeebleVictimList.push_back(Victim);
                pTarget->SetHealth(1);
                break;
            }
            default:
                break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MALCHEZZAR, DONE);
        DoScriptText(SAY_DEATH, m_creature);

        AxesCleanup();
        ClearWeapons();
        EnfeebleResetHealth();
        m_InfernalPositions.clear();
    }

    void AxesCleanup()
    {
        if (Creature* pAxe = m_creature->GetMap()->GetCreature(m_Axe))
            pAxe->ForcedDespawn();
    }

    void ClearWeapons()
    {
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        //damage
        const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, cinfo->mindmg);
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, cinfo->maxdmg);
        m_creature->UpdateDamagePhysical(BASE_ATTACK);
    }

    void EnfeebleResetHealth()
    {
        for(std::list<EnfeebleVictim>::const_iterator itr = m_EnfeebleVictimList.begin(); itr != m_EnfeebleVictimList.end(); ++itr)
        {
            if (Unit* pTarget = m_creature->GetMap()->GetUnit(itr->guid))
            {
                if (pTarget->isAlive())
                    pTarget->SetHealth(itr->health);
            }
        }
        m_EnfeebleVictimList.clear();
    }

    bool SummonInfernal()
    {
        if (m_pInstance)
        {
            std::list<InfernalPointWithInfernal>::iterator itr = m_InfernalPositions.begin();
            advance(itr, urand(0, m_InfernalPositions.size() -1));
            if (!itr->hasInfernal)
            {
                if (Creature* pInfernalTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetRandomInfernalTarget()))
                {
                    m_creature->GetMap()->CreatureRelocation(pInfernalTarget, itr->point.x, itr->point.y, INFERNAL_Z, 0.0f);
                    pInfernalTarget->Relocate(itr->point.x, itr->point.y, INFERNAL_Z);
                    if (DoCastSpellIfCan(pInfernalTarget, SPELL_SUMMON_INFERNAL) == CAST_OK)
                    {
                        DoScriptText(urand(0, 1) ? SAY_SUMMON1 : SAY_SUMMON2, m_creature);
                        itr->hasInfernal = true;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnfeebleResetTimer)
        {
            if (m_uiEnfeebleResetTimer <= uiDiff)                  //Let's not forget to reset that
            {
                EnfeebleResetHealth();
                m_uiEnfeebleResetTimer=0;
            }
            else
                m_uiEnfeebleResetTimer -= uiDiff;
        }

        if (m_creature->hasUnitState(UNIT_STAT_STUNNED))    //While shifting to m_uiPhase 2 m_malchezaarGuid stuns himself
            return;

        switch (m_uiPhase)
        {
            case 1:
                if (m_creature->GetHealthPercent() < 60.0f)
                {
                    //animation
                    if (DoCastSpellIfCan(m_creature, SPELL_EQUIP_AXES) == CAST_OK)
                    {
                        m_uiPhase = 2;
                        //text
                        DoScriptText(SAY_AXE_TOSS1, m_creature);

                        //passive thrash aura
                        m_creature->CastSpell(m_creature, SPELL_THRASH_AURA, true);

                        //models
                        SetEquipmentSlots(false, EQUIP_ID_AXE, EQUIP_ID_AXE, EQUIP_NO_CHANGE);

                        //damage
                        const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, 2*cinfo->mindmg);
                        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, 2*cinfo->maxdmg);
                        m_creature->UpdateDamagePhysical(BASE_ATTACK);

                        m_creature->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, cinfo->mindmg);
                        m_creature->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, cinfo->maxdmg);
                        //Sigh, updating only works on main attack , do it manually ....
                        m_creature->SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, cinfo->mindmg);
                        m_creature->SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, cinfo->maxdmg);

                        m_creature->SetAttackTime(OFF_ATTACK, (m_creature->GetAttackTime(BASE_ATTACK)*150)/100);
                    }
                }
                break;
            case 2:
                if (m_creature->GetHealthPercent() < 30.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_AXES) == CAST_OK)
                    {
                        m_uiInfernalTimer = 15000;

                        m_uiPhase = 3;

                        ClearWeapons();

                        //remove thrash
                        m_creature->RemoveAurasDueToSpell(SPELL_THRASH_AURA);

                        DoScriptText(SAY_AXE_TOSS2, m_creature);

                        if (m_uiShadowNovaTimer > 35000)
                            m_uiShadowNovaTimer = m_uiEnfeebleTimer + 5000;

                        return;
                    }
                }

                if (m_uiSunderArmorTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUNDER_ARMOR) == CAST_OK)
                        m_uiSunderArmorTimer = urand(10000, 18000);
                }
                else
                    m_uiSunderArmorTimer -= uiDiff;

                if (m_uiCleave_Timer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                        m_uiCleave_Timer = urand(6000, 12000);
                }
                else
                    m_uiCleave_Timer -= uiDiff;

                break;
            case 3:
                if (m_uiAxesTargetSwitchTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (Creature *pAxe = m_creature->GetMap()->GetCreature(m_Axe))
                        {
                            pAxe->AI()->AttackStart(pTarget);
                            m_uiAxesTargetSwitchTimer = urand(7500, 20000);
                        }
                    }
                }
                else
                    m_uiAxesTargetSwitchTimer -= uiDiff;

                if (m_uiAmplifyDamageTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_AMPLIFY_DAMAGE) == CAST_OK)
                            m_uiAmplifyDamageTimer = urand(20000, 30000);
                    }
                }
                else
                    m_uiAmplifyDamageTimer -= uiDiff;

                break;
            default:
                break;
        }



        //Time for global and double timers
        if (m_uiInfernalTimer < uiDiff)
        {
            if (SummonInfernal())
                m_uiInfernalTimer =  m_uiPhase == 3 ? 10000 : 45000;    //10 secs in m_uiPhase 3, 45 otherwise
        }
        else
            m_uiInfernalTimer -= uiDiff;

        if (m_uiShadowNovaTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWNOVA) == CAST_OK)
                m_uiShadowNovaTimer = m_uiPhase == 3 ? 31000 : -1;
        }
        else
            m_uiShadowNovaTimer -= uiDiff;

        if (m_uiPhase != 2)
        {
            if (m_uiSWPainTimer < uiDiff)
            {
                if (Unit* pTarget = m_uiPhase == 1 ? m_creature->getVictim() : m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SW_PAIN) == CAST_OK)
                        m_uiSWPainTimer = 20000;
                }
            }
            else
                m_uiSWPainTimer -= uiDiff;
        }

        if (m_uiPhase != 3)
        {
            if (m_uiEnfeebleTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENFEEBLE) == CAST_OK)
                {
                    m_uiEnfeebleTimer = 30000;
                    m_uiShadowNovaTimer = 5000;
                    m_uiEnfeebleResetTimer = 9000;
                }
            }
            else
                m_uiEnfeebleTimer -= uiDiff;
        }

        if (m_uiPhase==2)
            DoMeleeAttacksIfReady();
        else
            DoMeleeAttackIfReady();
    }

    void DoMeleeAttacksIfReady()
    {
        // Check if pTarget is valid
        if (!m_creature->getVictim())
            return;

        if (!m_creature->IsNonMeleeSpellCasted(false) && m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            //Check for base attack
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
            //Check for offhand attack
            if (m_creature->isAttackReady(OFF_ATTACK))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim(), OFF_ATTACK);
                m_creature->resetAttackTimer(OFF_ATTACK);
            }
        }
    }
};

CreatureAI* GetAI_boss_malchezaar(Creature* pCreature)
{
    return new boss_malchezaarAI(pCreature);
}

void AddSC_boss_prince_malchezaar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_malchezaar";
    pNewScript->GetAI = &GetAI_boss_malchezaar;
    pNewScript->RegisterSelf();
}
