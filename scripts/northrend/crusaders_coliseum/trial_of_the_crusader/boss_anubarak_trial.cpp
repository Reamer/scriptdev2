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
SDName: boss_anubarak_trial
SD%Complete: 70%
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum BossSpells
{

};

enum AnubarakYells
{
    SAY_ANUB_ANUB_INTRO_1           = -1649038,
    SAY_AGGRO                       = -1649064,
    SAY_SLAY_1                      = -1649065,
    SAY_SLAY_2                      = -1649066,
    SAY_DEATH                       = -1649067,
    SAY_BERSERK                     = -1649068,
    SAY_SUBMERGE                    = -1649069,
    SAY_LEECHING_SWARM              = -1649070,
    EMOTE_BURROW                    = -1649071,
    EMOTE_PURSUE                    = -1649072,
    EMOTE_EMERGE                    = -1649073,
    EMOTE_LEECHING_SWARM            = -1649074
};

enum AnubarakSpells
{
    // Anub'arak
    SPELL_DAZE_IMMUNITY             = 53757,
    SPELL_FREEZING_SLASH            = 66012,
    SPELL_PENETRATING_COLD          = 66013,
    SPELL_SUMMON_NERUBIAN_BURROWER  = 66332, // with spell_script_target to 34862, TODO: need a limitation in Core!
    SPELL_SUMMON_SCARAB             = 66339,
    SPELL_SUBMERGE                  = 65981,
    SPELL_EMERGE                    = 65982,
    SPELL_TELEPORT_TO_SPIKE         = 66170,
    SPELL_CLEAR_ALL_DEBUFFS         = 34098,
    SPELL_SUMMON_SPIKES             = 66169,
    SPELL_LEECHING_SWARM            = 66118,
    SPELL_BERSERK                   = 26662,
};

#define MAX_FROSTSPHERES    6
#define MAX_BURROWS         4

enum Summons
{
    NPC_BURROW                      = 34862,
    NPC_FROSTSPHERE                 = 34606,
    NPC_NERUBIAN_BURROWER           = 34607,
    NPC_SCARAB                      = 34605,
    NPC_ANUBARAK_SPIKED             = 34660,
};

enum AnubarakPhases
{
    PHASE_GROUND            = 0,
    PHASE_UNDERGROUND       = 1,
    PHASE_LEECHING_SWARM    = 2
};

// I'm not sure and happy about the frostsphere spawn positions
static const float aFrostSphereSpawnPositions[MAX_FROSTSPHERES][3] =
{
    { 701.427063f, 126.473961f, 158.020538f },
    { 712.571167f, 160.9948f, 158.436768f },
    { 736.0243f, 113.420143f, 158.022583f },
    { 747.920166f, 155.09201f, 158.06131f },
    { 769.6285f, 121.102432f, 158.05043f },

    // Not official
    { 786.6439f, 108.2498f, 155.6701f }
    //{ 806.8429f, 150.5902f, 155.6701f },
    //{ 759.1386f, 163.9654f, 155.6701f },
    //{ 744.3701f, 119.5211f, 155.6701f },
    //{ 710.0211f, 120.8152f, 155.6701f },
    //{ 706.6383f, 161.5266f, 155.6701f }
};

static const float aBurrowSpawnPositions[MAX_BURROWS][4] =
{
    { 735.4028f, 75.35764f, 142.2023f, 2.094395f },
    { 692.9202f, 184.809f, 142.2026f, 5.358161f },
    { 688.2066f, 102.8472f, 142.2023f, 0.6457718f },
    { 740.5452f, 189.1129f, 142.1972f, 3.752458f }
};

struct MANGOS_DLL_DECL boss_anubarak_trialAI : public ScriptedAI
{   
    boss_anubarak_trialAI(Creature* pCreature) : ScriptedAI(pCreature)
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

    AnubarakPhases m_AnubarakPhase;

    uint32 m_PhaseSwitchTimer;
    uint32 m_uiFreezingSlashTimer;
    uint32 m_uiPenetratingColdTimer;
    uint32 m_uiNerubianBurrowerSummonTimer;
    uint32 m_uiBerserkTimer;
    bool m_bDidIntroYell;

    ObjectGuid m_PursuingSpikesGuid;
    GUIDList m_lFrostSphereGuids;
    GUIDList m_lBurrowGuids;


    void Reset() 
    {

        m_bDidIntroYell                 = false;
        m_AnubarakPhase                 = PHASE_GROUND;
        m_PhaseSwitchTimer              = 80000;
        m_uiFreezingSlashTimer          = 20000;
        m_uiPenetratingColdTimer        = urand(15000, 25000);
        m_uiNerubianBurrowerSummonTimer = 10000;
        m_uiBerserkTimer                = 480000;

        DoDespawnAdds();
    }

