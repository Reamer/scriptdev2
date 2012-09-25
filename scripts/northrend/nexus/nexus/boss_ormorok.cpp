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
SDName: Boss_Ormorok
SD%Complete: 50%
SDComment: TODO: Correct timers. Research how spikes work, and attempt code it properly from mangos side.
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                   = -1576011,
    SAY_KILL                    = -1576012,
    SAY_REFLECT                 = -1576013,
    SAY_ICESPIKE                = -1576014,
    SAY_DEATH                   = -1576015,
    EMOTE_BOSS_GENERIC_FRENZY   = -1000005,

    SPELL_REFLECTION            = 47981,

    SPELL_CRYSTAL_SPIKES        = 47958,
    SPELL_CRYSTAL_SPIKES_DMG    = 47944,
    SPELL_CRYSTAL_SPIKES_DMG_H  = 57067,
    SPELL_CRYSTAL_SPIKES_H1     = 57082,
    SPELL_CRYSTAL_SPIKES_H2     = 57083,
    // Chain
    SPELL_SUMMON_SPIKES_NORTH   = 47954,
    SPELL_SUMMON_SPIKES_WEST    = 47955,
    SPELL_SUMMON_SPIKES_SOUTH   = 47956,
    SPELL_SUMMON_SPIKES_EAST    = 47957,
    SPELL_SUMMON_SPIKES_NORTH_EAST = 57077,
    SPELL_SUMMON_SPIKES_NORTH_WEST = 57078,
    SPELL_SUMMON_SPIKES_SOUTH_WEST = 57080,
    SPELL_SUMMON_SPIKES_SOUTH_EAST = 57081,
    // Visual
    SPELL_SUMMON_SPIKES_VISUAL_1   = 47942,
    SPELL_SUMMON_SPIKES_VISUAL_2   = 47943,


    SPELL_FRENZY                = 48017,
    SPELL_FRENZY_H              = 57086,

    SPELL_TRAMPLE               = 48016,
    SPELL_TRAMPLE_H             = 57066,

    SPELL_SUMMON_TANGLER_H      = 61564,

    NPC_CRYSTAL_SPIKE_CHAIN     = 27101,
    NPC_CRYSTAL_SPIKE_VISUAL    = 27079,
    GO_CRYSTAL_SPIKE            = 188537,
};

enum OrmorkQuadrant
{
    RIGHT_IN_FRONT,
    RIGHT_IN_BACK,
    LEFT_IN_FRONT,
    LEFT_IN_BACK
};

/*######
## boss_ormorok
######*/

struct MANGOS_DLL_DECL boss_ormorokAI : public ScriptedAI
{
    boss_ormorokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsEnraged;

    uint32 m_uiTrampleTimer;
    uint32 m_uiSpellReflectTimer;
    uint32 m_uiCrystalSpikeTimer;
    uint32 m_uiCrystalSpikeExplodeTimer;
    uint32 m_uiTanglerTimer;

    GuidList CrystalNPCList;
    GuidList CrystalGoList;

