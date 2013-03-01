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
SDName: blood_prince_council
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    // spells
    SPELL_BERSERK               = 26662,

    SPELL_INVOCATION_V_MOVE     = 71075,
    SPELL_INVOCATION_K_MOVE     = 71079,
    SPELL_INVOCATION_T_MOVE     = 71082,

    // Valanar
    SPELL_INVOCATION_VALANAR    = 70952,

    SPELL_SHOCK_VORTEX          = 72037,
    SPELL_SHOCK_VORTEX_AURA     = 71945, // in creature_template_addon
    SPELL_SHOCK_VORTEX_VISUAL   = 72633, // in creature_template_addon

    SPELL_EMP_SHOCK_VORTEX      = 72039,

    SPELL_KINETIC_BOMB          = 72053, // summons the target, which must summon the bomb above its head and make it move to the target
    SPELL_KINETIC_BOMB_TARGET   = 72080,
    SPELL_KINETIC_BOMB_DMG      = 72052,
    SPELL_KINETIC_BOMB_VISUAL   = 72054,
    SPELL_UNSTABLE              = 72059, // this proc makes the bomb teleport by 3-5? yards up and applies knockback aura
    SPELL_KNOCKBACK_AURA        = 72087, // this stacks to 2. duration = 3 secs, maybe this makes bomb stand for 3secs per stack?

    // Keleseth
    SPELL_INVOCATION_KELESETH   = 70981,

    SPELL_SHADOW_LANCE          = 71405,
    SPELL_EMP_SHADOW_LANCE      = 71815,

    SPELL_SHADOW_RESONANCE      = 71943,
    SPELL_SHADOW_RESONANCE_AURA = 71911, // blizz used this for determining the distance, probably. (not used)
    SPELL_SHADOW_RESONANCE_BUFF = 71822,

    SPELL_SHADOW_PRISON         = 73001,

    // Taldaram
    SPELL_INVOCATION_TALDARAM   = 70982,

    SPELL_GLITTERING_SPARKS     = 71807,

    SPELL_BALL_FLAMES_VISUAL    = 71706,
    SPELL_CONJURE_FLAME         = 71718, // dummy effect, must trigger spell summoning the sphere
    SPELL_CONJURE_EMP_FLAME     = 72040,
    SPELL_FLAMES                = 71393, // cast on the impact
    SPELL_BALL_FLAMES_PERIODIC  = 71709, // used on heroic, triggers empowered flare which reduces dmg of flames on impact
    SPELL_BALL_FLAMES_TRIGGERED = 71708, // this spell must reduce stack of 71756
    SPELL_FLAMES_BUFF           = 71756,
    SPELL_FLAME_BALL_SEARCH     = 71714,
};

enum
{
    NPC_KINETIC_BOMB_TARGET     = 38458,
    NPC_BALL_OF_FLAME           = 38332,
    NPC_BALL_OF_INFERNO_FLAME   = 38451,
    NPC_DARK_NUCLEUS            = 38369,
    NPC_SHOCK_VORTEX            = 38422,
};

// talks
enum
{
    SAY_KELESETH_INVOCATION     = -1631103,
    SAY_KELESETH_SPECIAL        = -1631104,
    SAY_KELESETH_SLAY_1         = -1631105,
    SAY_KELESETH_SLAY_2         = -1631106,
    SAY_KELESETH_BERSERK        = -1631107,
    SAY_KELESETH_DEATH          = -1631108,

    SAY_TALDARAM_INVOCATION     = -1631109,
    SAY_TALDARAM_SPECIAL        = -1631110,
    SAY_TALDARAM_SLAY_1         = -1631111,
    SAY_TALDARAM_SLAY_2         = -1631112,
    SAY_TALDARAM_BERSERK        = -1631113,
    SAY_TALDARAM_DEATH          = -1631114,