    void DoDespawnAdds()
    {
        for (GUIDList::const_iterator itr = m_lBurrowGuids.begin(); itr != m_lBurrowGuids.end(); ++itr)
            if (Creature* pBurrow = m_creature->GetMap()->GetCreature(*itr))
                pBurrow->ForcedDespawn();

        m_lBurrowGuids.clear();

        DoDespawnPursuingSpikes();

        for (GUIDList::const_iterator itr = m_lFrostSphereGuids.begin(); itr != m_lFrostSphereGuids.end(); ++itr)
            if (Creature* pFrostSphere = m_creature->GetMap()->GetCreature(*itr))
                pFrostSphere->ForcedDespawn();

        m_lFrostSphereGuids.clear();

        std::list<Creature*> lNerubianBurrowers;
        GetCreatureListWithEntryInGrid(lNerubianBurrowers, m_creature, NPC_NERUBIAN_BURROWER, DEFAULT_VISIBILITY_INSTANCE);

        for (std::list<Creature*>::iterator itr = lNerubianBurrowers.begin(); itr != lNerubianBurrowers.end(); ++itr)
            if ((*itr) && (*itr)->isAlive())
                (*itr)->ForcedDespawn();

        std::list<Creature*> lScarabs;
        GetCreatureListWithEntryInGrid(lScarabs, m_creature, NPC_SCARAB, DEFAULT_VISIBILITY_INSTANCE);

        for (std::list<Creature*>::iterator itr = lScarabs.begin(); itr != lScarabs.end(); ++itr)
            if ((*itr) && (*itr)->isAlive())
                (*itr)->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2,m_creature);
    }
    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_TWIN_VALKYR) == DONE && !m_bDidIntroYell && !m_creature->IsInEvadeMode() && pWho->GetTypeId() == TYPEID_PLAYER && pWho->isAlive() && !((Player*)pWho)->isGameMaster() && pWho->GetPositionZ() < m_creature->GetPositionZ() + 2.0f)
        {
            DoScriptText(SAY_ANUB_ANUB_INTRO_1, m_creature);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
            DoCastSpellIfCan(m_creature, SPELL_EMERGE);

            m_bDidIntroYell = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustReachedHome()
    {
        DoDespawnAdds();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        DoDespawnAdds();

        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        DoCastSpellIfCan(m_creature, SPELL_DAZE_IMMUNITY);

        for (uint8 i = 0; i < MAX_FROSTSPHERES; ++i)
            m_creature->SummonCreature(NPC_FROSTSPHERE, aFrostSphereSpawnPositions[i][0], aFrostSphereSpawnPositions[i][1], aFrostSphereSpawnPositions[i][2], 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);

        for (uint8 i = 0; i < MAX_BURROWS; ++i)
            m_creature->SummonCreature(NPC_BURROW, aBurrowSpawnPositions[i][0], aBurrowSpawnPositions[i][1], aBurrowSpawnPositions[i][2], aBurrowSpawnPositions[i][3], TEMPSUMMON_DEAD_DESPAWN, 0);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void JustSummoned(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case NPC_FROSTSPHERE:
                m_lFrostSphereGuids.push_back(pCreature->GetObjectGuid());
                break;
            case NPC_ANUBARAK_SPIKED:
                m_PursuingSpikesGuid = pCreature->GetObjectGuid();
                break;
            case NPC_BURROW:
                m_lBurrowGuids.push_back(pCreature->GetObjectGuid());
                break;
        }
    }

    void DoDespawnPursuingSpikes()
    {
        if (Creature* pPursuingSpikes = m_creature->GetMap()->GetCreature(m_PursuingSpikesGuid))
            pPursuingSpikes->ForcedDespawn();

        m_PursuingSpikesGuid.Clear();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (m_Phase != PHASE_UNDERGROUND)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    {
                        DoScriptText(SAY_BERSERK, m_creature);
                        m_uiBerserkTimer = 0;
                    }
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        switch (m_AnubarakPhase)
        {
            case PHASE_GROUND:
            {
                if (m_PhaseSwitchTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUBMERGE) == CAST_OK)
                    {
                        DoScriptText(SAY_SUBMERGE, m_creature);
                        DoScriptText(EMOTE_BURROW, m_creature);
                        m_PhaseSwitchTimer = 65000;
                        m_AnubarakPhase = PHASE_UNDERGROUND;
                        return;
                    }
                }
                else
                    m_PhaseSwitchTimer -= uiDiff;

                if (m_creature->GetHealthPercent() <= 30.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_LEECHING_SWARM) == CAST_OK)
                    {
                        DoScriptText(SAY_LEECHING_SWARM, m_creature);
                        DoScriptText(EMOTE_LEECHING_SWARM, m_creature);
                        m_AnubarakPhase = PHASE_LEECHING_SWARM;
                    }
                }
            }
            /* No break, because we need some spelltimers in phase3 too*/
            case PHASE_LEECHING_SWARM:
            {
                if (m_uiFreezingSlashTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FREEZING_SLASH) == CAST_OK)
                        m_uiFreezingSlashTimer = 20000;
                }
                else
                    m_uiFreezingSlashTimer -= uiDiff;

                if (m_uiPenetratingColdTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PENETRATING_COLD) == CAST_OK)
                        m_uiPenetratingColdTimer = urand(15000, 25000);
                }
                else
                    m_uiPenetratingColdTimer -= uiDiff;

                if (m_Phase == PHASE_GROUND || m_bIsHeroic)
                {
                    if (m_uiNerubianBurrowerSummonTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_NERUBIAN_BURROWER) == CAST_OK)
                            m_uiNerubianBurrowerSummonTimer = 45000;
                    }
                    else
                        m_uiNerubianBurrowerSummonTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_UNDERGROUND:
            {
                if (m_PhaseSwitchTimer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature, SPELL_TELEPORT_TO_SPIKE, CAST_TRIGGERED);
                    DoDespawnPursuingSpikes();

                    DoScriptText(EMOTE_EMERGE, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_EMERGE, CAST_TRIGGERED);
                    m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

                    if (!m_bIsHeroic)
                    {
                        for (uint8 i = 0; i < MAX_FROSTSPHERES; ++i)
                            m_creature->SummonCreature(NPC_FROSTSPHERE, aFrostSphereSpawnPositions[i][0], aFrostSphereSpawnPositions[i][1], aFrostSphereSpawnPositions[i][2], 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0);
                    }

                    m_PhaseSwitchTimer = 80000;
                    m_Phase = PHASE_GROUND;
                }
                else
                    m_PhaseSwitchTimer -= uiDiff;
                break;
            }
            default:
                m_creature->MonsterSay("Unknown Phase", LANG_UNIVERSAL);
                break;
        }
    }
};

