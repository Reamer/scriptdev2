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
// Jaraxxus - Magic aura (from start?) not fully offlike implemented.
// Legion flame visual effect not imlemented

/* ScriptData
SDName: trial_of_the_crusader
SD%Complete: 80%
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Yells
{
    SAY_AGGRO                           = -1649040,
    SAY_SLAY_1                          = -1649041,
    SAY_SLAY_2                          = -1649042,
    SAY_BERSERK                         = -1649044,
    SAY_INCINERATE                      = -1649045,
    SAY_MISTRESS                        = -1649046,
    SAY_INFERNO                         = -1649047,
};

enum Equipment
{
    EQUIP_MAIN                         = 47266,
    EQUIP_DONE                         = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_LEGION_FLAME                   = 34784,
    NPC_INFERNAL_VOLCANO               = 34813,
    NPC_FEL_INFERNAL                   = 34815,
    NPC_NETHER_PORTAL                  = 34825,
};

enum BossSpells
{
    SPELL_NETHER_POWER                 = 67108,
    SPELL_FEL_FIREBALL                 = 66532,
    SPELL_FEL_LIGHTING                 = 66528,
    SPELL_INCINERATE_FLESH             = 66237,
    SPELL_BURNING_INFERNO              = 66242,
    SPELL_LEGION_FLAME                 = 66197,
    SPELL_LEGION_FLAME_AURA            = 66201,
    SPELL_ENRAGE                       = 26662,

    // Spells from Mistress
    SPELL_SPINNING_STRIKE              = 66283, // Need core support
    SPELL_MISTRESS_KISS                = 67906,
    SPELL_SHIVAN_SLASH                 = 67098,

    // Spells from Inferno
    SPELL_FEL_INFERNO                  = 67047,
    SPELL_FEL_STREAK                   = 66494,

    // Summon
    SPELL_SUMMON_VOLCAN                = 66258,
    SPELL_SUMMON_NETHER_PORTAL         = 66269,
    SPELL_SUMMON_INFERNAL_PERIODIC     = 66252,
    SPELL_SUMMON_MISTRESS_PERIODIC     = 66263,
};

/*######
## boss_jaraxxus
######*/

struct MANGOS_DLL_DECL boss_jaraxxusAI : public ScriptedAI
{
    boss_jaraxxusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Difficulty m_uiMapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsHeroic = (m_uiMapDifficulty == RAID_DIFFICULTY_10MAN_HEROIC || m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;
    bool m_bIsHeroic;

    uint32 m_uiNetherPowerTimer;
    uint32 m_uiFelFireballTimer;
    uint32 m_uiFelLightingTimer;
    uint32 m_uiIncinerateFleshTimer;
    uint32 m_uiLegionFlameTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiEnrageTimer;

    bool m_bNextVolcanoSummon;

    void Reset() 
    {
        m_uiNetherPowerTimer        = 0;
        m_uiFelFireballTimer        = 15*IN_MILLISECONDS;
        m_uiFelLightingTimer        = 12*IN_MILLISECONDS;
        m_uiIncinerateFleshTimer    = 20*IN_MILLISECONDS;
        m_uiLegionFlameTimer        = 30*IN_MILLISECONDS;
        m_uiSummonTimer             = 20*IN_MILLISECONDS;
        m_uiEnrageTimer             = 10*MINUTE*IN_MILLISECONDS;

        m_bNextVolcanoSummon        = urand(0,1) ? false : true;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, FAIL);

        m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, DONE);
    }

    void Aggro(Unit* pWho)
    {
        if (pWho->GetEntry() == NPC_FIZZLEBANG)
            return;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void EnterEvadeMode()
    {
        // Fizzlebang set Jaraxxus for update tick infight
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS)
            return;

        ScriptedAI::EnterEvadeMode();
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetEntry() == NPC_FIZZLEBANG)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2 ,m_creature/*,pVictim*/);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_INFERNAL_VOLCANO:
            case NPC_NETHER_PORTAL:
                pSummoned->CastSpell(pSummoned, pSummoned->GetEntry() == NPC_INFERNAL_VOLCANO ? SPELL_SUMMON_INFERNAL_PERIODIC : SPELL_SUMMON_MISTRESS_PERIODIC, false);
                if (!m_bIsHeroic)
                    pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                else
                    pSummoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                break;
            default:
                break;
        }

    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnrageTimer)
        {
            if (m_uiEnrageTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiEnrageTimer = 0;
                }
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        if (m_uiNetherPowerTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_NETHER_POWER) == CAST_OK)
                m_uiNetherPowerTimer = 30000;
        }
        else
            m_uiNetherPowerTimer -= uiDiff;

        if (m_uiFelFireballTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_FEL_FIREBALL, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FEL_FIREBALL) == CAST_OK)
                    m_uiFelFireballTimer = 12000;
            }
        }
        else
            m_uiFelFireballTimer -= uiDiff;

        if (m_uiFelLightingTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_FEL_LIGHTING, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FEL_LIGHTING) == CAST_OK)
                    m_uiFelLightingTimer = 25000;
            }
        }
        else
            m_uiFelLightingTimer -= uiDiff;

        if (m_uiIncinerateFleshTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_INCINERATE_FLESH, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_INCINERATE_FLESH) == CAST_OK)
                {
                    DoScriptText(SAY_INCINERATE, m_creature);
                    m_uiIncinerateFleshTimer = 22000;
                }
            }
        }
        else
            m_uiIncinerateFleshTimer -= uiDiff;

        if (m_uiLegionFlameTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_LEGION_FLAME, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_LEGION_FLAME) == CAST_OK)
                    m_uiLegionFlameTimer = 30000;
            }
        }
        else
            m_uiLegionFlameTimer -= uiDiff;

        if (m_uiSummonTimer < uiDiff)
        {
            if (m_bNextVolcanoSummon)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_VOLCAN) == CAST_OK)
                {
                    DoScriptText(SAY_INFERNO, m_creature);
                    m_uiSummonTimer = MINUTE * IN_MILLISECONDS;
                    m_bNextVolcanoSummon = false;
                }
            }
            else
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_NETHER_PORTAL) == CAST_OK)
                {
                    DoScriptText(SAY_MISTRESS, m_creature);
                    m_uiSummonTimer = MINUTE * IN_MILLISECONDS;
                    m_bNextVolcanoSummon = true;
                }
            }
        }
        else
            m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jaraxxus(Creature* pCreature)
{
    return new boss_jaraxxusAI(pCreature);
}

void AddSC_boss_jaraxxus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_jaraxxus";
    pNewScript->GetAI = &GetAI_boss_jaraxxus;
    pNewScript->RegisterSelf();
}
