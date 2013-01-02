/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
    SPELL_CRYSTAL_SPIKES_VISUAL = 50442,
    // Chain
    SPELL_SUMMON_SPIKE          = 47947,
    SPELL_SUMMON_SPIKES_NORTH   = 47954,
    SPELL_SUMMON_SPIKES_WEST    = 47955,
    SPELL_SUMMON_SPIKES_SOUTH   = 47956,
    SPELL_SUMMON_SPIKES_EAST    = 47957,
    //SPELL_SUMMON_SPIKES_NORTH_EAST = 57077,
    //SPELL_SUMMON_SPIKES_NORTH_WEST = 57078,
    //SPELL_SUMMON_SPIKES_SOUTH_WEST = 57080,
    //SPELL_SUMMON_SPIKES_SOUTH_EAST = 57081,
    // Visual
    //SPELL_SUMMON_SPIKES_VISUAL_1   = 47942,
    //SPELL_SUMMON_SPIKES_VISUAL_2   = 47943,


    SPELL_FRENZY                = 48017,
    SPELL_FRENZY_H              = 57086,

    SPELL_TRAMPLE               = 48016,
    SPELL_TRAMPLE_H             = 57066,

    SPELL_SUMMON_TANGLER_H      = 61564,

    NPC_CRYSTALLINE_TANGLER     = 32665,
    NPC_CRYSTAL_SPIKE_INITIAL   = 27101,
    NPC_CRYSTAL_SPIKE_TRIGGER   = 27079,
    NPC_CRYSTAL_SPIKE           = 27099,    // summon from 47947
    GO_CRYSTAL_SPIKE            = 188537,

    // Others
    SPIKE_DISTANCE = 5
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
    uint8  m_uiCrystalSpikesCount;
    uint32 m_uiCrystalSpikesTimer;
    uint32 m_uiTanglerTimer;

    float m_fBaseX;
    float m_fBaseY;
    float m_fBaseZ;
    float m_fBaseO;

    void Reset() override
    {
        m_bIsEnraged = false;

        m_uiTrampleTimer = urand(10000, 35000);
        m_uiSpellReflectTimer = urand(5000, 10000);
        m_uiCrystalSpikeTimer = urand(15000, 30000);
        m_uiCrystalSpikesCount = 0;
        m_uiCrystalSpikesTimer = 0;
        m_uiTanglerTimer = 20000;
    }

    void Aggro(Unit* pWho) override
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMOROK, DONE);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_CRYSTALLINE_TANGLER:
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pSummoned->AI()->AttackStart(pTarget);
                break;
            }
            default:
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
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

        if (m_uiCrystalSpikesTimer)
        {
            if (m_uiCrystalSpikesTimer < uiDiff)
            {
                float m_fSpikeXY[4][2];
                m_fSpikeXY[0][0] = m_fBaseX+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO));
                m_fSpikeXY[0][1] = m_fBaseY+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO));
                m_fSpikeXY[1][0] = m_fBaseX-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO));
                m_fSpikeXY[1][1] = m_fBaseY-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO));
                m_fSpikeXY[2][0] = m_fBaseX+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[2][1] = m_fBaseY+(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[3][0] = m_fBaseX-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*cos(m_fBaseO-(M_PI/2)));
                m_fSpikeXY[3][1] = m_fBaseY-(SPIKE_DISTANCE*m_uiCrystalSpikesCount*sin(m_fBaseO-(M_PI/2)));

                for (uint8 i = 0; i < 4; i++)
                    m_creature->CastSpell(m_fSpikeXY[i][0], m_fSpikeXY[i][1], m_fBaseZ, SPELL_SUMMON_SPIKE, true);

                if (++m_uiCrystalSpikesCount >= 13)
                    m_uiCrystalSpikesTimer = 0;

                m_uiCrystalSpikesTimer = 250;
            }
            else
                m_uiCrystalSpikesTimer -= uiDiff;
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
            //DoCastSpellIfCan(m_creature, SPELL_CRYSTAL_SPIKES);
            m_uiCrystalSpikeTimer = urand(15000, 30000);
            m_uiCrystalSpikesCount = 1;
            m_uiCrystalSpikesTimer = 250;
            m_fBaseX = m_creature->GetPositionX();
            m_fBaseY = m_creature->GetPositionY();
            m_fBaseZ = m_creature->GetPositionZ();
            m_fBaseO = m_creature->GetOrientation();
        }
        else
            m_uiCrystalSpikeTimer -= uiDiff;

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