CreatureAI* GetAI_boss_anubarak_trial(Creature* pCreature)
{
    return new boss_anubarak_trialAI(pCreature);
}

enum SwarmScarabSpells
{
    SPELL_DETERMINATION             = 66092,
    SPELL_ACID_MANDIBLE             = 65775,
    SPELL_ACHIEV_TRAITOR_KING_10    = 68186,
    SPELL_ACHIEV_TRAITOR_KING_25    = 68515,
};

struct MANGOS_DLL_DECL mob_swarm_scarabAI : public ScriptedAI
{
    mob_swarm_scarabAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Difficulty MapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIs25Man = (MapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || MapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    bool m_bIs25Man;

    uint32 m_uiAcidMandibleTimer;
    uint32 m_uiDeterminationTimer;

    void Reset()
    {
        m_uiAcidMandibleTimer    = 2000;
        m_uiDeterminationTimer   = urand(10000, 20000);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) 
            return;
    }

    void JustDied(Unit* Killer)
    {
        m_creature->CastSpell(m_creature, m_bIs25Man ? SPELL_ACHIEV_TRAITOR_KING_25 : SPELL_ACHIEV_TRAITOR_KING_10, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDeterminationTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DETERMINATION) == CAST_OK)
                m_uiDeterminationTimer = 30000;
        }
        else
            m_uiDeterminationTimer -= uiDiff;

        if (m_uiAcidMandibleTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ACID_MANDIBLE) == CAST_OK)
                m_uiAcidMandibleTimer = 2000;
        }
        else
            m_uiAcidMandibleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_swarm_scarab(Creature* pCreature)
{
    return new mob_swarm_scarabAI(pCreature);
};

enum FrostsphereSpells
{
    SPELL_PERMAFROST_VISUAL         = 65882,
    SPELL_FROSTSPHERE_INVISIBLE     = 66185,
    SPELL_PERMAFROST                = 66193,
    SPELL_FROSTSPHERE_VISUAL        = 67539,

    POINT_GROUND            = 0,
};

struct MANGOS_DLL_DECL npc_anubarak_trial_frostsphereAI : public ScriptedAI
{
    npc_anubarak_trial_frostsphereAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bPermafrost;

    void Reset()
    {
        m_bPermafrost = false;
    }

