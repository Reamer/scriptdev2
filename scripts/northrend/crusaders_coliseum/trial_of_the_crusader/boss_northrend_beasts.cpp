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
SDName:
SD%Complete: 0
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"
#include "Vehicle.h"

enum Emotes
{
    EMOTE_SUMMON_SNOBOLD        = -1713601,
    EMOTE_ENRAGE                = -1713504,
    EMOTE_TRAMPLE               = -1713506,
    EMOTE_STAGGERED             = -1713507,
    EMOTE_CRASH                 = -1713508,
};
enum Equipment
{
    EQUIP_MAIN                  = 47505,
    EQUIP_DONE                  = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_SLIME_POOL              = 35176,
    NPC_FIRE_BOMB               = 34854,
};

enum GormokSpells
{
    HACK_SPELL_AURA_FOR_RIDE    = 100020,
    SPELL_IMPALE                = 66331,
    SPELL_STAGGERING_STOMP      = 67648,
    SPELL_RISING_ANGER          = 66636,
    SPELL_SNOBOLLED             = 66406,
    SPELL_BATTER                = 66408,
    SPELL_FIRE_BOMB             = 66313,
    SPELL_FIRE_BOMB_DOT         = 66318,
    SPELL_HEAD_CRACK            = 66407,

};
/*######
## npc_beast_combat_stalker
######*/

enum
{
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,

    SPELL_BERSERK                       = 26662,
    SPELL_ENRAGE                        = 68335,


    PHASE_GORMOK                        = 0,
    PHASE_WORMS                         = 1,
    PHASE_ICEHOWL                       = 2,
};

