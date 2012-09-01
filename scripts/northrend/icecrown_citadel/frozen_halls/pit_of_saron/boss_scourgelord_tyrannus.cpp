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
SDName: boss_scourgelord_tyrannus
SD%Complete: 50%
SDComment: missing intro and outro; encounter need vehicle support
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_GAUNTLET_PREFIGHT_1             = -1658050,
    SAY_GAUNTLET_GENERAL_TRASH          = -1658051,
    SAY_GAUNTLET_PREFIGHT_2             = -1658052,
    SAY_AGGRO                           = -1658053,
    SAY_SLAY_1                          = -1658054,
    SAY_SLAY_2                          = -1658055,
    SAY_DEATH                           = -1658056,
    SAY_MARK                            = -1658057,
    SAY_SMASH                           = -1658058,

    EMOTE_RIMEFANG_ICEBOLT              = -1658059,
    EMOTE_SMASH                         = -1658060,

    SPELL_FORCEFUL_SMASH                = 69155,
    SPELL_OVERLORDS_BRAND               = 69172,
    SPELL_DARK_MIGHT                    = 69167,
    SPELL_HOARFROST                     = 69246,
    SPELL_MARK_OF_RIMEFANG              = 69275,
    SPELL_ICY_BLAST                     = 69233,
    SPELL_ICY_BLAST_SLOW                = 69238,

    NPC_ICY_BLAST                       = 36731,
    SPELL_ICY_BLAST_AURA                = 69238,
    SPELL_ICY_BLAST_AURA_H              = 69628,

    EQUIP_ID                            = 51796,

    NPC_DAILY_DUNGEON                   = 22852,
};

const float RimefangSummon[4] = {1013.827f, 169.71f, 628.157f, 5.31f};

enum gauntlet
{
    SAY_GAUNTLET1            = -1610081,
    SAY_GAUNTLET2            = -1610082,
    SAY_TUNNEL               = -1610083,
    // icicle event
    NPC_COLLAPSING_ICICLE    = 36847,
    SPELL_ICICLE             = 69428,
    SPELL_ICICLE_DMG         = 69426,
    SPELL_ICICLE_SUMMON      = 69424,
    // mobs
    // first 2 waves
    NPC_WRATHBRINGER         = 36840,
    NPC_FLAMEBEARER          = 36893,
    NPC_DEATHBRINGER         = 36892,
    // another 2 waves
    NPC_FALLEN_WARRIOR       = 36841,
    NPC_WRATHBONE_COLDWRAITH = 36842,
    NPC_WRATHBONE_SORCERER   = 37728,    // this is for the end event, not used
    NPC_GLACIAL_REVENANT     = 36874,
};

struct MANGOS_DLL_DECL boss_rimefangAI : public ScriptedAI
{
    boss_rimefangAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        m_uiMainTargetGUID.Clear();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;

    uint32 m_uiHoarfrostTimer;
    uint32 m_uiIcyBlastTimer;
    uint32 m_uiIcyBlastSlowTimer;
    ObjectGuid m_uiMainTargetGUID;

    void Reset()
    {
        m_uiHoarfrostTimer      = 25000;
        m_uiIcyBlastTimer       = 35000;
        m_uiIcyBlastSlowTimer   = 30000;
        m_uiMainTargetGUID.Clear();
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
    }

