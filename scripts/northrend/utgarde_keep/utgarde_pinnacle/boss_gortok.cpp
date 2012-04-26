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
SDName: Boss_Gortok
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Pinnacle
EndScriptData */

/* TODO
implement Orb - a red ball-> this Orb activate the mobs
*/


#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    // gortok handling
    SAY_AGGRO               = -1575015,
    SAY_SLAY_1              = -1575016,
    SAY_SLAY_2              = -1575017,
    SAY_DEATH               = -1575018,

    SPELL_IMPALE            = 48261,
    SPELL_IMPALE_H          = 59268,

    SPELL_WITHERING_ROAR    = 48256,
    SPELL_WITHERING_ROAR_H  = 59267,

    SPELL_ARCING_SMASH      = 48260,

    // orb handling
    SPELL_FREEZE            = 16245,

    SPELL_WAKEUP_GORTOK     = 47670,
    SPELL_WAKEUP_SUBBOSS    = 47669,

    SPELL_ORB_VISUAL        = 48044,
    
    SPELL_GORTOK_EVENT      = 48055,
    SPELL_ORB_CHANNEL       = 48048,
};

struct MANGOS_DLL_DECL npc_gortok_orbAI : public ScriptedAI
{
    npc_gortok_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pinnacle*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
    instance_pinnacle* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStepTimer;
    uint32 m_uiCheckTimer;
    uint8 m_uiStep;
    bool m_bTimeToAct;
    uint8 m_uiBossCount;
    
    ObjectGuid m_currentBossGuid;
    

    void Reset()
    {
        m_uiStepTimer = 4000;
        m_uiCheckTimer = 2000;
        m_uiStep = 1;
        m_bTimeToAct = true;
        m_uiBossCount = 0;
        m_creature->SetLevitate(true);
        m_creature->SetDisplayId(16925);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        m_creature->CastSpell(m_creature, SPELL_ORB_VISUAL, true);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == 1)
            DoAction();
    }

    void DoAction()
    {
        m_uiStepTimer = 2000;
        m_uiStep = 3;
        m_bTimeToAct = true;
    }
    
    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if ((pSpell->Id == SPELL_WAKEUP_SUBBOSS || pSpell->Id == SPELL_WAKEUP_GORTOK) && pTarget->GetTypeId() == TYPEID_UNIT)
        {
            m_currentBossGuid = pTarget->GetObjectGuid();
            pTarget->RemoveAurasDueToSpell(SPELL_FREEZE);
            pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            ((Creature*)pTarget)->SetInCombatWithZone();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_GORTOK) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_bTimeToAct)
        {
            if (m_uiCheckTimer < uiDiff)
            {
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(m_currentBossGuid))
                {
                    if (pTemp->isDead())
                        DoAction();
                    else if (!pTemp->isInCombat())
                        m_pInstance->SetData(TYPE_GORTOK, FAIL);
                }
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }
        else
        {
            if (m_uiStepTimer < uiDiff)
            {
                switch (m_uiStep)
                {
                    case 1:
                    {
                        m_creature->GetMotionMaster()->MovePoint(0, 238.61f, -460.71f, 109.57f + 4.0f, false);
                        m_uiStepTimer = 4000;
                        ++m_uiStep;
                        break;
                    }
                    case 2:
                    {
                        m_creature->GetMotionMaster()->MovePoint(1, 279.11f, -452.01f, 109.57f + 2.0f, false);
                        m_bTimeToAct = false;
                        ++m_uiStep;
                        break;
                    }
                    case 3:
                    {
                        ++m_uiBossCount;
                        bool nextBoss = m_uiBossCount > (m_bIsRegularMode ? 2 : 4);
                        if (DoCastSpellIfCan(m_creature, nextBoss ? SPELL_WAKEUP_GORTOK : SPELL_WAKEUP_SUBBOSS) == CAST_OK)
                        {
                            m_bTimeToAct = false;
                        }
                        break;
                    }
                    default:
                    {
                        if (m_pInstance)
                            m_pInstance->SetData(TYPE_GORTOK, FAIL); // should never appear
                        break;
                    }
                }
            }
            else
                m_uiStepTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_gortok_orb(Creature* pCreature)
{
    return new npc_gortok_orbAI(pCreature);
}

/*######
## boss_gortok
######*/

struct MANGOS_DLL_DECL boss_gortokAI : public ScriptedAI
{
    boss_gortokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pinnacle*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_pinnacle* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWitheringRoar;
    uint32 m_uiImpale;
    uint32 m_uiArcingSmash;

    void Reset()
    {
        m_uiWitheringRoar   = urand(7000,11000);
        m_uiImpale          = 9000;
        m_uiArcingSmash     = 5000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, DONE);
    }

    void EnterEvadeMode()
    {
        ScriptedAI::EnterEvadeMode();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
            
        if (m_pInstance->GetData(TYPE_GORTOK) != IN_PROGRESS) // something wrong with Orb
            EnterEvadeMode();

        if (m_uiWitheringRoar < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WITHERING_ROAR : SPELL_WITHERING_ROAR_H) == CAST_OK)
                m_uiWitheringRoar = urand(7000,11000);
        }
        else
            m_uiWitheringRoar -= uiDiff;
        
        if (m_uiImpale < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, 0.0f, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H) == CAST_OK)
                    m_uiImpale = 9000;
            }
        }
        else
            m_uiImpale -= uiDiff;

        if (m_uiArcingSmash < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCING_SMASH) == CAST_OK)
                m_uiArcingSmash = 5000;
        }
        else
            m_uiArcingSmash -= uiDiff;
                
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gortok(Creature* pCreature)
{
    return new boss_gortokAI(pCreature);
}

bool ProcessEventId_event_start_gortok(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (pSource->isType(TYPEMASK_WORLDOBJECT))
    {
        WorldObject* pObject = (WorldObject*)pSource;
        if (instance_pinnacle* m_pInstance = (instance_pinnacle*)pObject->GetInstanceData())
        {
            if (m_pInstance->GetData(TYPE_GORTOK) == NOT_STARTED || m_pInstance->GetData(TYPE_GORTOK) == FAIL)
            {
                pObject->SummonCreature(NPC_STASIS_CONTROLLER, 238.61f, -460.71f, 109.57f, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
                m_pInstance->SetData(TYPE_GORTOK, IN_PROGRESS);
                return true;
            }
        }
    }
    return false;
}

void AddSC_boss_gortok()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_gortok";
    pNewScript->GetAI = &GetAI_boss_gortok;
    pNewScript->RegisterSelf();
    
    pNewScript = new Script;
    pNewScript->Name = "npc_gortok_orb";
    pNewScript->GetAI = &GetAI_npc_gortok_orb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_start_gortok";
    pNewScript->pProcessEventId= &ProcessEventId_event_start_gortok;
    pNewScript->RegisterSelf();
}