    SAY_VALANAR_INVOCATION      = -1631115,
    SAY_VALANAR_SPECIAL         = -1631116,
    SAY_VALANAR_SLAY_1          = -1631117,
    SAY_VALANAR_SLAY_2          = -1631118,
    SAY_VALANAR_BERSERK         = -1631119,
    SAY_VALANAR_DEATH           = -1631120,
};

// Blood Orb Controller
struct MANGOS_DLL_DECL npc_blood_orb_controlAI : public ScriptedAI
{
    npc_blood_orb_controlAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiInvocationTimer;
    uint32 m_uiInvocationOfBloodTarget;

    void Reset() override
    {
        m_uiInvocationTimer = 30000;
        m_uiInvocationOfBloodTarget = NPC_VALANAR;
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) override
    {
        if (pTarget)
        {
            if (pSpell->Id == SPELL_INVOCATION_V_MOVE || pSpell->Id == SPELL_INVOCATION_T_MOVE || pSpell->Id == SPELL_INVOCATION_K_MOVE)
            {
                uint32 spellId = 0;
                switch (pTarget->GetEntry())
                {
                     case NPC_VALANAR:  spellId = SPELL_INVOCATION_VALANAR; break;
                     case NPC_KELESETH: spellId = SPELL_INVOCATION_KELESETH;break;
                     case NPC_TALDARAM: spellId = SPELL_INVOCATION_TALDARAM;break;
                     default:               // should never appear
                         m_creature->MonsterSay("Unknown Target hit by Invocation Move", LANG_UNIVERSAL);
                         break;
                }
                m_uiInvocationOfBloodTarget = pTarget->GetEntry();
                DoCastSpellIfCan(m_creature, spellId, CAST_TRIGGERED);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_BLOOD_PRINCE_COUNCIL) != IN_PROGRESS)
            return;

        // battle in progress
        // every 30 seconds cast Invocation of Blood on random prince
        if (m_uiInvocationTimer < uiDiff)
        {
            uint32 spellId = 0;
            switch (m_uiInvocationOfBloodTarget)
            {
                case NPC_VALANAR:   spellId = SPELL_INVOCATION_V_MOVE; break;
                case NPC_KELESETH:  spellId = SPELL_INVOCATION_K_MOVE; break;
                case NPC_TALDARAM:  spellId = SPELL_INVOCATION_T_MOVE; break;
                default:                    // should never appear
                    m_creature->MonsterSay("Unknown NPC Entry was Target of Invocation of Blood", LANG_UNIVERSAL);
                    break;
            }
            if (DoCastSpellIfCan(m_creature, spellId, CAST_TRIGGERED) == CAST_OK)
                m_uiInvocationTimer = 30000;
        }
        else
            m_uiInvocationTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_blood_orb_control(Creature* pCreature)
{
    return new npc_blood_orb_controlAI(pCreature);
}

// base struct for Blood Prince Council
struct MANGOS_DLL_DECL base_blood_prince_council_bossAI : public ScriptedAI
{
    base_blood_prince_council_bossAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
        DoCastSpellIfCan(m_creature, SPELL_FEIGN_DEATH, CAST_TRIGGERED);
    }
    
    instance_icecrown_citadel* m_pInstance;

    bool m_bIsEmpowered;
    bool m_bIsSaidSpecial; // 1st spell cast after being empowered is followed by special say
    uint32 m_uiEmpowermentFadeTimer;
    uint32 m_uiInvocationSpellEntry;
    int32 m_iSayInvocationEntry;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSphereTimer;

    void Reset() override
    {
        m_bIsEmpowered = false;
        m_bIsSaidSpecial = false;
        m_uiEmpowermentFadeTimer = 30000;
        m_uiSphereTimer = urand(5000, 15000);
        m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        m_creature->SetHealth(1);
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage) override
    {
        if (!m_bIsEmpowered && pDealer->GetEntry() != NPC_BLOOD_ORB_CONTROL)
            uiDamage = 0;
    }

    void Aggro(Unit *pWho) override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_BLOOD_PRINCE_COUNCIL) != IN_PROGRESS)
            {
                m_pInstance->SetData(TYPE_BLOOD_PRINCE_COUNCIL, IN_PROGRESS);

                if (Creature *pOrb = m_pInstance->GetSingleCreatureFromStorage(NPC_BLOOD_ORB_CONTROL))
                {
                    uint32 uiHealth = m_creature->GetMaxHealth();
                    pOrb->Respawn();
                    pOrb->SetMaxHealth(uiHealth);
                    pOrb->SetHealth(uiHealth);
                    if (Creature* pValanar = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                    {
                        pOrb->CastSpell(pValanar, SPELL_INVOCATION_VALANAR, true);
                    }
                }
            }
        }
    }

    void JustDied(Unit *pKiller) override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_BLOOD_PRINCE_COUNCIL) != DONE)
                m_pInstance->SetData(TYPE_BLOOD_PRINCE_COUNCIL, DONE);

            if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

            if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_KELESETH))
                pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
            
            if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_TALDARAM))
                pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);

            if (Creature *pTmp = m_pInstance->GetSingleCreatureFromStorage(NPC_LANATHEL_INTRO))
                pTmp->DealDamage(pTmp, pTmp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_BLOOD_PRINCE_COUNCIL) != FAIL)
                m_pInstance->SetData(TYPE_BLOOD_PRINCE_COUNCIL, FAIL);
        }
    }
    
    void SpellHit(Unit *pCaster, const SpellEntry *pSpell) override
    {
        if (pSpell->Id == m_uiInvocationSpellEntry)
        {
            m_creature->SetHealth(pCaster->GetHealth());
            m_bIsEmpowered = true;
            DoScriptText(m_iSayInvocationEntry, m_creature);
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Invocation of Blood
        if (m_bIsEmpowered)
        {
            if (m_uiEmpowermentFadeTimer <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(m_uiInvocationSpellEntry);
                m_creature->SetHealth(1);
                m_bIsEmpowered = false;
                m_bIsSaidSpecial = false;
                m_uiEmpowermentFadeTimer = 30000;
            }
            else
                m_uiEmpowermentFadeTimer -= uiDiff;
        }

        // Berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
        }
        else
            m_uiBerserkTimer -= uiDiff;
    }
};


