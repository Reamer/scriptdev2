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
SDName: Boss_Eregos
SD%Complete: 60%
SDComment:
SDAuthor: originally from TC, reworked by MaxXx2021 Aka Mioka, corrected by /dev/rsa
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum Spells
{
    SAY_AGGRO            = -1578040,
    SAY_KILL_1           = -1578041,
    SAY_KILL_2           = -1578042,
    SAY_KILL_3           = -1578043,
    SAY_DEATH            = -1578044,
    SAY_ARCANE_SHIELD    = -1578045,
    SAY_FIRE_SHIELD      = -1578046,
    SAY_NATURE_SHIELD    = -1578047,
    SAY_FRENZY           = -1578048,
    SAY_SPAWN            = -1578049,

    SPELL_PLANAR_SHIFT                            = 51162,
    SPELL_ARCANE_BARRAGE_N                        = 50804,
    SPELL_ARCANE_BARRAGE_H                        = 59381,
    SPELL_ARCANE_VOLLEY_N                         = 51153,
    SPELL_ARCANE_VOLLEY_H                         = 59382,
    SPELL_ENRAGED_ASSAULT                         = 51170,
    SPELL_SUMMON_DRAKE                            = 51175,
    SPELL_PLANAR_ANOMALIES                        = 57959,
    SPELL_PLANAR_ANOMALIES_SUMMON                 = 57963,
    SPELL_PLANAR_ANOMALIES_VISUAL                 = 57971,
    SPELL_PLANAR_ANOMALIES_DMG_AURA               = 59379,

    NPC_PLANAR_ANOMALY                            = 30879,
    NPC_DRAGON                                    = 28276
};

struct MANGOS_DLL_DECL boss_eregosAI : public ScriptedAI
{
    boss_eregosAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiArcaneBarrageTimer;
    uint32 uiArcaneVolleyTimer;
    uint32 uiEnragedAssaultTimer;
    uint32 uiSummonTimer;

    float m_fHealthPercentForSpecial;

    bool m_bIsMove;

    void Reset()
    {
        m_creature->SetLevitate(true);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);

        uiArcaneBarrageTimer    = 12000;
        uiArcaneVolleyTimer     = 17000;
        uiEnragedAssaultTimer   = 24000;
        uiSummonTimer           = 15000;
        m_fHealthPercentForSpecial = 60.0f;
        m_bIsMove               = true;
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        uint8 uiText = urand(0, 2);
        switch (uiText)
        {
           case 0: DoScriptText(SAY_KILL_1, m_creature); break;
           case 1: DoScriptText(SAY_KILL_2, m_creature); break;
           case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EREGOS, DONE);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(m_creature->HasAura(SPELL_PLANAR_SHIFT))
           damage = 0;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if(pSummoned->GetEntry() == NPC_DRAGON)
           pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < m_fHealthPercentForSpecial)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_PLANAR_SHIFT) == CAST_OK)
            {
                m_fHealthPercentForSpecial -= 40.0f;
                DoCastSpellIfCan(m_creature, SPELL_PLANAR_ANOMALIES, CAST_TRIGGERED);
            }
        }

        if(m_creature->GetDistance2d(m_creature->getVictim()) > 35.0f && !m_bIsMove)
        {
            m_bIsMove = true;
            SetCombatMovement(m_bIsMove);
            if(Unit* pTarget = m_creature->getVictim())
                m_creature->GetMotionMaster()->MoveChase(pTarget);
        }

        if(m_creature->GetDistance2d(m_creature->getVictim()) < 20.0f && m_bIsMove)
        {
            m_bIsMove = false;
            SetCombatMovement(m_bIsMove);
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->StopMoving();
        }

        if(uiArcaneBarrageTimer <= diff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE_N : SPELL_ARCANE_BARRAGE_H) == CAST_OK)
                    uiArcaneBarrageTimer = 3000;
            }
        }
        else
            uiArcaneBarrageTimer -= diff;

        if(uiSummonTimer <= diff)
        {
           for(uint8 i = 1; i < 3; i++)
               DoCast(m_creature, SPELL_SUMMON_DRAKE, true);
           uiSummonTimer = 15000;
        }
        else
            uiSummonTimer -= diff;

        if(uiArcaneVolleyTimer <= diff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ARCANE_VOLLEY_N : SPELL_ARCANE_VOLLEY_H) == CAST_OK)
                uiArcaneVolleyTimer = 17000;
        }
        else
            uiArcaneVolleyTimer -= diff;

        if(uiEnragedAssaultTimer <= diff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGED_ASSAULT) == CAST_OK)
                uiEnragedAssaultTimer = 44000;
        }
        else
            uiEnragedAssaultTimer -= diff;
    }
};

struct MANGOS_DLL_DECL npc_planar_anomalyAI : public ScriptedAI
{
    npc_planar_anomalyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
       Reset();
    }

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetDisplayId(11686);
        m_creature->SetObjectScale(2.0f);
        m_creature->SetLevitate(true);
        m_creature->SetSpeedRate(MOVE_RUN, 1.5, true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (Unit* pTarget = m_creature->GetMap()->GetUnit(m_creature->GetCreatorGuid()))
            m_creature->GetMotionMaster()->MoveChase(pTarget);
        else
            m_creature->MonsterSay("I have no Creator", LANG_UNIVERSAL);
    }

    void UpdateAI(const uint32 diff)
    {
        DoCastSpellIfCan(m_creature, SPELL_PLANAR_ANOMALIES_VISUAL, CAST_AURA_NOT_PRESENT);
        DoCastSpellIfCan(m_creature, SPELL_PLANAR_ANOMALIES_DMG_AURA, CAST_AURA_NOT_PRESENT);
    }
};

CreatureAI* GetAI_boss_eregos(Creature* pCreature)
{
    return new boss_eregosAI (pCreature);
}

CreatureAI* GetAI_npc_planar_anomaly(Creature* pCreature)
{
    return new npc_planar_anomalyAI (pCreature);
}

void AddSC_boss_eregos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_eregos";
    newscript->GetAI = &GetAI_boss_eregos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_planar_anomaly";
    newscript->GetAI = &GetAI_npc_planar_anomaly;
    newscript->RegisterSelf();
}