struct MANGOS_DLL_DECL npc_beast_combat_stalkerAI : public Scripted_NoMovementAI
{
    npc_beast_combat_stalkerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
        m_creature->SetInCombatWithZone();
    }

    ScriptedInstance* m_pInstance;
    ObjectGuid m_aSummonedBossGuid[4];
    bool m_bFirstWormDied;
    uint32 m_uiBerserkTimer;
    uint32 m_uiAttackDelayTimer;
    uint32 m_uiNextBeastTimer;
    uint32 m_uiPhase;

    void Reset()
    {
        m_uiAttackDelayTimer = 0;
        m_uiNextBeastTimer = 0;
        m_bFirstWormDied = false;
        m_uiPhase = PHASE_GORMOK;

        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBerserkTimer    = 15*MINUTE*IN_MILLISECONDS;
        else
            m_uiBerserkTimer    = 9*MINUTE*IN_MILLISECONDS;
    }

    void MoveInLineOfSight(Unit* pWho) {}

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        uiDamage = 0;
    }

    void EnterEvadeMode()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);

        for (uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]))
                pBoss->ForcedDespawn();
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
        DoCastSpellIfCan(m_creature, HACK_SPELL_AURA_FOR_RIDE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:     m_uiPhase = PHASE_GORMOK; m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE); break;
            case NPC_DREADSCALE: m_uiPhase = PHASE_WORMS; m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE); break;
            case NPC_ICEHOWL:    m_uiPhase = PHASE_ICEHOWL; m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE); break;
            case NPC_ACIDMAW:
                // Cast emerge and delayed set in combat?
                pSummoned->SetInCombatWithZone();
                m_aSummonedBossGuid[3] = pSummoned->GetObjectGuid();
                return;
        }

        m_aSummonedBossGuid[m_uiPhase] = pSummoned->GetObjectGuid();

        pSummoned->GetMotionMaster()->MovePoint(m_uiPhase, aMovePositions[m_uiPhase][0], aMovePositions[m_uiPhase][1], aMovePositions[m_uiPhase][2], false);

        // Next beasts are summoned only for heroic modes
        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            m_uiNextBeastTimer = 150*IN_MILLISECONDS;       // 2 min 30

        m_uiAttackDelayTimer = 15000;                       // TODO, must be checked.
    }

    // Only for Dreadscale and Icehowl
    void DoSummonNextBeast(uint32 uiBeastEntry)
    {
        if (uiBeastEntry == NPC_DREADSCALE)
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_2, pTirion);

            m_creature->SummonCreature(NPC_DREADSCALE, aSpawnPositions[2][0], aSpawnPositions[2][1], aSpawnPositions[2][2], aSpawnPositions[2][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
        else
        {
            if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                DoScriptText(SAY_TIRION_BEAST_3, pTirion);

            m_creature->SummonCreature(NPC_ICEHOWL, aSpawnPositions[4][0], aSpawnPositions[4][1], aSpawnPositions[4][2], aSpawnPositions[4][3], TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (!m_pInstance)
            return;

        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                break;

            case NPC_DREADSCALE:
            case NPC_ACIDMAW:
                if (m_bFirstWormDied && m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);
                else
                {
                    if (Creature* pMateWorm = m_pInstance->GetSingleCreatureFromStorage(pSummoned->GetEntry() == NPC_ACIDMAW ? NPC_DREADSCALE : NPC_ACIDMAW))
                    {
                        pMateWorm->CastSpell(pMateWorm, SPELL_ENRAGE, true);
                    }
                    m_bFirstWormDied = true;
                }
                break;

            case NPC_ICEHOWL:
                m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
                m_creature->ForcedDespawn();
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiNextBeastTimer)
        {
            if (m_uiNextBeastTimer <= uiDiff)
            {
                if (m_uiPhase == PHASE_GORMOK)
                    DoSummonNextBeast(NPC_DREADSCALE);
                else if (m_uiPhase == PHASE_WORMS)
                    DoSummonNextBeast(NPC_ICEHOWL);

                m_uiNextBeastTimer = 0;
            }
            else
                m_uiNextBeastTimer -= uiDiff;
        }

        if (m_uiAttackDelayTimer)
        {
            if (m_uiAttackDelayTimer <= uiDiff)
            {
                if (m_uiPhase == PHASE_WORMS)
                    m_creature->SummonCreature(NPC_ACIDMAW, aSpawnPositions[3][0], aSpawnPositions[3][1], aSpawnPositions[3][2], aSpawnPositions[3][3], TEMPSUMMON_DEAD_DESPAWN, 0);

                if (Creature* pBeast = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[m_uiPhase]))
                    pBeast->SetInCombatWithZone();
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
                m_uiAttackDelayTimer = 0;
            }
            else
                m_uiAttackDelayTimer -= uiDiff;
        }

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                for (uint8 i = 0; i < 4; ++i)
                {
                    Creature* pBoss = m_creature->GetMap()->GetCreature(m_aSummonedBossGuid[i]);
                    if (pBoss && pBoss->isAlive())
                        pBoss->CastSpell(pBoss, SPELL_BERSERK, true);
                }
                m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        m_creature->SelectHostileTarget();
    }
};

CreatureAI* GetAI_npc_beast_combat_stalker(Creature* pCreature)
{
    return new npc_beast_combat_stalkerAI(pCreature);
}

/*######
## boss_gormok
######*/

struct MANGOS_DLL_DECL boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        m_uiMapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsHeroic = m_uiMapDifficulty > RAID_DIFFICULTY_25MAN_NORMAL;
        m_bIs25Man = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;
    Difficulty m_uiMapDifficulty;

    bool m_bIsHeroic;
    bool m_bIs25Man;

    uint8 NextSnoboldSeat;

    uint32 m_uiImpaleTimer;
    uint32 m_uiStaggeringStompTimer;
    uint32 m_uiThrowSnoboldTimer;

    void Reset()
    {
        if (!m_pInstance)
            return;

        SetEquipmentSlots(false, EQUIP_MAIN, -1, -1);

        NextSnoboldSeat = 0;

        m_uiImpaleTimer           = urand(15000, 30000);
        m_uiStaggeringStompTimer  = urand(20000, 25000);
        m_uiThrowSnoboldTimer     = 30 * IN_MILLISECONDS;
    }

    bool ThrowSnobold(Unit *pTarget)
    {
        Unit* pSnoboldVassal = NULL;
        if (VehicleKit* pGormokVehicle = m_creature->GetVehicleKit())
        {
            for (int i = 0; i < MAX_VEHICLE_SEAT; ++i)
            {
                pSnoboldVassal = pGormokVehicle->GetPassenger(i);
                if (pSnoboldVassal)
                    break;
            }
        }
        if (pSnoboldVassal)
        {
            if (VehicleKit* pPlayerVehicle = pTarget->GetVehicleKit())
            {
                if (pPlayerVehicle->HasEmptySeat(0))
                {
                    pSnoboldVassal->EnterVehicle(pTarget,0);
                    return true;
                }
            }
        }
        return false;
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiData == POINT_COMBAT_POSITION)
        {
            m_creature->SetInCombatWithZone();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiImpaleTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_IMPALE) == CAST_OK)
                m_uiImpaleTimer = 10000;
        }
        else
            m_uiImpaleTimer -= uiDiff;

        if (m_uiStaggeringStompTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STAGGERING_STOMP) == CAST_OK)
                m_uiStaggeringStompTimer = 20000;
        }
        else
            m_uiStaggeringStompTimer -= uiDiff;

        if (m_uiThrowSnoboldTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
            {
                if (ThrowSnobold(pTarget))
                {
                    DoScriptText(EMOTE_SUMMON_SNOBOLD, m_creature);
                    m_uiThrowSnoboldTimer = 30*IN_MILLISECONDS;
                }
            }
        }
        else
            m_uiThrowSnoboldTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