    void SetMainTarget(ObjectGuid m_uiTargetGUID)
    {
        m_uiMainTargetGUID = m_uiTargetGUID;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if(pSummoned->GetEntry() == NPC_ICY_BLAST)
            pSummoned->CastSpell(pSummoned, SPELL_ICY_BLAST_AURA, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHoarfrostTimer < uiDiff)
        {
            Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiMainTargetGUID);
            if (!pTarget)
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_HOARFROST) == CAST_OK)
                {
                    DoScriptText(EMOTE_RIMEFANG_ICEBOLT, m_creature);
                    m_uiHoarfrostTimer = 20000;
                }
            }
        }
        else
            m_uiHoarfrostTimer -= uiDiff;

        if (m_uiIcyBlastTimer < uiDiff)
        {
            Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiMainTargetGUID);
            if (!pTarget)
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ICY_BLAST) == CAST_OK)
                    m_uiIcyBlastTimer = 35000;
            }
        }
        else
            m_uiIcyBlastTimer -= uiDiff;

        if (m_uiIcyBlastSlowTimer < uiDiff)
        {
            Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiMainTargetGUID);
            if (!pTarget)
                pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget)
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ICY_BLAST_SLOW))
                    m_uiIcyBlastSlowTimer = 40000;
            }
        }
        else
            m_uiIcyBlastSlowTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_tyrannusAI : public ScriptedAI
{
    boss_tyrannusAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        m_uiRimefangGUID.Clear();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;

    uint32 m_uiForcefulSmashTimer;
    uint32 m_uiOverlordsBrandTimer;
    uint32 m_uiDarkMightTimer;
    uint32 m_uiMarkOfRimefangTimer;

    ObjectGuid m_uiRimefangGUID;

    void Reset()
    {
        m_uiRimefangGUID.Clear();
        m_uiForcefulSmashTimer  = 10000;
        m_uiOverlordsBrandTimer = 35000;
        m_uiDarkMightTimer      = 40000;
        m_uiMarkOfRimefangTimer = 30000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, FAIL);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->SummonCreature(NPC_DAILY_DUNGEON, 1032.35f, 198.31f, 628.15f, 4.73f, TEMPSUMMON_MANUAL_DESPAWN, 5000);

        DoScriptText(SAY_DEATH, m_creature);

        // Temp hack until outro is implemented
        if (Creature* pRimefang = m_pInstance->instance->GetCreature(m_uiRimefangGUID))
        {
            pRimefang->GetMotionMaster()->Clear();
            pRimefang->GetMotionMaster()->MovePoint(0, 844.752f, 358.993f, 645.330f);
            pRimefang->setFaction(35);
            pRimefang->DeleteThreatList();
            pRimefang->RemoveAllAuras();
            pRimefang->ForcedDespawn(10000);
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, DONE);

    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiForcefulSmashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FORCEFUL_SMASH) == CAST_OK)
                m_uiForcefulSmashTimer = 50000;
        }
        else
            m_uiForcefulSmashTimer -= uiDiff;

        if (m_uiOverlordsBrandTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_OVERLORDS_BRAND) == CAST_OK)
                    m_uiOverlordsBrandTimer = 45000;
            }
        }
        else
            m_uiOverlordsBrandTimer -= uiDiff;

        if (m_uiDarkMightTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DARK_MIGHT) == CAST_OK)
            {
                DoScriptText(SAY_SMASH, m_creature);
                DoScriptText(EMOTE_SMASH, m_creature);
                m_uiDarkMightTimer = 60000;
            }
        }
        else
            m_uiDarkMightTimer -= uiDiff;

        if (m_uiMarkOfRimefangTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_MARK_OF_RIMEFANG) == CAST_OK)
                {
                    if (Creature* pRimefang = m_pInstance->GetSingleCreatureFromStorage(NPC_RIMEFANG))
                        ((boss_rimefangAI*)pRimefang->AI())->SetMainTarget(pTarget->GetObjectGuid());

                    DoScriptText(SAY_MARK, m_creature);
                    m_uiMarkOfRimefangTimer = urand(30000, 40000);
                }
            }
        }
        else
            m_uiMarkOfRimefangTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_colapsing_icicleAI: public ScriptedAI
{
    npc_colapsing_icicleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        m_creature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiDamageTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_ICICLE);
        m_uiDamageTimer = 500;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiDamageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_ICICLE_DMG);
            m_uiDamageTimer = 10000;
        }
        else m_uiDamageTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_tyrannus(Creature* pCreature)
{
    return new boss_tyrannusAI (pCreature);
}

CreatureAI* GetAI_boss_rimefang(Creature* pCreature)
{
    return new boss_rimefangAI (pCreature);
}

CreatureAI* GetAI_npc_colapsing_icicle(Creature* pCreature)
{
    return new npc_colapsing_icicleAI (pCreature);
}

void AddSC_boss_tyrannus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name="boss_scourgelord_tyrannus";
    pNewScript->GetAI = &GetAI_boss_tyrannus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name="boss_rimefang";
    pNewScript->GetAI = &GetAI_boss_rimefang;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name="npc_colapsing_icicle";
    pNewScript->GetAI = &GetAI_npc_colapsing_icicle;
    pNewScript->RegisterSelf();
}
