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
SDName: boss_professor_putricide
SD%Complete: 85%
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
    // controlled abomination
    SPELL_MUTATED_TRANSFORMATION    = 70308,
    SPELL_EAT_OOZE                  = 72527,
    SPELL_REGURGITATED_OOZE         = 70539,
    SPELL_MUTATED_SLASH             = 70542,
    SPELL_MUTATED_AURA              = 70405,
    SPELL_ABOMINATION_POWER_DRAIN   = 70385, // prevents normal regen of abomination's power

    // green ooze
    SPELL_OOZE_ADHESIVE             = 70447,
    SPELL_OOZE_ERUPTION             = 70492,

    // orange ooze
    SPELL_GASEOUS_BLOAT             = 70672,
    SPELL_EXPUNGED_GAS              = 70701,
    SPELL_GASEOUS_BLOAT_VISUAL      = 70215,

    // Assistance Spells from other Encounter
    SPELL_OOZE_FLOOD_TRIGGER        = 69795,
    SPELL_VILE_GAS                  = 72272,
    SPELL_MALLEABLE_GOO_ASSIST      = 70852,

    // Professor spells
    SPELL_BERSERK                   = 47008,
    SPELL_UNSTABLE_EXPERIMENT       = 70351,
    SPELL_VOLATILE_EXPERIMENT       = 72842, // heroic
    SPELL_GREEN_OOZE_SUMMON         = 71412,
    SPELL_ORANGE_OOZE_SUMMON        = 71415,

    SPELL_SLIME_PUDDLE_SEARCH_SPELL = 71424,
    SPELL_SLIME_PUDDLE_SUMMON_TRIG  = 71425,
    SPELL_SLIME_PUDDLE              = 70341,
    SPELL_SLIME_PUDDLE_SUMMON       = 70342,

    SPELL_SLIME_PUDDLE_AURA         = 70343,    
    SPELL_GROW_STACKER_GROW_AURA    = 70345,

    SPELL_MALLEABLE_GOO             = 72295,

    SPELL_CHOKING_GAS_BOMB          = 71255,
    SPELL_CHOKING_GAS_BOMB_AURA     = 71259,
    SPELL_CHOKING_GAS_BOMB_EXPL_AUR = 71280,
    SPELL_CHOKING_GAS_EXPLOSION     = 71279,

    // phase transitions
    SPELL_TEAR_GAS_1                = 71615,
    SPELL_TEAR_GAS                  = 71617,
    SPELL_TEAR_GAS_2                = 71618,
    SPELL_TEAR_GAS_PERIODIC_AURA    = 73170,
    SPELL_TEAR_GAS_CANCEL           = 71620,

    SPELL_CREATE_CONCOCTION         = 71621,
    SPELL_GUZZLE_POTIONS            = 71893,

    SPELL_MUTATED_PLAGUE            = 72451,
    SPELL_MUTATED_PLAGUE_CLEAR      = 72618,

    // heroic
    SPELL_UNBOUND_PLAGUE            = 70911,
    SPELL_OOZE_VARIABLE             = 70352, // aura 303 - dont allow taking damage from attacker with linked aura303?
    SPELL_OOZE_VARIABLE_OOZE        = 74118, // anyway, implemented as hardcoded in script
    SPELL_GAS_VARIABLE              = 70353,
    SPELL_GAS_VARIABLE_GAS          = 74119,

    // NPCs
    NPC_GREEN_ORANGE_OOZE_STALKER   = 37824, // casts orange and green visual spell and then summons add
    NPC_GROWING_OOZE_PUDDLE         = 37690,
    NPC_GROWING_OOZE_PUDDLE_TRIGGER = 38234,
    NPC_CHOKING_GAS_BOMB            = 38159,
    NPC_VOLATILE_OOZE               = 37697,
    NPC_MUTATED_ABOMINATION         = 37672,
    NPC_MALLEABLE_GOO               = 38556,

/*
    SPELL_OOZE_GAS_PROTECTION     = 70812,
    SPELL_OOZE_BEAM_PROTECTION    = 71530,
    SPELL_OOZE_TANK_PROTECTION    = 71770,

    SPELL_ORANGE_RADIATION        = 45857, //Additional visual

    QUEST_24749                   = 71518,
    SHADOW_INFUSION_AURA          = 71516,

    VIEW_1                        = 30881,
    VIEW_2                        = 30881,
    VIEW_3                        = 30993,
*/
};