struct MANGOS_DLL_DECL mob_snobold_vassalAI : public ScriptedAI
{
    mob_snobold_vassalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    uint32 m_uiBatterTimer;
    uint32 m_uiFireBombTimer;
    uint32 m_uiHeadCrackTimer;

    ObjectGuid m_VehicleBasePlayer;

    void Reset()
    {
        m_uiBatterTimer         = 10000;
        m_uiFireBombTimer       = urand(10000, 20000);
        m_uiHeadCrackTimer      = urand(10000, 30000);
    }

    void EnteredVehicle(Unit* pWho, int8 /*seatId*/, bool apply)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            if (apply)
            {
                DoCastSpellIfCan(m_creature, SPELL_SNOBOLLED);
                DoCastSpellIfCan(m_creature, SPELL_RISING_ANGER, CAST_TRIGGERED);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS , UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                m_VehicleBasePlayer = pWho->GetObjectGuid();
            }
            else
            {
                if (Unit* pBase = m_creature->GetMap()->GetUnit(m_VehicleBasePlayer))
                {
                    pBase->RemoveAurasDueToSpell(SPELL_SNOBOLLED);
                }
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (Creature* pGormok = m_pInstance->GetSingleCreatureFromStorage(NPC_GORMOK))
            pGormok->RemoveAuraHolderFromStack(SPELL_RISING_ANGER, 1);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Snowbold need always a vehicle (Player or Gormok)
        if (!m_creature->GetVehicle() || !m_creature->GetVehicle()->GetBase())
        {
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            return;
        }

        // If Snowbold is not on player no actions
        if (m_creature->GetVehicle()->GetBase()->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_uiBatterTimer <= uiDiff)
        {
            if (Unit* pBase = m_creature->GetMap()->GetUnit(m_VehicleBasePlayer))
            {
                if (DoCastSpellIfCan(pBase, SPELL_BATTER) == CAST_OK)
                    m_uiBatterTimer = 15000;
            }
        }
        else
            m_uiBatterTimer -= uiDiff;

        if (m_uiFireBombTimer <= uiDiff)
        {
            if (Creature* pGormok = m_pInstance->GetSingleCreatureFromStorage(NPC_GORMOK))
            {
                if (Unit* pTarget = pGormok->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_FIRE_BOMB, SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_FIRE_BOMB) == CAST_OK)
                    {
                        float x, y, z;
                        pTarget->GetPosition(x, y, z);
                        if (Creature *pBomb = m_creature->SummonCreature(NPC_FIRE_BOMB, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                            pBomb->CastSpell(pBomb, SPELL_FIRE_BOMB_DOT, true);
                        m_uiFireBombTimer = urand(10000, 20000);
                    }
                }
            }
        }
        else
            m_uiFireBombTimer -= uiDiff;

        if (m_uiHeadCrackTimer <= uiDiff)
        {
            if (Unit* pBase = m_creature->GetMap()->GetUnit(m_VehicleBasePlayer))
            {
                if (DoCastSpellIfCan(pBase, SPELL_HEAD_CRACK) == CAST_OK)
                    m_uiHeadCrackTimer = urand(10000, 30000);
            }
        }
        else
            m_uiHeadCrackTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_snobold_vassal(Creature* pCreature)
{
    return new mob_snobold_vassalAI(pCreature);
}

enum AcidmawAndDreadscaleSpells
{
    SPELL_ACID_SPIT             = 66880,
    SPELL_FIRE_SPIT             = 66796,

    SPELL_PARALYTIC_SPRAY       = 66901,
    SPELL_BURNING_SPRAY         = 66902,
    
    SPELL_PARALYTIC_TOXIN       = 66823,
    SPELL_BURNING_BILE          = 66869,

    SPELL_ACID_SPEW             = 66819,
    SPELL_MOLTEN_SPEW           = 66821,
    
    SPELL_PARALYTIC_BITE        = 66824,
    SPELL_BURNING_BITE          = 66879,

    // both
    SPELL_SWEEP                 = 66794,
    SPELL_EMERGE_SLOW           = 66947,    // cast time 3 seconds - it look so in div videos that the moving worm use this for emerge
    SPELL_EMERGE_FAST           = 66949,    // cast time 2,5 seconds - it look so in div videos that the fixed worm use this for emerge
    SPELL_SUBMERGE_FAST         = 66948,    // cast time 0,75 seconds - it looks so in div videos that the moving worm use this spell for submerge
    SPELL_SUBMERGE_SLOW         = 66936,    // cast time 2 seconds - it looks so in div videos that the fixed worm use this spell for submerge
    SPELL_CHURNING_GROUND_VISUAL= 66969,

    SPELL_SUMMON_SLIME_POOL     = 66883,
    SPELL_SLIME_POOL_AURA       = 66882,

    SPELL_CHECK_ACHIEV          = 68523,
};

enum Models
{
    MODEL_ACIDMAW_STATIONARY    = 29815,
    MODEL_ACIDMAW_MOBILE        = 29816,
    MODEL_DREADSCALE_STATIONARY = 26935,
    MODEL_DREADSCALE_MOBILE     = 24564,
};

enum WormPhase
{
    PHASE_NON_ATTACK            = 3,
    PHASE_STATIONARY            = 0,
    PHASE_SUBMERGED             = 1,
    PHASE_MOBILE                = 2
};

struct MANGOS_DLL_DECL boss_acidmaw_and_dreadscaleAI : public ScriptedAI
{
    boss_acidmaw_and_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    WormPhase m_Phase;
    uint32 m_uiPhaseTimer;
    uint32 m_uiSpewTimer;
    uint32 m_uiBiteTimer;
    uint32 m_uiSpitTimer;
    uint32 m_uiSprayTimer;
    uint32 m_uiSlimePoolTimer;
    uint32 m_uiSweepTimer;

    void Reset()
    {
        if (m_creature->GetEntry() == NPC_ACIDMAW)
        {
            m_Phase = PHASE_STATIONARY;
            SetCombatMovement(false);
        }
        else    // Deadscale
        {
            m_Phase = PHASE_MOBILE;
        }
        m_uiPhaseTimer              = 40 * IN_MILLISECONDS;
        m_uiSpewTimer               = urand(10000, 20000);
        m_uiBiteTimer               = 15 * IN_MILLISECONDS;
        m_uiSpitTimer               = 1 * IN_MILLISECONDS;
        m_uiSprayTimer              = 10 * IN_MILLISECONDS;
        m_uiSlimePoolTimer          = 10 * IN_MILLISECONDS;
        m_uiSweepTimer              = 15 * IN_MILLISECONDS;
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SLIME_POOL)
            pSummoned->CastSpell(pSummoned, SPELL_SLIME_POOL_AURA, false);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_ENRAGE)
        {
            if (m_Phase != PHASE_MOBILE)
            {
                m_creature->SetDisplayId(m_creature->GetEntry() == NPC_ACIDMAW ? MODEL_ACIDMAW_MOBILE : MODEL_DREADSCALE_MOBILE);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_Phase = PHASE_MOBILE;
            }
            m_uiPhaseTimer = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_Phase)
        {
            case PHASE_MOBILE:

                if (m_uiSpewTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_creature->GetEntry() == NPC_ACIDMAW ? SPELL_ACID_SPEW : SPELL_MOLTEN_SPEW) == CAST_OK)
                        m_uiSpewTimer = urand(15000, 30000);
                }
                else
                    m_uiSpewTimer -= uiDiff;