// Valanar
struct MANGOS_DLL_DECL boss_valanar_iccAI : public base_blood_prince_council_bossAI
{
    boss_valanar_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_VALANAR;
        m_iSayInvocationEntry = SAY_VALANAR_INVOCATION;
    }

    uint32 m_uiVortexTimer;

    void Reset() override
    {
        base_blood_prince_council_bossAI::Reset();

        m_uiVortexTimer = urand(5000, 10000);
    }

    void KilledUnit(Unit *pVictim) override
    {
        DoScriptText(urand(0, 1) ? SAY_VALANAR_SLAY_1 : SAY_VALANAR_SLAY_2 , m_creature);
    }

    void JustDied(Unit *pKiller) override
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        DoScriptText(SAY_VALANAR_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Kinetic Bomb
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Shock Vortex
        if (m_uiVortexTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsEmpowered ? SPELL_EMP_SHOCK_VORTEX : SPELL_SHOCK_VORTEX) == CAST_OK)
            {
                m_uiVortexTimer = urand(18000, 22000);

                if (m_bIsEmpowered && !m_bIsSaidSpecial)
                {
                    DoScriptText(SAY_VALANAR_SPECIAL, m_creature);
                    m_bIsSaidSpecial = false;
                }
            }
        }
        else
            m_uiVortexTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_valanar_icc(Creature* pCreature)
{
    return new boss_valanar_iccAI(pCreature);
}


