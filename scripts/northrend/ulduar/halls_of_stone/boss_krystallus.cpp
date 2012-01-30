/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss Krystallus
SDAuthor: ckegg
SD%Complete: 0%
SDComment: 
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

enum
{
    SAY_AGGRO                          = -1599000,
    SAY_KILL                           = -1599001,
    SAY_DEATH                          = -1599002,
    SAY_SHATTER                        = -1599003,

    SPELL_BOULDER_TOSS                  = 50843,
    SPELL_BOULDER_TOSS_H                = 59742,
    SPELL_GROUND_SPIKE                  = 59750,
    SPELL_GROUND_SLAM                   = 50827,
    SPELL_SHATTER                       = 50810,
    SPELL_SHATTER_H                     = 61546,
    SPELL_STOMP                         = 50868,
    SPELL_STOMP_H                       = 59744,

};

/*######
## boss_krystallus
######*/

struct MANGOS_DLL_DECL boss_krystallusAI : public ScriptedAI
{
    boss_krystallusAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_halls_of_stone*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_halls_of_stone* m_pInstance;

    bool m_bIsRegularMode;

    uint32 m_uiTossTimer;
    uint32 m_uiSpikeTimer;
    uint32 m_uiSlamTimer;
    uint32 m_uiShatter_Timer;
    uint32 m_uiStompTimer;

    void Reset()
    {
        m_uiTossTimer = 3000 + rand()%6000;
        m_uiSpikeTimer = 9000 + rand()%5000;
        m_uiSlamTimer = 15000 + rand()%3000;
        m_uiStompTimer = 20000 + rand()%9000;
        m_uiShatter_Timer = 0;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, NOT_STARTED);
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, FAIL);
    }

    void EnterCombat(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO,m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiTossTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, m_bIsRegularMode ? SPELL_BOULDER_TOSS_H : SPELL_BOULDER_TOSS, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_BOULDER_TOSS : SPELL_BOULDER_TOSS_H) == CAST_OK)
                    m_uiTossTimer = 9000 + rand()%6000;
            }
        }
        else
            m_uiTossTimer -= uiDiff;

        if (m_uiSpikeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_GROUND_SPIKE, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_GROUND_SPIKE) == CAST_OK)
                    m_uiSpikeTimer = 12000 + rand()%5000;
            }
        }
        else
            m_uiSpikeTimer -= uiDiff;

        if (m_uiStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STOMP : SPELL_STOMP_H) == CAST_OK)
                m_uiStompTimer = 20000 + rand()%9000;
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiSlamTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_GROUND_SLAM) == CAST_OK)
            {
                m_uiShatter_Timer = 10000;
                m_uiSlamTimer = 15000 + rand()%3000;
            }
        }
        else
            m_uiSlamTimer -= uiDiff;

        if (m_uiShatter_Timer)
        {
            if (m_uiShatter_Timer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHATTER : SPELL_SHATTER_H) == CAST_OK)
                {
                    m_uiShatter_Timer = 0;
                }
            }
            else
                m_uiShatter_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krystallus(Creature* pCreature)
{
    return new boss_krystallusAI (pCreature);
}

void AddSC_boss_krystallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_krystallus";
    newscript->GetAI = &GetAI_boss_krystallus;
    newscript->RegisterSelf();
}