// talks
enum
{
    SAY_AGGRO                   = -1631092,
    SAY_AIRLOCK                 = -1631093,
    SAY_PHASE_CHANGE            = -1631094,
    SAY_TRANSFORM_1             = -1631095,
    SAY_TRANSFORM_2             = -1631096,
    SAY_SLAY_1                  = -1631097,
    SAY_SLAY_2                  = -1631098,
    SAY_BERSERK                 = -1631099,
    SAY_DEATH                   = -1631100,

    // during other fights
    SAY_SLIME_FLOW_1            = -1631074,
    SAY_SLIME_FLOW_2            = -1631075,
    SAY_BLIGHT                  = -1631083,
    SAY_FESTERGUT_DEATH         = -1631091,
    SAY_ROTFACE_DEATH           = -1631080,
};

static LOCATION SpawnLoc[]=
{
    {4356.779785f, 3263.510010f, 389.398010f, 1.586f},  // 0 Putricide start point o=1.586
    {4295.081055f, 3188.883545f, 389.330261f, 4.270f},  // 1 Puticide Festergut say, o=4.27
    {4417.302246f, 3188.219971f, 389.332520f, 5.102f},  // 2 Putricide Rotface say o=5.102
    {4388.1f, 3213.29f, 408.7399f, 3.8397f},            // 3 Ooze stalker - green
    {4324.7212f, 3214.6428f, 408.7399f, 5.5764f},       // 3 Ooze stalker - orange
};

#define POINT_PUTRICIDE_SPAWN 1

enum PutricidePhases
{
    PHASE_ONE               = 1,
    PHASE_RUNNING_ONE       = 2,
    PHASE_TRANSITION_ONE    = 3,
    PHASE_TWO               = 4,
    PHASE_RUNNING_TWO       = 5,
    PHASE_TRANSITION_TWO    = 6,
    PHASE_THREE             = 7
};


/**
 * boss_professor_putricide
 */
struct MANGOS_DLL_DECL boss_professor_putricideAI : public ScriptedAI
{
    boss_professor_putricideAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    PutricidePhases m_Phase;

    uint32 m_uiHealthCheckTimer;
    uint32 m_uiTransitionTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSlimePuddleTimer;
    uint32 m_uiUnstableExperimentTimer;
    uint32 m_uiMalleableGooTimer;
    uint32 m_uiChokingGasBombTimer;
    uint32 m_uiMutatedPlagueTimer;
    uint32 m_uiUnboundPlagueTimer;

    bool m_bIsGreenOoze; // green or orange ooze to summon

    uint32 m_uiAssistanceSpellTimer;

    void Reset() override
    {
        m_Phase                     = PHASE_ONE;
        SetCombatMovement(true);

        m_bIsGreenOoze              = true; // first ooze summoned is always green

        m_uiHealthCheckTimer        = 1000;
        m_uiTransitionTimer         = 15000;
        m_uiEnrageTimer             = 10 * MINUTE * IN_MILLISECONDS;
        m_uiSlimePuddleTimer        = 10000;
        m_uiUnstableExperimentTimer = 20000;
        m_uiMalleableGooTimer       = 5000;
        m_uiChokingGasBombTimer     = urand(10000, 15000);
        m_uiMutatedPlagueTimer      = 0;
        m_uiUnboundPlagueTimer      = 10000;

        m_uiAssistanceSpellTimer = 10000;
    }

    void DamageTaken(Unit *pDealer, uint32 &uiDamage) override
    {
        if (IsAssisting())
            uiDamage = 0;
    }

