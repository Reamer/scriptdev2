/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Archavon_The_Stone_Watcher
SD%Complete: 90%
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

enum
{
    SPELL_ROCK_SHARDS                       = 58678,
    SPELL_CRUSHING_LEAP_N                   = 58960,
    SPELL_CRUSHING_LEAP_H                   = 60894,
    SPELL_STOMP_N                           = 58663,
    SPELL_STOMP_H                           = 60880,
    SPELL_IMPALE_DMG_N                      = 58666,
    SPELL_IMPALE_DMG_H                      = 60882,
    SPELL_IMPALE_STUN                       = 50839,
    SPELL_BERSERK                           = 47008
};

struct MANGOS_DLL_DECL boss_archavonAI : public ScriptedAI
{
    boss_archavonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiEvadeCheckCooldown;

    uint32 m_uiBerserkTimer;
    uint32 m_uiRockShardsTimer;
    uint32 m_uiCrushingLeapTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiImpaleAfterStompTimer;

    void Reset()
    {
        m_uiEvadeCheckCooldown = 2000;
        m_uiBerserkTimer = 300000;
        m_uiRockShardsTimer = 15000;
        m_uiCrushingLeapTimer = 30000;
        m_uiStompTimer = 45000;
        m_uiImpaleAfterStompTimer = 1000;
    }

    void Aggro(Unit *pWho)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAVON, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAVON, DONE);
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ARCHAVON, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEvadeCheckCooldown < uiDiff)
        {
            if (m_creature->GetDistance2d(140.34f, -102.34f) > 80.0f)
                EnterEvadeMode();
            m_uiEvadeCheckCooldown = 2000;
        }
        else
            m_uiEvadeCheckCooldown -= uiDiff;

        if (m_uiStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STOMP_N : SPELL_STOMP_H) == CAST_OK)
            {
                m_uiImpaleAfterStompTimer = 1000;
                m_uiStompTimer = 45000+rand()%15000;
            }
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiImpaleAfterStompTimer)
        {
            if (m_uiImpaleAfterStompTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->getVictim())
                {
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_IMPALE_DMG_N : SPELL_IMPALE_DMG_H);
                    pTarget->CastSpell(pTarget, SPELL_IMPALE_STUN, true);
                }
            }
            else
                m_uiImpaleAfterStompTimer -= uiDiff;
        }

        if (m_uiRockShardsTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ROCK_SHARDS) == CAST_OK)
                    m_uiRockShardsTimer = 15000+rand()%15000;
            }
        }
        else
            m_uiRockShardsTimer -= uiDiff;

        if (m_uiCrushingLeapTimer < uiDiff)
        {
            if (Unit* m_pCrushingLeapTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER | SELECT_FLAG_NOT_IN_MELEE_RANGE))
            {
                if (DoCastSpellIfCan(m_pCrushingLeapTarget, m_bIsRegularMode ? SPELL_CRUSHING_LEAP_N : SPELL_CRUSHING_LEAP_H) == CAST_OK)
                    m_uiCrushingLeapTimer = 30000+rand()%15000;
            }
        }
        else
            m_uiCrushingLeapTimer -= uiDiff;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_archavon(Creature *pCreature)
{
    return new boss_archavonAI (pCreature);
};

void AddSC_boss_archavon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_archavon";
    newscript->GetAI = &GetAI_boss_archavon;
    newscript->RegisterSelf();
};