// Keleseth
struct MANGOS_DLL_DECL boss_keleseth_iccAI : public base_blood_prince_council_bossAI
{
    boss_keleseth_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_KELESETH;
        m_iSayInvocationEntry = SAY_KELESETH_INVOCATION;
    }

    uint32 m_uiShadowLanceTimer;

    void Reset() override
    {
        base_blood_prince_council_bossAI::Reset();

        m_uiShadowLanceTimer = urand(2000, 5000);
        m_uiSphereTimer = 4000;
    }

    void KilledUnit(Unit *pVictim) override
    {
        DoScriptText(urand(0,1) ? SAY_KELESETH_SLAY_1 : SAY_KELESETH_SLAY_2, m_creature);
    }

    void JustDied(Unit *pKiller) override
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        DoScriptText(SAY_KELESETH_DEATH, m_creature);
    }

    void JustSummoned(Creature *pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_DARK_NUCLEUS)
            pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance->IsHeroicDifficulty())
        {
            DoCastSpellIfCan(m_creature, SPELL_SHADOW_PRISON, CAST_AURA_NOT_PRESENT);
        }

        // Shadow Resonance
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_RESONANCE) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Shadow Lance
        if (m_uiShadowLanceTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsEmpowered ? SPELL_EMP_SHADOW_LANCE : SPELL_SHADOW_LANCE) == CAST_OK)
            {
                if (m_bIsEmpowered && !m_bIsSaidSpecial)
                {
                    DoScriptText(SAY_KELESETH_SPECIAL, m_creature);
                    m_bIsSaidSpecial = true;
                }

                m_uiShadowLanceTimer = urand(2000, 5000);
            }
        }
        else
            m_uiShadowLanceTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_keleseth_icc(Creature* pCreature)
{
    return new boss_keleseth_iccAI(pCreature);
}


// Taldaram
struct MANGOS_DLL_DECL boss_taldaram_iccAI : public base_blood_prince_council_bossAI
{
    boss_taldaram_iccAI(Creature* pCreature) : base_blood_prince_council_bossAI(pCreature)
    {
        Reset();
        m_uiInvocationSpellEntry = SPELL_INVOCATION_TALDARAM;
        m_iSayInvocationEntry = SAY_TALDARAM_INVOCATION;
    }

    uint32 m_uiSparksTimer;

    void Reset() override
    {
        base_blood_prince_council_bossAI::Reset();

        m_uiSparksTimer = urand(8000, 15000);
    }

    void KilledUnit(Unit *pVictim) override
    {
        DoScriptText(urand(0,1) ? SAY_TALDARAM_SLAY_1 : SAY_TALDARAM_SLAY_2, m_creature);
    }

    void JustDied(Unit *pKiller) override
    {
        base_blood_prince_council_bossAI::JustDied(pKiller);

        DoScriptText(SAY_TALDARAM_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        base_blood_prince_council_bossAI::UpdateAI(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Conjure Flame
        if (m_uiSphereTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsEmpowered ? SPELL_CONJURE_EMP_FLAME : SPELL_CONJURE_FLAME) == CAST_OK)
                m_uiSphereTimer = 20000;
        }
        else
            m_uiSphereTimer -= uiDiff;

        // Glittering Sparks
        if (m_uiSparksTimer <= uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_GLITTERING_SPARKS, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_GLITTERING_SPARKS) == CAST_OK)
                    m_uiSparksTimer = 30000;
            }
        }
        else
            m_uiSparksTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taldaram_icc(Creature* pCreature)
{
    return new boss_taldaram_iccAI(pCreature);
}

// Ball of Flames
struct MANGOS_DLL_DECL mob_ball_of_flamesAI : public ScriptedAI
{
    mob_ball_of_flamesAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiStartTimer;
    bool m_bIsDespawned;