    void AttackStart(Unit* pWho) { }
    void MoveInLineOfSight(Unit* pWho) {}
    void UpdateAI(const uint32 uiDiff) { }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_creature->GetHealth() > uiDamage)
            return;

        uiDamage = 0;

        if (m_bPermafrost)
            return;

        float fZ = m_creature->GetMap()->GetHeight(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), true, MAX_FALL_DISTANCE);

        if (fZ <= INVALID_HEIGHT)
            return;

        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->GetMotionMaster()->MovePoint(POINT_GROUND, m_creature->GetPositionX(), m_creature->GetPositionY(), fZ);
        m_creature->RemoveAurasDueToSpell(SPELL_FROSTSPHERE_VISUAL);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_bPermafrost = true;
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_GROUND)
        {
            DoCastSpellIfCan(m_creature, SPELL_PERMAFROST, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_PERMAFROST_VISUAL, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_FROSTSPHERE_INVISIBLE, CAST_TRIGGERED);
        }
    }
};

CreatureAI* GetAI_mob_frost_sphere(Creature* pCreature)
{
    return new npc_anubarak_trial_frostsphereAI(pCreature);
};
enum PursuingSpikeSpells
{
    SPELL_PURSUING_SPIKES_FAIL      = 66181,
    SPELL_PURSUING_SPIKES_GROUND    = 65921, // Set as aura in database
    SPELL_PURSUING_SPIKES_SPEED1    = 65920,
    SPELL_PURSUING_SPIKES_SPEED2    = 65922,
    SPELL_PURSUING_SPIKES_SPEED3    = 65923,
    SPELL_PURSUING_SPIKES_SEARCH    = 67470,
    SPELL_MARK                      = 67574,
};

enum PursuingSpikesPhases
{
    PHASE_SEARCH            = 0,
    PHASE_NO_MOVEMENT       = 1,
    PHASE_IMPALE_NORMAL     = 2,
    PHASE_IMPALE_MIDDLE     = 3,
    PHASE_IMPALE_FAST       = 4
};

struct MANGOS_DLL_DECL npc_anubarak_trial_spikesAI : public ScriptedAI
{
    npc_anubarak_trial_spikesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    PursuingSpikesPhases m_Phase;
    uint32 m_PhaseSwitchTimer;
    uint32 m_PermaFrostCheckTimer;

    ObjectGuid m_PursuingVictimGuid;

    void Reset()
    {
        m_Phase = PHASE_SEARCH;
        m_PhaseSwitchTimer = 1000;
        m_PermaFrostCheckTimer = 1000;
        m_PursuingVictimGuid.Clear();

        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_PURSUING_SPIKES_SEARCH)
        {
            SetPursuingVictim(pTarget);
        }
        else if (pSpell->Id == SPELL_PURSUING_SPIKES_FAIL && pTarget->GetTypeId() == TYPEID_UNIT)
        {
            PermafrostHit((Creature*)pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_PhaseSwitchTimer)
        {
            if (m_PhaseSwitchTimer <= uiDiff)
            {
                switch (m_Phase)
                {
                    case PHASE_SEARCH:
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_PURSUING_SPIKES_SEARCH) == CAST_OK)
                        {
                            m_Phase = PHASE_NO_MOVEMENT;
                            m_PhaseSwitchTimer = 3000;
                        }
                        break;
                    }
                    case PHASE_NO_MOVEMENT:
                        if (DoCastSpellIfCan(m_creature, SPELL_PURSUING_SPIKES_SPEED1, CAST_TRIGGERED) == CAST_OK)
                        {
                            m_Phase = PHASE_IMPALE_NORMAL;
                            m_PhaseSwitchTimer = 7000;
                        }
                        break;
                    case PHASE_IMPALE_NORMAL:
                        if (DoCastSpellIfCan(m_creature, SPELL_PURSUING_SPIKES_SPEED2, CAST_TRIGGERED) == CAST_OK)
                        {
                            m_Phase = PHASE_IMPALE_MIDDLE;
                            m_PhaseSwitchTimer = 7000;
                        }
                        break;
                    case PHASE_IMPALE_MIDDLE:
                        if (DoCastSpellIfCan(m_creature, SPELL_PURSUING_SPIKES_SPEED3, CAST_TRIGGERED) == CAST_OK)
                        {
                            m_Phase = PHASE_IMPALE_FAST;
                            m_PhaseSwitchTimer = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
            else
                m_PhaseSwitchTimer -= uiDiff;
        }

        if (m_PermaFrostCheckTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_PURSUING_SPIKES_FAIL) == CAST_OK)
                m_PermaFrostCheckTimer = 1000;
        }
        else
            m_PermaFrostCheckTimer -= uiDiff;
    }

    void PermafrostHit(Creature* pPermafrost)
    {
        // To prevent more than one call
        if (m_Phase == PHASE_SEARCH)
            return;

        switch (m_Phase)
        {
            case PHASE_IMPALE_NORMAL:
                m_creature->RemoveAurasDueToSpell(SPELL_PURSUING_SPIKES_SPEED1);
                break;
            case PHASE_IMPALE_MIDDLE:
                m_creature->RemoveAurasDueToSpell(SPELL_PURSUING_SPIKES_SPEED2);
                break;
            case PHASE_IMPALE_FAST:
                m_creature->RemoveAurasDueToSpell(SPELL_PURSUING_SPIKES_SPEED3);
                break;
        }

        // Despawn Permafrost
        if (pPermafrost)
            pPermafrost->ForcedDespawn(3000);

        // After the spikes hit the icy surface they can't move for about ~5 seconds
        m_Phase = PHASE_SEARCH;
        m_PhaseSwitchTimer = 1000;
        m_creature->GetMotionMaster()->MoveIdle();
    }

    void SetPursuingVictim(Unit* pWho)
    {
        if (pWho && m_PursuingVictimGuid != pWho->GetObjectGuid())
        {
            DoScriptText(EMOTE_PURSUE, m_creature, pWho);
            DoCastSpellIfCan(pWho, SPELL_MARK, CAST_TRIGGERED);
            m_PursuingVictimGuid = pWho->GetObjectGuid();
            DoStartMovement(pWho);
        }
    }
};