                if (m_uiBiteTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, m_creature->GetEntry() == NPC_ACIDMAW ? SPELL_PARALYTIC_BITE : SPELL_BURNING_BITE, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, m_creature->GetEntry() == NPC_ACIDMAW ? SPELL_PARALYTIC_BITE : SPELL_BURNING_BITE) == CAST_OK)
                            m_uiBiteTimer = urand(15000, 30000);
                    }
                }
                else
                     m_uiBiteTimer -= uiDiff;

                if (m_uiSlimePoolTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_SLIME_POOL) == CAST_OK)
                        m_uiSlimePoolTimer = 12000;
                }
                else
                    m_uiSlimePoolTimer -= uiDiff;

                if (m_uiPhaseTimer)
                {
                    if (m_uiPhaseTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_FAST) == CAST_OK)
                        {
                            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            m_Phase = PHASE_SUBMERGED;
                            m_uiPhaseTimer = 7000;
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveRandomAroundPoint(563.672974f, 139.571f, m_creature->GetPositionZ(), 50.0f);
                        }
                    }
                    else
                        m_uiPhaseTimer -= uiDiff;
                }

                break;
            case PHASE_SUBMERGED:
            {
                DoCastSpellIfCan(m_creature, SPELL_CHURNING_GROUND_VISUAL, CAST_AURA_NOT_PRESENT);

                if (m_uiPhaseTimer < uiDiff)
                {
                    if (m_creature->HasAura(SPELL_SUBMERGE_FAST))
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_EMERGE_FAST) == CAST_OK)
                        {
                            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_FAST);
                            m_creature->RemoveAurasDueToSpell(SPELL_CHURNING_GROUND_VISUAL);
                            m_creature->SetDisplayId(m_creature->GetEntry() == NPC_ACIDMAW ? MODEL_ACIDMAW_STATIONARY : MODEL_DREADSCALE_STATIONARY);
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveIdle();
                            m_uiPhaseTimer = 40 * IN_MILLISECONDS;
                            m_Phase = PHASE_STATIONARY;
                            SetCombatMovement(false);
                        }
                    }
                    else
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_EMERGE_SLOW) == CAST_OK)
                        {
                            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_SLOW);
                            m_creature->RemoveAurasDueToSpell(SPELL_CHURNING_GROUND_VISUAL);
                            m_creature->SetDisplayId(m_creature->GetEntry() == NPC_ACIDMAW ? MODEL_ACIDMAW_MOBILE : MODEL_DREADSCALE_MOBILE);
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            m_uiPhaseTimer = 40 * IN_MILLISECONDS;
                            m_Phase = PHASE_MOBILE;
                            SetCombatMovement(true);
                        }
                    }
                }
                else
                    m_uiPhaseTimer -= uiDiff;
                break;
            }
            case PHASE_STATIONARY:
                if (m_uiSpitTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, m_creature->GetEntry() == NPC_ACIDMAW ? SPELL_ACID_SPIT : SPELL_FIRE_SPIT) == CAST_OK)
                            m_uiSpitTimer = 1200;
                    }
                }
                else
                    m_uiSpitTimer -= uiDiff;

                if (m_uiSprayTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if(DoCastSpellIfCan(pTarget,  m_creature->GetEntry() == NPC_ACIDMAW ? SPELL_PARALYTIC_SPRAY : SPELL_BURNING_SPRAY) == CAST_OK)
                            m_uiSprayTimer = 20000;
                    }
                }
                else
                    m_uiSprayTimer -= uiDiff;

                if (m_uiSweepTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SWEEP) == CAST_OK)
                    {
                        m_uiSweepTimer = 20000;
                        m_uiSpitTimer = 3000;
                    }
                }
                else
                    m_uiSweepTimer -= uiDiff;

                if (m_uiPhaseTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_SLOW) == CAST_OK)
                    {
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        m_Phase = PHASE_SUBMERGED;
                        m_uiPhaseTimer = 7000;
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->GetMotionMaster()->MoveRandomAroundPoint(563.672974f, 139.571f, m_creature->GetPositionZ(), 50.0f);
                    }
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;
            default:
                m_creature->MonsterSay("Unknown Phase", LANG_UNIVERSAL);
                break;
        }

        if (m_Phase == PHASE_STATIONARY || m_Phase == PHASE_MOBILE)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_acidmaw_and_dreadscale(Creature* pCreature)
{
    return new boss_acidmaw_and_dreadscaleAI(pCreature);
}