    void Reset()
    {
        m_bIsEnraged = false;

        m_uiTrampleTimer = urand(10000, 35000);
        m_uiSpellReflectTimer = urand(5000, 10000);
        m_uiCrystalSpikeTimer = urand(15000, 30000);
        m_uiCrystalSpikeExplodeTimer = 0;
        m_uiTanglerTimer = 20000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMOROK, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_CRYSTAL_SPIKE_CHAIN:
            {
                if (m_creature->GetDistance(pSummoned) < 40.0f)
                    pSummoned->CastSpell(pSummoned, GetNextSpellForQuadrant(GetOrmorkQuadrant(pSummoned)), false, NULL, NULL, m_creature->GetObjectGuid());
                pSummoned->CastSpell(m_creature, urand(0,1) ? SPELL_SUMMON_SPIKES_VISUAL_1 : SPELL_SUMMON_SPIKES_VISUAL_2, true, NULL, NULL, m_creature->GetObjectGuid());
                break;
            }
            case NPC_CRYSTAL_SPIKE_VISUAL:
            {
                CrystalNPCList.push_back(pSummoned->GetObjectGuid());
                break;
            }
            default:
                break;
        }
    }

    void JustSummoned(GameObject* pGo)
    {
        switch (pGo->GetEntry())
        {
            case GO_CRYSTAL_SPIKE:
                CrystalGoList.push_back(pGo->GetObjectGuid());
                break;
            default:
                break;
        }
    }

    uint32 GetNextSpellForQuadrant(OrmorkQuadrant ormorkQuadrant)
    {
        uint32 result = 0;
        switch (ormorkQuadrant)
        {
            case LEFT_IN_FRONT:
            {
                switch (urand (0,3))
                {
                    case 0: result = SPELL_SUMMON_SPIKES_NORTH; break;
                    case 1: result = SPELL_SUMMON_SPIKES_WEST; break;
                    default: result = SPELL_SUMMON_SPIKES_NORTH_WEST; break;
                }
                break;
            }
            case LEFT_IN_BACK:
            {
                switch (urand(0,3))
                {
                    case 0: result = SPELL_SUMMON_SPIKES_WEST; break;
                    case 1: result = SPELL_SUMMON_SPIKES_SOUTH; break;
                    default: result = SPELL_SUMMON_SPIKES_SOUTH_WEST; break;
                }
                break;
            }
            case RIGHT_IN_FRONT:
            {
                switch (urand(0,3))
                {
                    case 0: result = SPELL_SUMMON_SPIKES_NORTH; break;
                    case 1: result = SPELL_SUMMON_SPIKES_EAST; break;
                    default: result = SPELL_SUMMON_SPIKES_NORTH_EAST; break;
                }
                break;
            }
            case RIGHT_IN_BACK:
            {
                switch (urand(0,3))
                {
                    case 0: result = SPELL_SUMMON_SPIKES_EAST; break;
                    case 1: result = SPELL_SUMMON_SPIKES_SOUTH; break;
                    default: result = SPELL_SUMMON_SPIKES_SOUTH_EAST; break;
                }
                break;
            }
            default:
                break;
        }
        return result;
    }

    OrmorkQuadrant GetOrmorkQuadrant(Unit* pTarget)
    {

        // we have target in front
        if (m_creature->HasInArc(M_PI_F, pTarget))
        {
            if (IsTargetInLeftSide(pTarget))
            {
                return LEFT_IN_FRONT;
            }
            else
            {
                return RIGHT_IN_FRONT;
            }
        }
        else // target behind
        {
            if (IsTargetInLeftSide(pTarget))
            {
                return LEFT_IN_BACK;
            }
            else
            {
                return RIGHT_IN_BACK;
            }
        }
    }

    bool IsTargetInLeftSide(Unit* pTarget)
    {
        float angle = m_creature->GetAngle(pTarget);
        angle -= m_creature->GetOrientation();

        // move angle to range -pi ... +pi
        angle = MapManager::NormalizeOrientation(angle);
        if (angle > M_PI_F)
            angle -= 2.0f*M_PI_F;

        float lborder =  -1 * M_PI_F;                       // in range -pi..0
        return (( angle >= lborder ) && ( angle <= 0 ));
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                m_bIsEnraged = true;
                DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : SPELL_FRENZY_H);
            }
        }

        if (m_uiTrampleTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TRAMPLE : SPELL_TRAMPLE_H);
            m_uiTrampleTimer = urand(10000, 35000);
        }
        else
            m_uiTrampleTimer -= uiDiff;

        if (m_uiSpellReflectTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTION);
            m_uiSpellReflectTimer = urand(25000, 40000);
        }
        else
            m_uiSpellReflectTimer -= uiDiff;

        if (m_uiCrystalSpikeTimer < uiDiff)
        {
            DoScriptText(SAY_ICESPIKE, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_CRYSTAL_SPIKES);
            m_uiCrystalSpikeTimer = urand(15000, 30000);
            m_uiCrystalSpikeExplodeTimer = 5000;
        }
        else
            m_uiCrystalSpikeTimer -= uiDiff;

        if (m_uiCrystalSpikeExplodeTimer)
        {
            if (m_uiCrystalSpikeExplodeTimer <= uiDiff)
            {
                for(GuidList::const_iterator itr = CrystalGoList.begin(); itr != CrystalGoList.end(); ++itr)
                {
                    if (GameObject* pGo = m_creature->GetMap()->GetGameObject(*itr))
                    {
                        pGo->Use(m_creature);
                    }
                }
                for (GuidList::const_iterator itr = CrystalNPCList.begin(); itr != CrystalNPCList.end(); ++itr)
                {
                    if (Creature* pCrystal = m_creature->GetMap()->GetCreature(*itr))
                    {
                        pCrystal->CastSpell(pCrystal, m_bIsRegularMode ? SPELL_CRYSTAL_SPIKES_DMG : SPELL_CRYSTAL_SPIKES_DMG_H, false);
                    }
                }
                m_uiCrystalSpikeExplodeTimer = 0;
            }
        }

        if (!m_bIsRegularMode)
        {
            if (m_uiTanglerTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SUMMON_TANGLER_H);
                m_uiTanglerTimer = urand(15000, 25000);
            }
            else
                m_uiTanglerTimer -= uiDiff;
        }

        // DEBUG CODE
        Map::PlayerList const& pPlayers = m_creature->GetMap()->GetPlayers();
        if (!pPlayers.isEmpty())
        {
            for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
            {
                Player* pTarget = itr->getSource();
                switch (GetOrmorkQuadrant(pTarget))
                {
                case RIGHT_IN_FRONT:
                    pTarget->MonsterSay("right Front", LANG_UNIVERSAL);
                    break;
                case RIGHT_IN_BACK:
                    pTarget->MonsterSay("right Back", LANG_UNIVERSAL);
                    break;
                case LEFT_IN_FRONT:
                    pTarget->MonsterSay("left front", LANG_UNIVERSAL);
                    break;
                case LEFT_IN_BACK:
                    pTarget->MonsterSay("left back", LANG_UNIVERSAL);
                    break;
                default:
                    pTarget->MonsterSay("unknown", LANG_UNIVERSAL);
                    break;
                }
            }
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ormorok(Creature* pCreature)
{
    return new boss_ormorokAI(pCreature);
}

void AddSC_boss_ormorok()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ormorok";
    pNewScript->GetAI = &GetAI_boss_ormorok;
    pNewScript->RegisterSelf();
}