CreatureAI* GetAI_npc_anubarak_trial_spikes(Creature* pCreature)
{
    return new npc_anubarak_trial_spikesAI(pCreature);
}

enum NerubianBorrowerSpells
{
    SPELL_SPIDER_FRENZY             = 66128,
    SPELL_SUBMERGE_BURROWER         = 68394,
    SPELL_EXPOSE_WEAKNESS           = 67720,
    SPELL_SHADOW_STRIKE             = 66134,
};
struct MANGOS_DLL_DECL mob_nerubian_borrowerAI : public ScriptedAI
{
    mob_nerubian_borrowerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsHeroic = pCreature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC
                   || pCreature->GetMap()->GetDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC;
        Reset();
    }

    bool m_bIsSubmerged;
    bool m_bIsHeroic;

    uint32 m_uiShadowStrikeTimer;
    uint32 m_uiSubmergeBurrowerTimer;

    void Reset()
    {
        m_bIsSubmerged = false;

        m_uiShadowStrikeTimer      = 5000;
        m_uiSubmergeBurrowerTimer  = 5000;
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_SPIDER_FRENZY);
        DoCastSpellIfCan(m_creature, SPELL_EXPOSE_WEAKNESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsSubmerged)
        {
            if (m_uiSubmergeBurrowerTimer < uiDiff)
            {
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_uiSubmergeBurrowerTimer = 5000;
            }
            else
                m_uiSubmergeBurrowerTimer -= uiDiff;

            if (m_creature->GetHealthPercent() > 50.0f)
            {
                m_creature->CastSpell(m_creature, SPELL_EMERGE, false);
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_BURROWER);
                m_bIsSubmerged = false;
            }
        }
        else
        {
            if (m_bIsHeroic)
            {
                if (m_uiShadowStrikeTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_STRIKE) == CAST_OK)
                            m_uiShadowStrikeTimer = 30000;
                    }
                }
                else
                    m_uiShadowStrikeTimer -= uiDiff;
            }

            if (m_creature->GetHealthPercent() < 20.0f && !m_creature->HasAuraOfDifficulty(SPELL_PERMAFROST))
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_BURROWER) == CAST_OK)
                {
                    m_bIsSubmerged = true;
                    m_uiSubmergeBurrowerTimer = 5000;
                }
            }

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_nerubian_borrower(Creature* pCreature)
{
    return new mob_nerubian_borrowerAI(pCreature);
};

void AddSC_boss_anubarak_trial()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_anubarak_trial";
    pNewScript->GetAI = &GetAI_boss_anubarak_trial;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_swarm_scarab";
    pNewScript->GetAI = &GetAI_mob_swarm_scarab;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_nerubian_borrower";
    pNewScript->GetAI = &GetAI_mob_nerubian_borrower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_anubarak_spike";
    pNewScript->GetAI = &GetAI_npc_anubarak_trial_spikes;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_frost_sphere";
    pNewScript->GetAI = &GetAI_mob_frost_sphere;
    pNewScript->RegisterSelf();
}