    void Reset() override
    {
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 7.0f, false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetLevitate(true);
        m_creature->SetDisplayId(26767);

        m_uiStartTimer = 3000;
        m_bIsDespawned = false;

        DoCastSpellIfCan(m_creature, SPELL_BALL_FLAMES_VISUAL, CAST_TRIGGERED);
        if (m_creature->GetEntry() == NPC_BALL_OF_INFERNO_FLAME)
            DoCastSpellIfCan(m_creature, SPELL_BALL_FLAMES_PERIODIC, CAST_TRIGGERED);

    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) override
    {
        if(!pTarget)
            return;

        if (pSpell->Id == SPELL_FLAME_BALL_SEARCH)
        {
            m_creature->FixateTarget(pTarget);
            m_creature->GetMotionMaster()->MoveChase(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_bIsDespawned)
            return;

        if (m_uiStartTimer)
        {
            if (m_uiStartTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FLAME_BALL_SEARCH) == CAST_OK)
                {
                    m_uiStartTimer = 0;
                }
            }
            else
                m_uiStartTimer -= uiDiff;
        }

        if (!m_creature->GetFixatedTarget())
            return;

        if (m_creature->GetDistance(m_creature->GetFixatedTarget()) < INTERACTION_DISTANCE)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FLAMES) == CAST_OK)
            {
                m_creature->ForcedDespawn(2000);
                m_creature->RemoveAllAuras();
                m_bIsDespawned = true;
            }
        }
    }
};

CreatureAI* GetAI_mob_ball_of_flames(Creature* pCreature)
{
     return new mob_ball_of_flamesAI (pCreature);
};

// Kinetic Bomb
struct MANGOS_DLL_DECL mob_kinetic_bombAI : public ScriptedAI
{
    mob_kinetic_bombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;
    bool m_bHasCast;
    bool m_bIsStarted;
    uint32 m_uiStartTimer;

    void Reset()
    {
        float x, y, z;

        m_bHasCast = false;
        m_bIsStarted = false;
        m_uiStartTimer = 3000;

        m_creature->SetSpeedRate(MOVE_WALK, 0.3f);

        m_creature->SetDisplayId(31095);

        DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_VISUAL, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_UNSTABLE, CAST_TRIGGERED);

        m_creature->SetLevitate(true);
        m_creature->GetPosition(x, y, z);
        m_creature->NearTeleportTo(x, y, z + (m_pInstance && m_pInstance->IsHeroicDifficulty() ? 10.0f : 15.0f), 0.0f);

        if (Unit *creator = m_creature->GetCreator())
        {
            creator->GetPosition(x, y, z);
        }

        m_creature->GetMotionMaster()->MovePoint(0, x, y, z, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsStarted)
        {
            if (m_uiStartTimer <= uiDiff)
            {
                m_bIsStarted = true;
            }
            else
                m_uiStartTimer -= uiDiff;

            return;
        }

        if (m_bHasCast)
            return;

        if (Unit *creator = m_creature->GetCreator())
        {
            if (m_creature->GetDistance(creator) < 1.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_KINETIC_BOMB_DMG) == CAST_OK)
                {
                    m_bHasCast = true;
                    m_creature->ForcedDespawn(3000);
                }
            }
        }
    }
};

CreatureAI* GetAI_mob_kinetic_bomb(Creature* pCreature)
{
     return new mob_kinetic_bombAI (pCreature);
};

void AddSC_blood_prince_council()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_taldaram_icc";
    pNewscript->GetAI = &GetAI_boss_taldaram_icc;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_keleseth_icc";
    pNewscript->GetAI = &GetAI_boss_keleseth_icc;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_valanar_icc";
    pNewscript->GetAI = &GetAI_boss_valanar_icc;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_blood_orb_control";
    pNewscript->GetAI = &GetAI_npc_blood_orb_control;
    pNewscript->RegisterSelf();
    
    pNewscript = new Script;
    pNewscript->Name = "npc_ball_of_flames";
    pNewscript->GetAI = &GetAI_mob_ball_of_flames;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_kinetic_bomb";
    pNewscript->GetAI = &GetAI_mob_kinetic_bomb;
    pNewscript->RegisterSelf();

}
