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
SDName: boss_krick_and_ick
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    // ick
    SPELL_POISON_NOVA             = 68989,
    SPELL_MIGHTY_KICK             = 69021,
    SPELL_PURSUED                 = 68987,
    SPELL_CONFUSION               = 69029,
    // krick
    SPELL_TOXIC_WASTE             = 69024,
    SPELL_SHADOW_BOLT             = 69028,
    SPELL_EXPLOSIVE_BARRAGE       = 69012,

    SAY_AGGRO                     = -1658024,
    SAY_SLAY1                     = -1658025,
    SAY_SLAY2                     = -1658026,
    SAY_KRICK_BARRAGE             = -1658027,
    SAY_KRICK_POISON              = -1658028,
    SAY_KRICK_CHASE_1             = -1658029,
    SAY_KRICK_CHASE_2             = -1658030,
    SAY_KRICK_CHASE_3             = -1658031,
    SAY_KRICK_BARRAGE_EMOTE       = -1658032,
    SAY_ICK_POISON_EMOTE          = -1658033,
    SAY_ICK_CHASE_EMOTE           = -1658034,
};

struct MANGOS_DLL_DECL boss_IckAI : public ScriptedAI
{
    boss_IckAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    // Ick
    uint32 m_uiPoisonNovaTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiStopAutomaticMovement;
    uint32 m_uiMightKickTimer;
    // Krick
    uint32 m_uiToxicWasteTimer;
    uint32 m_uiShadowboltTimer;
    uint32 m_uiExplosivBarrageTimer;

    void Reset()
    {
        if (m_creature->GetVehicleKit())
            m_creature->GetVehicleKit()->Reset();
        m_uiPoisonNovaTimer         = 30000;
        m_uiPursueTimer             = 10000;
        m_uiStopAutomaticMovement   = 0;
        m_uiMightKickTimer          = 20000;
        m_uiToxicWasteTimer         = 5000;
        m_uiShadowboltTimer         = 15000;
        m_uiExplosivBarrageTimer    = 35000;
    }

    void KilledUnit(Unit *victim)
    {
        if(Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
        {
            DoScriptText(urand(0,1) ? SAY_SLAY1 : SAY_SLAY2, pKrick);
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_PURSUED)
        {
            m_uiStopAutomaticMovement = 12000;
            m_creature->AI()->AttackStart(pTarget);
            SetCombatMovement(false);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK, IN_PROGRESS);

        if(Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
        {
            DoScriptText(SAY_AGGRO, pKrick);
        }
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {

        DoMeleeAttackIfReady();

        if (m_uiStopAutomaticMovement)
        {
            if (m_uiStopAutomaticMovement <= uiDiff)
            {
                m_uiStopAutomaticMovement = 0;
                SetCombatMovement(true);
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else
                m_uiStopAutomaticMovement -= uiDiff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


        if (m_uiPoisonNovaTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
            {
                if (DoCastSpellIfCan(m_creature, SPELL_POISON_NOVA) == CAST_OK)
                {
                    DoScriptText(SAY_KRICK_POISON, pKrick);
                    m_uiPoisonNovaTimer = urand(29000, 31000);
                }
            }
        }
        else
            m_uiPoisonNovaTimer -= uiDiff;

        if (m_uiToxicWasteTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_TOXIC_WASTE, SELECT_FLAG_PLAYER))
                {
                    pKrick->CastSpell(pTarget, SPELL_TOXIC_WASTE, false);
                    m_uiToxicWasteTimer = urand(9000, 11000);
                }
            }
        }
        else
            m_uiToxicWasteTimer -= uiDiff;

        if (m_uiShadowboltTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SHADOW_BOLT, SELECT_FLAG_PLAYER))
                {
                    pKrick->CastSpell(pTarget, SPELL_SHADOW_BOLT, false);
                    m_uiShadowboltTimer = urand(14000, 16000);
                }
            }
        }
        else
            m_uiShadowboltTimer -= uiDiff;

        /*if (m_uiExplosivBarrageTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
            {
                m_creature->GetMotionMaster()->MoveIdle();
                SetCombatMovement(false);
                pKrick->CastSpell(pKrick, SPELL_EXPLOSIVE_BARRAGE, false);
                DoScriptText(SAY_KRICK_BARRAGE, pKrick);
                DoScriptText(SAY_KRICK_BARRAGE_EMOTE, pKrick);
                m_uiExplosivBarrageTimer = urand(26000, 30000);
                m_uiStopAutomaticMovement = 18000;
            }
        }
        else
            m_uiExplosivBarrageTimer -= uiDiff;*/

        if (m_uiPursueTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->GetSingleCreatureFromStorage(NPC_KRICK))
            {
                if (DoCastSpellIfCan(m_creature, SPELL_PURSUED) == CAST_OK)
                {
                    switch (urand(0, 2))
                    {
                        case 0: DoScriptText(SAY_KRICK_CHASE_1, pKrick); break;
                        case 1: DoScriptText(SAY_KRICK_CHASE_2, pKrick); break;
                        case 2: DoScriptText(SAY_KRICK_CHASE_3, pKrick); break;
                        default:
                            break;
                    }
                    m_uiPursueTimer = 25000;
                }
            }
        }
        else
            m_uiPursueTimer -= uiDiff;


        if (m_uiMightKickTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_KICK) == CAST_OK)
                m_uiMightKickTimer = 25000;
        }
        else
            m_uiMightKickTimer -= uiDiff;

    }
};

struct MANGOS_DLL_DECL boss_KrickAI : public ScriptedAI
{
    boss_KrickAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }
    void AttackStart(Unit* pWho)
    {
    }
    void UpdateAI(const uint32 uiDiff)
    {
//        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
  //          return;


    }
};

CreatureAI* GetAI_boss_Ick(Creature* pCreature)
{
    return new boss_IckAI (pCreature);
}

CreatureAI* GetAI_boss_Krick(Creature* pCreature)
{
    return new boss_KrickAI (pCreature);
}

void AddSC_boss_krick_and_ick()
{
    Script *pNewscript;
    pNewscript = new Script;
    pNewscript->Name = "boss_ick";
    pNewscript->GetAI = &GetAI_boss_Ick;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_krick";
    pNewscript->GetAI = &GetAI_boss_Krick;
    pNewscript->RegisterSelf();
}