    void KilledUnit(Unit* pVictim) override
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        if (IsAssisting())
        {
            SetCombatMovement(false);
            return;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PROFESSOR_PUTRICIDE, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *pKiller) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PROFESSOR_PUTRICIDE, DONE);

        DoCastSpellIfCan(m_creature, SPELL_MUTATED_PLAGUE_CLEAR, CAST_TRIGGERED);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome() override
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_PROFESSOR_PUTRICIDE) == IN_PROGRESS)
            m_pInstance->SetData(TYPE_PROFESSOR_PUTRICIDE, FAIL);

        if (VehicleKitPtr pKit = m_creature->GetVehicleKit())
        {
            pKit->RemoveAllPassengers();
            pKit->Reset();
        }

        DoCastSpellIfCan(m_creature, SPELL_MUTATED_PLAGUE_CLEAR, CAST_TRIGGERED);
        // some weird bug with not regenerating health after wipe ;/
        // m_creature->SetHealth(m_creature->GetMaxHealth());
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData) override
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_PUTRICIDE_SPAWN)
        {
            if (m_Phase == PHASE_RUNNING_ONE)
            {
                DoCastSpellIfCan(m_creature, SPELL_CREATE_CONCOCTION);
                DoScriptText(SAY_TRANSFORM_1, m_creature);
                m_uiTransitionTimer = m_pInstance->IsHeroicDifficulty() ? 30 * IN_MILLISECONDS : 4 * IN_MILLISECONDS;
                m_Phase = PHASE_TRANSITION_ONE; // counter for entering phase 2
            }
            else if (m_Phase == PHASE_RUNNING_TWO)
            {
                DoCastSpellIfCan(m_creature, SPELL_GUZZLE_POTIONS);
                m_uiTransitionTimer = 4 * IN_MILLISECONDS;
                if (m_pInstance->IsHeroicDifficulty())
                    m_uiTransitionTimer = m_pInstance->Is25ManDifficulty() ? 20 * IN_MILLISECONDS : 30 * IN_MILLISECONDS;
                DoScriptText(SAY_TRANSFORM_2, m_creature);
                m_Phase = PHASE_TRANSITION_TWO; // counter for entering phase 3
            }
        }
    }

    void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpell) override
    {
        if (pSpell->Id == SPELL_OOZE_FLOOD_TRIGGER)
            DoScriptText(urand(0, 1) ? SAY_SLIME_FLOW_1 : SAY_SLIME_FLOW_2, m_creature);
    }

    void DoExperiment(bool green, bool both = false)
    {
        if (green || both)
        {
            if (Unit *pGreen = m_creature->SummonCreature(NPC_GREEN_ORANGE_OOZE_STALKER, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, SpawnLoc[3].o, TEMPSUMMON_TIMED_DESPAWN, 10000))
                DoCastSpellIfCan(m_creature, SPELL_GREEN_OOZE_SUMMON, CAST_TRIGGERED);
        }

        if (!green || both)
        {
            if (Unit *pOrange = m_creature->SummonCreature(NPC_GREEN_ORANGE_OOZE_STALKER, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, SpawnLoc[4].o, TEMPSUMMON_TIMED_DESPAWN, 10000))
                DoCastSpellIfCan(m_creature, SPELL_ORANGE_OOZE_SUMMON, CAST_TRIGGERED);
        }
    }

    bool IsAssisting()
    {
        return m_pInstance &&
                (m_pInstance->GetData(TYPE_FESTERGUT) == IN_PROGRESS || m_pInstance->GetData(TYPE_ROTFACE) == IN_PROGRESS);
    }

    void JustSummoned(Creature *pSummoned) override
    {
        if (pSummoned->GetEntry() != NPC_GREEN_ORANGE_OOZE_STALKER)
            pSummoned->SetInCombatWithZone();

        switch (pSummoned->GetEntry())
        {
            case NPC_GROWING_OOZE_PUDDLE_TRIGGER:
            {
                DoCastSpellIfCan(pSummoned, SPELL_SLIME_PUDDLE, CAST_TRIGGERED);
                break;
            }
            case NPC_GROWING_OOZE_PUDDLE:
            {
                pSummoned->CastSpell(pSummoned, SPELL_SLIME_PUDDLE_AURA, true);
                pSummoned->CastSpell(pSummoned, SPELL_GROW_STACKER_GROW_AURA, true);
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                break;
            }
            case NPC_CHOKING_GAS_BOMB:
            {
                pSummoned->CastSpell(pSummoned, SPELL_CHOKING_GAS_BOMB_AURA, true);
                // creature is already despawned and spell doesn't hit anybody, this need proper implementation in core
                pSummoned->CastSpell(pSummoned, SPELL_CHOKING_GAS_BOMB_EXPL_AUR, true);
                pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                break;
            }
        }
    }

    void UpdateMalleableGoo(const uint32 uiDiff)
    {
        // Malleable Goo
        if (m_uiMalleableGooTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_MALLEABLE_GOO) == CAST_OK)
                m_uiMalleableGooTimer = 20000;
        }
        else
            m_uiMalleableGooTimer -= uiDiff;
    }

    void UpdateSlimePuddleTimer(const uint32 uiDiff)
    {
        // Slime Puddle
        if (m_uiSlimePuddleTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SLIME_PUDDLE_SEARCH_SPELL) == CAST_OK)
                m_uiSlimePuddleTimer = urand (33000, 36000);
        }
        else
            m_uiSlimePuddleTimer -= uiDiff;
    }

    void UpdateUnstableExperimentTimer(const uint32 uiDiff)
    {
        // Unstable Experiment
        if (m_uiUnstableExperimentTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_UNSTABLE_EXPERIMENT) == CAST_OK)
            {
                DoExperiment(m_bIsGreenOoze);
                m_uiUnstableExperimentTimer = 30000;
                m_bIsGreenOoze = !m_bIsGreenOoze;
            }
        }
        else
            m_uiUnstableExperimentTimer -= uiDiff;
    }

    void UpdateUnboundPlagueTimer(const uint32 uiDiff)
    {
        // Unbound Plague
        if (m_pInstance->IsHeroicDifficulty())
        {
            if (m_uiUnboundPlagueTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_UNBOUND_PLAGUE) == CAST_OK)
                    m_uiUnboundPlagueTimer = 70000;
            }
            else
                m_uiUnboundPlagueTimer -= uiDiff;
        }
    }

    void UpdateChokingGasBombTimer(const uint32 uiDiff)
    {
        // Choking Gas
        if (m_uiChokingGasBombTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CHOKING_GAS_BOMB) == CAST_OK)
                m_uiChokingGasBombTimer = urand(25000, 30000);
        }
        else
            m_uiChokingGasBombTimer -= uiDiff;
    }

    void UpdateEnrageTimer(const uint32 uiDiff)
    {
        // Enrage
        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiEnrageTimer = 30 * MINUTE * IN_MILLISECONDS;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_pInstance && IsAssisting())
        {
            if (m_pInstance->IsHeroicDifficulty())
            {
                if (m_pInstance->GetData(TYPE_ROTFACE) == IN_PROGRESS)
                {
                    if (m_uiAssistanceSpellTimer <= uiDiff)
                    {
                        if (Creature* pRotface = m_pInstance->GetSingleCreatureFromStorage(NPC_ROTFACE))
                        {
                            if (Unit* pTarget = pRotface->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_VILE_GAS, SELECT_FLAG_PLAYER))
                            {
                                if (DoCastSpellIfCan(m_creature, SPELL_VILE_GAS) == CAST_OK)
                                    m_uiAssistanceSpellTimer = 30000;
                            }
                        }
                    }
                    else
                        m_uiAssistanceSpellTimer -= uiDiff;
                }

                if (m_pInstance->GetData(TYPE_FESTERGUT) == IN_PROGRESS)
                {
                    if (m_uiAssistanceSpellTimer <= uiDiff)
                    {
                        if (Creature* pRotface = m_pInstance->GetSingleCreatureFromStorage(NPC_FESTERGUT))
                        {
                            if (Unit* pTarget = pRotface->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_VILE_GAS, SELECT_FLAG_PLAYER))
                            {
                                if (DoCastSpellIfCan(pTarget, SPELL_MALLEABLE_GOO_ASSIST) == CAST_OK)
                                    m_uiAssistanceSpellTimer = 30000;
                            }
                        }
                    }
                    else
                        m_uiAssistanceSpellTimer -= uiDiff;
                }
            }
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        UpdateEnrageTimer(uiDiff);
        UpdateUnboundPlagueTimer(uiDiff);
        UpdateSlimePuddleTimer(uiDiff);

        switch(m_Phase)
        {
            case PHASE_ONE:
            {
                // health check
                if (m_uiHealthCheckTimer <= uiDiff)
                {
                    if (m_creature->GetHealthPercent() <= 80.0f)
                    {
                        if (m_pInstance->IsHeroicDifficulty())
                        {
                            if (DoCastSpellIfCan(m_creature, SPELL_VOLATILE_EXPERIMENT) == CAST_OK)
                            {
                                DoExperiment(true, true);
                                DoScriptText(SAY_PHASE_CHANGE, m_creature);
                                m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                                SetCombatMovement(false);
                                m_Phase = PHASE_RUNNING_ONE;
                            }
                        }
                        else
                        {
                            if (DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS) == CAST_OK)
                            {
                                m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                                SetCombatMovement(false);
                                m_Phase = PHASE_RUNNING_ONE;
                            }                            
                        }
                        return;
                    }
                    m_uiHealthCheckTimer = 1000;
                }
                else
                    m_uiHealthCheckTimer -= uiDiff;

                UpdateUnstableExperimentTimer(uiDiff);
                break;
            }
            case PHASE_TRANSITION_ONE:
            {
                if (m_uiTransitionTimer <= uiDiff)
                {
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiTransitionTimer = 15000;
                    m_Phase = PHASE_TWO;
                    DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_CANCEL, CAST_TRIGGERED); // no affect in heroic mode, because no Tear Gas
                }
                else
                    m_uiTransitionTimer -= uiDiff;

                return;
            }
            case PHASE_TWO:
            {
                // health check
                if (m_uiHealthCheckTimer <= uiDiff)
                {
                    if (m_creature->GetHealthPercent() <= 35.0f)
                    {
                        if (m_pInstance->IsHeroicDifficulty())
                        {
                            if (DoCastSpellIfCan(m_creature, SPELL_VOLATILE_EXPERIMENT) == CAST_OK)
                            {
                                DoExperiment(true, true);
                                DoScriptText(SAY_PHASE_CHANGE, m_creature);
                                SetCombatMovement(false);
                                m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                                m_Phase = PHASE_RUNNING_TWO;
                            }
                        }
                        else
                        {
                            if (DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS) == CAST_OK)
                            {
                                SetCombatMovement(false);
                                m_creature->GetMotionMaster()->MovePoint(POINT_PUTRICIDE_SPAWN, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                                m_Phase = PHASE_RUNNING_TWO;
                            }
                        }
                        return;
                    }
                    m_uiHealthCheckTimer = 1000;
                }
                else
                    m_uiHealthCheckTimer -= uiDiff;

                UpdateMalleableGoo(uiDiff);
                UpdateUnstableExperimentTimer(uiDiff);
                UpdateChokingGasBombTimer(uiDiff);
                break;
            }
            case PHASE_TRANSITION_TWO:
            {
                if (m_uiTransitionTimer <= uiDiff)
                {
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiTransitionTimer = 15000;
                    m_Phase = PHASE_THREE;
                    DoCastSpellIfCan(m_creature, SPELL_TEAR_GAS_CANCEL, CAST_TRIGGERED); // no affect in heroic mode, because no Tear Gas
                }
                else
                    m_uiTransitionTimer -= uiDiff;

                return;
            }
            case PHASE_THREE:
            {
                UpdateMalleableGoo(uiDiff);
                UpdateChokingGasBombTimer(uiDiff);
                break;
            }
            case PHASE_RUNNING_ONE:
            case PHASE_RUNNING_TWO:
                // wait for arriving at the table (during phase transition)
                break;
            default:
                m_creature->MonsterSay("Unknown Phase", LANG_UNIVERSAL);
                break;

        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_professor_putricide(Creature* pCreature)
{
    return new boss_professor_putricideAI(pCreature);
}

