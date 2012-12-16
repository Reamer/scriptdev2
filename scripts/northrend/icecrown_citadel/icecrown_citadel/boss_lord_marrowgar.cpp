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
SDName: boss_lord_marrowgar
SD%Complete: 100%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_AGGRO                   = -1631002,
    SAY_BONE_STORM              = -1631003,
    SAY_BONE_SPIKE_1            = -1631004,
    SAY_BONE_SPIKE_2            = -1631005,
    SAY_BONE_SPIKE_3            = -1631006,
    SAY_SLAY_1                  = -1631007,
    SAY_SLAY_2                  = -1631008,
    SAY_DEATH                   = -1631009,
    SAY_BERSERK                 = -1631010,

    // spells
    SPELL_BERSERK               = 47008,
    SPELL_BONE_SLICE            = 69055,
    SPELL_BONE_STORM            = 69076,
    SPELL_COLDFLAME             = 69140,
    SPELL_COLDFLAME_STORM       = 72705,
    SPELL_BONE_SPIKE            = 69057,
    SPELL_BONE_SPIKE_STORM      = 73142,

    // summoned spells
    SPELL_COLDFLAME_AURA        = 69145,
    SPELL_IMPALED               = 69065,

    // npcs
    NPC_BONE_SPIKE              = 38711,
    NPC_COLDFLAME               = 36672,

    // max cold flame charges
    MAX_CHARGES_NORMAL          = 4,
    MAX_CHARGES_HEROIC          = 5,
};

enum MarrowgarPhase
{
    PHASE_NORMAL                = 1,
    PHASE_BONE_STORM_CHARGE     = 2,
    PHASE_BONE_STORM_CHARGING   = 3,
    PHASE_BONE_STORM_COLDFLAME  = 4,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        // on heroic, there is 1 more Bone Storm charge
        m_uiMaxCharges = m_pInstance && m_pInstance->IsHeroicDifficulty() ? MAX_CHARGES_HEROIC : MAX_CHARGES_NORMAL;
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    MarrowgarPhase m_Phase;
    uint8 m_uiChargesCount;
    uint8 m_uiMaxCharges;
    uint32 m_uiBerserkTimer;
    uint32 m_uiBoneSliceTimer;
    uint32 m_uiColdflameTimer;
    uint32 m_uiBoneSpikeTimer;
    uint32 m_uiBoneStormTimer;
    uint32 m_uiBoneStormChargeTimer;
    uint32 m_uiBoneStormColdflameTimer;

