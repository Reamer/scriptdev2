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
SDName: Boss_Varos
SD%Complete: 70%
SDComment:
SDAuthor: originally from TC, reworked by MaxXx2021 Aka Mioka, corrected by /dev/rsa
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
    SAY_AGGRO                                     = -1578020,
    SAY_KILL_1                                    = -1578021,
    SAY_KILL_2                                    = -1578022,
    SAY_DEATH                                     = -1578023,
    SAY_AIRSTRIKE_1                               = -1578024,
    SAY_AIRSTRIKE_2                               = -1578025,
    SAY_AIRSTRIKE_3                               = -1578026,

    SPELL_ENERGIZE_CORES_SHOW                     = 61407,
    SPELL_ENERGIZE_CORES                          = 50785,
    SPELL_ENERGIZE_CORES_H                        = 59372,

    SPELL_CALL_AZURE_RING_CAPTAIN                 = 51002, //Effect    Send Event (12229)
    SPELL_CALL_AZURE_RING_CAPTAIN_2               = 51006, //Effect    Send Event (10665)
    SPELL_CALL_AZURE_RING_CAPTAIN_3               = 51007, //Effect    Send Event (18454)
    SPELL_CALL_AZURE_RING_CAPTAIN_4               = 51008, //Effect    Send Event (18455)
    SPELL_CALL_AMPLIFY_MAGIC                      = 51054,
    SPELL_CALL_AMPLIFY_MAGIC_H                    = 59371,

    NPC_AZURE_CAPTAIN                             = 28236,
    NPC_BEAM                                      = 28239,

    SPELL_CENTRIFUGE_CORE_PASSIVE                 = 50798,

    //SPELL_BEAM                                    = 51024, //need spell script on 28239
    SPELL_BEAM_DMG_AURA                           = 51019,
    SPELL_BEAM_SPAWN                              = 51022, //need script target 28239
};

struct MANGOS_DLL_DECL boss_varosAI : public ScriptedAI
{
    boss_varosAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_oculus*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_oculus* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCoreTimer;
    uint32 m_uiDragonAttackTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiAmplifyMagicTimer;

    ObjectGuid m_Core;

    bool m_bIsCastChain;

    void Reset()
    {
        m_uiCheckTimer = 2000;
        m_uiDragonAttackTimer = 10000;
        m_uiCoreTimer = urand(7000, 15000);
        m_uiAmplifyMagicTimer = urand (7000, 12000);
        m_bIsCastChain = false;
        if(m_pInstance)
            m_pInstance->CheckVarosCentrifugeConstructState();
    }

    void CheckVehicle()
    {
        Map* map = m_creature->GetMap();
        if(map && map->IsDungeon())
        {
           Map::PlayerList const &PlayerList = map->GetPlayers();

           if(PlayerList.isEmpty())
              return;

           for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
           {
              if(i->getSource()->isAlive() && i->getSource()->GetVehicle())
                 EnterEvadeMode();
           }
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if(m_pInstance)
           m_pInstance->SetData(TYPE_VAROS, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_VAROS, FAIL);
            m_pInstance->ResetDragons();
        }
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VAROS, DONE);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0,1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCheckTimer < uiDiff)
        {
            CheckVehicle();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_uiDragonAttackTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CALL_AZURE_RING_CAPTAIN) == CAST_OK)
            {
                m_uiDragonAttackTimer = 25000;
                uint8 uiText = urand(0, 2);
                switch (uiText)
                {
                    case 0: DoScriptText(SAY_AIRSTRIKE_1, m_creature); break;
                    case 1: DoScriptText(SAY_AIRSTRIKE_2, m_creature); break;
                    case 2: DoScriptText(SAY_AIRSTRIKE_3, m_creature); break;
                }
            }
        }
        else
            m_uiDragonAttackTimer -= uiDiff;

        if (m_uiAmplifyMagicTimer < uiDiff)
        {
            if  (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, m_bIsRegularMode ? SPELL_CALL_AMPLIFY_MAGIC : SPELL_CALL_AMPLIFY_MAGIC_H, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CALL_AMPLIFY_MAGIC : SPELL_CALL_AMPLIFY_MAGIC_H) == CAST_OK)
                {
                    m_uiAmplifyMagicTimer = urand(7000, 12000);
                }
            }
        }
        else
            m_uiAmplifyMagicTimer -= uiDiff;


        if(!m_bIsCastChain)
        {
            if(m_uiCoreTimer < uiDiff)
            {
                if (Creature* pCore = m_pInstance->GetFarestVaromOrb())
                {
                    m_creature->SetOrientation(m_creature->GetAngle(pCore));
                    if (DoCastSpellIfCan(pCore, SPELL_ENERGIZE_CORES_SHOW) == CAST_OK)
                    {
                        m_Core = pCore->GetObjectGuid();
                        m_uiCoreTimer = 5000;
                        m_bIsCastChain = true;
                    }
                }
            }
            else
                m_uiCoreTimer -= uiDiff;
        }
        else
        {
            if(m_uiCoreTimer < uiDiff)
            {
                if (Creature* pCore = m_creature->GetMap()->GetCreature(m_Core))
                {
                    m_creature->SetOrientation(m_creature->GetAngle(pCore));
                    if (DoCastSpellIfCan(pCore, m_bIsRegularMode ? SPELL_ENERGIZE_CORES : SPELL_ENERGIZE_CORES_H) == CAST_OK)
                    {
                        m_bIsCastChain = false;
                        m_uiCoreTimer = 5000;
                    }
                }
            }
            else
                m_uiCoreTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_varos_orbAI : public ScriptedAI
{
    npc_varos_orbAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
       Reset();
    }

    void Reset()
    {
        //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
    }

    void AttackStart(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        DoCastSpellIfCan(m_creature, SPELL_CENTRIFUGE_CORE_PASSIVE, CAST_AURA_NOT_PRESENT);
    }
};

struct MANGOS_DLL_DECL npc_varos_beam_targetAI : public ScriptedAI
{
    npc_varos_beam_targetAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
       Reset();
    }

    void Reset()
    {
        DoCastSpellIfCan(m_creature, SPELL_BEAM_DMG_AURA);
        DoCastSpellIfCan(m_creature, SPELL_BEAM_SPAWN, CAST_TRIGGERED);
        m_creature->GetMotionMaster()->MoveRandomAroundPoint(VAROS_MIDDLE_X, VAROS_MIDDLE_Y, VAROS_MIDDLE_Z, 30.0f);
    }

    void UpdateAI(const uint32 diff)
    {
    }
};

CreatureAI* GetAI_boss_varos(Creature* pCreature)
{
    return new boss_varosAI (pCreature);
}

CreatureAI* GetAI_npc_varos_orb(Creature* pCreature)
{
    return new npc_varos_orbAI (pCreature);
}

CreatureAI* GetAI_npc_varos_beam_target(Creature* pCreature)
{
    return new npc_varos_beam_targetAI (pCreature);
}

bool ProcessEventId_event_call_dragon(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (instance_oculus* pInstance = (instance_oculus*)((Creature*)pSource)->GetInstanceData())
    {
        pInstance->DoProcessCallDragonEvent();
        return true;
    }
    return false;
}

void AddSC_boss_varos()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_varos";
    pNewScript->GetAI = &GetAI_boss_varos;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_varos_orb";
    pNewScript->GetAI = &GetAI_npc_varos_orb;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_varos_beam_target";
    pNewScript->GetAI = &GetAI_npc_varos_beam_target;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_call_dragon";
    pNewScript->pProcessEventId = &ProcessEventId_event_call_dragon;
    pNewScript->RegisterSelf();
}