enum HelperPhase
{
    CASTING = 0,
    FOLLOW  = 1,
    WAITING = 2,
};
/**
 * mob_icc_gas_cloud
 */
struct MANGOS_DLL_DECL mob_icc_gas_cloudAI : public ScriptedAI
{
    mob_icc_gas_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        //m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        Reset();
    }

    instance_icecrown_citadel *m_pInstance;

    HelperPhase phase;


    void Reset() override
    {
        phase = WAITING;
        DoCastSpellIfCan(m_creature, SPELL_GAS_VARIABLE_GAS, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_GASEOUS_BLOAT_VISUAL, CAST_TRIGGERED);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) override
    {
        if (!pTarget)
            return;

        if (pSpell->SpellDifficultyId == 2176)
        {
            m_creature->FixateTarget(pTarget);
            phase = FOLLOW;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_PROFESSOR_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn(2500);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (phase)
        {
            case FOLLOW:
                if (m_creature->GetDistance(m_creature->getVictim()) <= 4.0f)
                {
                    m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_EXPUNGED_GAS, true, NULL, NULL, m_creature->GetObjectGuid());
                    phase = WAITING;
                }
                break;
            case WAITING:
                if (DoCastSpellIfCan(m_creature, SPELL_GASEOUS_BLOAT) == CAST_OK)
                    phase = CASTING;
                break;
            case CASTING:
            default:
                //do nothing
                break;
        }
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_icc_gas_cloud(Creature* pCreature)
{
    return new mob_icc_gas_cloudAI(pCreature);
}

/**
 * mob_icc_volatile_ooze
 */
struct MANGOS_DLL_DECL mob_icc_volatile_oozeAI : public ScriptedAI
{
    mob_icc_volatile_oozeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        //m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;
    HelperPhase phase;

    void Reset()
    {
        phase = WAITING;
        DoCastSpellIfCan(m_creature, SPELL_OOZE_VARIABLE_OOZE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_OOZE_ADHESIVE);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell) override
    {
        if (!pTarget)
            return;

        if (pSpell->SpellDifficultyId == 2241)
        {
            m_creature->FixateTarget(pTarget);
            phase = FOLLOW;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_PROFESSOR_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn(2500);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (phase)
        {
            case FOLLOW:
                if (m_creature->GetDistance(m_creature->getVictim()) <= 4.0f)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_OOZE_ERUPTION, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                        phase = WAITING;
                }
                break;
            case WAITING:
                if (DoCastSpellIfCan(m_creature, SPELL_OOZE_ADHESIVE) == CAST_OK)
                    phase = CASTING;
                break;
            case CASTING:
            default:
                //do nothing
                break;
        }
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_icc_volatile_ooze(Creature* pCreature)
{
    return new mob_icc_volatile_oozeAI(pCreature);
}

// mob Mutated Abomination
struct MANGOS_DLL_DECL mob_mutated_amobinationAI : public ScriptedAI
{
    mob_mutated_amobinationAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    void Reset() override
    {
        m_creature->SetPower(POWER_ENERGY, 0);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->SpellDifficultyId == 2178)
        {
            m_creature->MonsterSay("I have cast Regurgitated Ooze", LANG_UNIVERSAL);
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEVE_NAUSEA, false);
        }
    }

    void JustDied(Unit* pKiller) override
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_PROFESSOR_PUTRICIDE) != DONE)
        {
            // Possibly remove GO_FLAG_NO_INTERACT when amob dies is not blizz-like
            if (GameObject* pGOTable = m_pInstance->GetSingleGameObjectFromStorage(GO_DRINK_ME_TABLE))
                pGOTable->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
        }
    }
};

CreatureAI* GetAI_mob_mutated_amobination(Creature* pCreature)
{
    return new mob_mutated_amobinationAI(pCreature);
}

// GO Drink Me!
bool GOUse_go_drink_me(Player* pPlayer, GameObject* pGameObject)
{
    pGameObject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
    pPlayer->CastSpell(pPlayer, SPELL_MUTATED_TRANSFORMATION , true, NULL, NULL, pGameObject->GetObjectGuid());
    return true;
}

void AddSC_boss_professor_putricide()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "boss_professor_putricide";
    pNewScript->GetAI = &GetAI_boss_professor_putricide;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_icc_volatile_ooze";
    pNewScript->GetAI = &GetAI_mob_icc_volatile_ooze;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_icc_gas_cloud";
    pNewScript->GetAI = &GetAI_mob_icc_gas_cloud;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_mutated_amobination";
    pNewScript->GetAI = &GetAI_mob_mutated_amobination;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_drink_me";
    pNewScript->pGOUse = &GOUse_go_drink_me;
    pNewScript->RegisterSelf();
}