    void Reset()
    {
        SetCombatMovement(true);

        m_Phase                     = PHASE_NORMAL;
        m_uiChargesCount            = 0;
        m_uiBerserkTimer            = 10*MINUTE*IN_MILLISECONDS;
        m_uiBoneSliceTimer          = 1000;
        m_uiColdflameTimer          = 5000;
        m_uiBoneSpikeTimer          = 15000;
        m_uiBoneStormTimer          = 45000;
        m_uiBoneStormChargeTimer    = 3000;
        m_uiBoneStormColdflameTimer = 1000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId)
        {
            m_Phase = PHASE_BONE_STORM_COLDFLAME;
            ++m_uiChargesCount;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_COLDFLAME)
        {
            pSummoned->CastSpell(pSummoned, SPELL_COLDFLAME_AURA, true);

            float fX, fY;
            float fZ = pSummoned->GetPositionZ();
            // Note: the NearPoint2D function may not be correct here, because we may use a wrong Z value
            m_creature->GetNearPoint2D(fX, fY, 80.0f, m_creature->GetAngle(pSummoned));
            pSummoned->GetMotionMaster()->MovePoint(0, fX, fY, fZ, false);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_Phase)
        {
            case PHASE_NORMAL:

                // Coldflame
                if (m_uiColdflameTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_COLDFLAME) == CAST_OK)
                        m_uiColdflameTimer = 5000;
                }
                else
                    m_uiColdflameTimer -= uiDiff;

                // Bone Storm
                if (m_uiBoneStormTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_BONE_STORM) == CAST_OK)
                    {
                        // ToDo: research if we need to increase the speed here
                        DoScriptText(SAY_BONE_STORM, m_creature);
                        m_Phase = PHASE_BONE_STORM_CHARGE;
                        SetCombatMovement(false);
                        m_creature->GetMotionMaster()->MoveIdle();
                        m_uiBoneStormTimer = 90000;
                    }
                }
                else
                    m_uiBoneStormTimer -= uiDiff;

                // Bone Slice
                if (m_uiBoneSliceTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BONE_SLICE) == CAST_OK)
                        m_uiBoneSliceTimer = 1000;
                }
                else
                    m_uiBoneSliceTimer -= uiDiff;

                DoMeleeAttackIfReady();

                break;
            case PHASE_BONE_STORM_CHARGE:

                // next charge to random enemy
                if (m_uiBoneStormChargeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
                    {
                        float fX, fY, fZ;
                        pTarget->GetPosition(fX, fY, fZ);
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->GetMotionMaster()->MovePoint(1, fX, fY, fZ);
                        m_uiBoneStormChargeTimer = 3000;
                        m_Phase = PHASE_BONE_STORM_CHARGING;
                    }
                }
                else
                    m_uiBoneStormChargeTimer -= uiDiff;

                break;
            case PHASE_BONE_STORM_CHARGING:
                // waiting to arrive at target position
                break;
            case PHASE_BONE_STORM_COLDFLAME:

                if (m_uiBoneStormColdflameTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_COLDFLAME_STORM) == CAST_OK)
                    {
                        // When the max cold flame charges are reached, remove Bone storm aura
                        if (m_uiChargesCount == m_uiMaxCharges)
                        {
                            m_creature->RemoveAurasDueToSpell(SPELL_BONE_STORM);
                            m_uiBoneStormTimer = 60000;
                            m_uiBoneSliceTimer = 10000;
                            SetCombatMovement(true);
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            m_uiChargesCount = 0;
                            m_Phase = PHASE_NORMAL;
                        }
                        else
                            m_Phase = PHASE_BONE_STORM_CHARGE;

                         m_uiBoneStormColdflameTimer = 1000;
                    }
                }
                else
                    m_uiBoneStormColdflameTimer -= uiDiff;

                break;
        }

        // Bone spike - different spells for the normal phase or storm phase
        if (m_uiBoneSpikeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_Phase == PHASE_NORMAL ? SPELL_BONE_SPIKE : SPELL_BONE_SPIKE_STORM) == CAST_OK)
            {
                switch (urand(0, 2))
                {
                    case 0: DoScriptText(SAY_BONE_SPIKE_1, m_creature); break;
                    case 1: DoScriptText(SAY_BONE_SPIKE_2, m_creature); break;
                    case 2: DoScriptText(SAY_BONE_SPIKE_3, m_creature); break;
                }
                m_uiBoneSpikeTimer = 18000;
            }
        }
        else
            m_uiBoneSpikeTimer -= uiDiff;

        // Berserk
        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK))
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }
    }
};

/*####
# Bone Spike
####*/
struct MANGOS_DLL_DECL npc_bone_spikeAI : public ScriptedAI
{
    npc_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_icecrown_citadel*)pCreature->GetInstanceData());
    }

    instance_icecrown_citadel* m_pInstance;
    uint32 m_uiEmpaledTime;

    void Reset()
    {
        SetCombatMovement(false);
        m_uiEmpaledTime = 0;
    }
    void AttackStart(Unit *pWho){}

    void PassengerBoarded(Unit * pWho, int8 seatId, bool apply)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            if (apply)
                DoCastSpellIfCan(pWho, SPELL_IMPALED, CAST_TRIGGERED);
            else
                pWho->RemoveAurasDueToSpell(SPELL_IMPALED);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        m_uiEmpaledTime += uiDiff;
        if (m_pInstance && m_uiEmpaledTime > 8 * IN_MILLISECONDS)
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEVE_BONED, false);
    }
};

CreatureAI* GetAI_npc_bone_spike(Creature* pCreature)
{
    return new npc_bone_spikeAI(pCreature);
}


CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_marrowgar";
    pNewScript->GetAI = &GetAI_boss_lord_marrowgar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_bone_spike";
    pNewScript->GetAI = &GetAI_npc_bone_spike;
    pNewScript->RegisterSelf();
}