/*######
## boss_icehowl
######*/

enum
{
    EMOTE_MASSIVE_CRASH          = -1649039,

    // icehowl spells
    SPELL_FEROCIOUS_BUTT        = 66770,
    SPELL_ARCTIC_BREATH         = 66689,
    SPELL_WHIRL                 = 67345,
    SPELL_MASSIVE_CRASH         = 66683,
    SPELL_SURGE_OF_ADRENALINE   = 68667,    // used only in non heroic
    SPELL_TRAMPLE               = 66734,
    SPELL_STAGGERED_DAZE        = 66758,
    SPELL_FROTHING_RAGE         = 66759,

    POINT_ID_TRAMPLE            = 10,
};

struct MANGOS_DLL_DECL boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;

    uint32 m_uiFerociousButtTimer;
    uint32 m_uiArticBreathTimer;
    uint32 m_uiWhirlTimer;
    uint32 m_uiMassiveCrashTimer;
    uint32 m_uiTrampleTimer;
    uint32 m_uiFrothingRageTimer;

    uint8 m_uiTrampleStage;

    bool m_bIsTrample;
    bool m_bTrampleCasted;
    bool m_bMovementStarted;
    bool m_bAdrenalineCasted;

    float fPosX, fPosY, fPosZ;

    void Reset()
    {
        m_uiFerociousButtTimer  = urand(20000,30000);
        m_uiArticBreathTimer    = urand(25000,30000);
        m_uiWhirlTimer          = urand(20000,25000);
        m_uiMassiveCrashTimer   = 45000;
        m_uiTrampleTimer        = 50000;
        m_uiFrothingRageTimer   = 30000;

        m_uiTrampleStage    = 0;

        m_bMovementStarted  = false;
        m_bTrampleCasted    = false;
        m_bIsTrample        = false;
        m_bAdrenalineCasted = false;
        fPosX = 0;
        fPosY = 0;
        fPosZ = 0;
    }

    void JustReachedHome()
    {
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->DoUseDoorOrButton(GO_MAIN_GATE);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || id != POINT_ID_TRAMPLE)
            return;

        m_bMovementStarted = false;
        SetCombatMovement(true);
        if (m_creature->getVictim())
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    // every player casts surge of addrenaline on normal difficulty
    void DoCastSurgeOfAdrenaline()
    {
        Map::PlayerList const &PlayerList = m_creature->GetMap()->GetPlayers();

        if (PlayerList.isEmpty())
            return;

        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        {
            if (i->getSource()->isAlive())
                i->getSource()->CastSpell(i->getSource(), SPELL_SURGE_OF_ADRENALINE, false);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsTrample)
        {
            if(m_uiTrampleTimer < uiDiff)
            {
                switch(m_uiTrampleStage)
                {
                    // go to center
                    case 0:
                        SetCombatMovement(false);
                        m_creature->GetMotionMaster()->MoveIdle();
                        m_creature->GetMotionMaster()->MoveJump(aSpawnPositions[11][0], aSpawnPositions[11][1], aSpawnPositions[11][2], 10.0f, 10.0f);

                        ++m_uiTrampleStage;
                        m_uiTrampleTimer = 3000;
                        break;
                        // cast massive crash & stop
                    case 1:
                        if (DoCastSpellIfCan(m_creature, SPELL_MASSIVE_CRASH) == CAST_OK)
                        {
                            ++m_uiTrampleStage;
                            m_uiTrampleTimer = 8000;
                        }
                        break;
                        // wait 5 secs -> cast surge of addrenaline on players
                    case 2:
                        if (m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || m_creature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
                        {
                            if (!m_bAdrenalineCasted)
                            {
                                m_bAdrenalineCasted = true;
                                DoCastSurgeOfAdrenaline();
                            }
                        }
                        // pick a target and run for it
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTarget->GetPosition(fPosX, fPosY, fPosZ);
                            DoScriptText(EMOTE_MASSIVE_CRASH, m_creature, pTarget);
                            m_bMovementStarted = true;
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                            m_creature->SetWalk(false);
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_TRAMPLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
                            ++m_uiTrampleStage;
                            m_uiTrampleTimer = 500;
                        }
                        break;
                        // run to the target; if target hit cast Trample, else go to next phase
                    case 3:
                        if (m_bMovementStarted)
                        {
                            Map::PlayerList const &lPlayers = m_creature->GetMap()->GetPlayers();
                            if (lPlayers.isEmpty())
                                return;

                            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                            {
                                Player* pPlayer = itr->getSource();
                                if (!pPlayer)
                                    continue;

                                if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                                {
                                    DoCast(pPlayer, SPELL_TRAMPLE);
                                    m_bTrampleCasted = true;
                                    m_bMovementStarted = false;
                                    m_creature->GetMotionMaster()->MovementExpired();
                                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                                }
                            }
                        }
                        else
                            ++m_uiTrampleStage;

                        if (m_bTrampleCasted)
                            ++m_uiTrampleStage;
                        break;
                        // if trample not casted, cast stun, else continue
                    case 4:
                        if (!m_bTrampleCasted)
                        {
                            // Missing emote!
                            //DoScriptText(EMOTE_STUN, m_creature);
                            DoCast(m_creature, SPELL_STAGGERED_DAZE);
                        }
                        m_bMovementStarted = false;
                        m_bAdrenalineCasted = false;
                        m_bTrampleCasted = false;
                        SetCombatMovement(true);
                        m_creature->GetMotionMaster()->MovementExpired();
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_uiMassiveCrashTimer = urand(45000, 50000);
                        m_bIsTrample = false;
                        break;
                    default:
                        m_uiTrampleTimer = 100000;
                }
            }
            else
                m_uiTrampleTimer -= uiDiff;
        }

        // return if doing trample
        if(m_bIsTrample)
            return;

        if (m_uiFrothingRageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FROTHING_RAGE) == CAST_OK)
                m_uiFrothingRageTimer = 40000;
        }
        else
            m_uiFrothingRageTimer -= uiDiff;

        if (m_uiMassiveCrashTimer < uiDiff)
        {
            m_bIsTrample = true;
            m_uiTrampleTimer = 1000;
            m_uiTrampleStage = 0;
            m_uiMassiveCrashTimer = urand(45000, 50000);
        }
        else
            m_uiMassiveCrashTimer -= uiDiff;

        if (m_uiWhirlTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WHIRL) == CAST_OK)
                m_uiWhirlTimer = urand(20000, 25000);
        }
        else
            m_uiWhirlTimer -= uiDiff;

        if (m_uiArticBreathTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ARCTIC_BREATH) == CAST_OK)
                    m_uiArticBreathTimer = urand(25000,30000);
            }
        }
        else
            m_uiArticBreathTimer -= uiDiff;

        if (m_uiFerociousButtTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_FEROCIOUS_BUTT) == CAST_OK)
                    m_uiFerociousButtTimer = urand(20000, 30000);
            }
        }
        else
            m_uiFerociousButtTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_beast_combat_stalker";
    pNewScript->GetAI = &GetAI_npc_beast_combat_stalker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_gormok";
    pNewScript->GetAI = &GetAI_boss_gormok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_snobold_vassal";
    pNewScript->GetAI = &GetAI_mob_snobold_vassal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_acidmaw_and_dreadscale";
    pNewScript->GetAI = &GetAI_boss_acidmaw_and_dreadscale;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_icehowl";
    pNewScript->GetAI = &GetAI_boss_icehowl;
    pNewScript->RegisterSelf();
}
