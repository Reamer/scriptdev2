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
SDName: boss_mimiron
SD%Complete: 
SDComment: TODO Achievments: Set Up Us the Bomb;Not-So-Friendly Fire
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603220,
    SAY_BERSERK         = -1603222,
    SAY_TANK_SLAY1      = -1603224,
    SAY_TANK_SLAY2      = -1603225,
    SAY_TORSO_SLAY1     = -1603228,
    SAY_TORSO_SLAY2     = -1603229,
    SAY_HEAD_SLAY1      = -1603232,
    SAY_HEAD_SLAY2      = -1603233,
    SAY_ROBOT_SLAY1     = -1603236,
    SAY_ROBOT_SLAY2     = -1603237,


    EMOTE_PLASMA_BLAST     = -1603240,
    EMOTE_LASER_SLAVE      = -1603241,

    // spells from mimiron
    SPELL_USE_VEHICLE       = 52391,

    SPELL_JET_PACK          = 63341, // used by mimiron to change seats
    SPELL_SELF_REPAIR       = 64383,

    // hard mode spells
    SPELL_SELF_DESTRUCTION  = 64613,    // visual aura
    SPELL_SELF_DESTRUCT     = 64610,    // damage aura
    SPELL_EMERGENCY_MODE_AURA   = 65101,
    NPC_MIMIRON_INFERNO     = 33370,    // used to cast the self destruct

    SPELL_FLAMES            = 64561,    // may be the fires spells
    SPELL_FLAMES_SUMMON     = 64563,    // 64567
    SPELL_FLAMES_SPREAD     = 64562,
    NPC_FLAME               = 34121,
    NPC_FLAME_INITIAL       = 34363,

    //spells
    //leviathan
    SPELL_MINE_SUMMON_10    = 63027,
    SPELL_NAPALM_SHELL      = 63666,
    SPELL_NAPALM_SHELL_H    = 65026,
    SPELL_PLASMA_BLAST      = 62997,
    SPELL_PLASMA_BLAST_H    = 64529,
    SPELL_SHOCK_BLAST       = 63631, // also in phase 4
    SPELL_FLAME_SUPRESSANT  = 64570, // hard mode
    LEVIATHAN_TURRET        = 34071,

    //vx001
    SPELL_RAPID_BURST_LEFT  = 63387,
    SPELL_RAPID_BURST_RIGHT = 64019,
    SPELL_RAPID_BURST_LEFT_H= 64531,
    SPELL_RAPID_BURST_RIGHT_H= 64532,
    SPELL_LASER_BARRAGE     = 63293, // also in phase 4
    SPELL_LASER_VISUAL      = 63300,
    SPELL_LASER_TRIGG       = 63274,
    SPELL_LASER_SLAVE       = 63414,
    SPELL_ROCKET_STRIKE     = 64064,
    SPELL_ROCKET_TRIGGER    = 63036,
    NPC_ROCKET_STRIKE       = 34047,
    SPELL_HEAT_WAVE         = 63677,
    SPELL_HEAT_WAVE_H       = 64533,
    SPELL_HAND_PULSE_LEFT   = 64348, // only in phase 4
    SPELL_HAND_PULSE_RIGHT  = 64352,
    SPELL_HAND_PULSE_LEFT_H = 64536,
    SPELL_HAND_PULSE_RIGHT_H= 64537,

    SPELL_FLAME_SUPRESS     = 65192,    // used by robot in melee range

    // frostbomb
    SPELL_FORST_BOMB        = 64623,
    SPELL_FROST_BOMB_EXPL   = 64626,
    SPELL_FROST_BOMB_AURA   = 64624,    // before explode
    SPELL_FROST_BOMB_VISUAL = 64625,    // bomb grows
    //SPELL_FROST_BOMB_SUMMON = 64627,    // summon the frostbomb

    /*aerial unit */
    SPELL_PLASMA_BALL_FLY   = 63689, // in phase 2
    SPELL_PLASMA_BALL_FLY_H = 64535, // in phase 2
    SPELL_PLASMA_BALL       = 65647, // in phase 4, i use this spell also in phase 2, because aerial unit not move
    SPELL_PLASMA_BALL_H     = 65648, // in phase 4

    // summon
    SPELL_SUMMON_SCRAP_BOT_TRIGGER_FOR_SCRAP_BOT = 63820,   // m_creature hits one 33856
    SPELL_SUMMON_SCRAP_BOT_TRIGGER_FOR_ASSAULT_BOT = 64425, // m_creature hits one 33856
    SPELL_SUMMON_FIRE_BOT_TRIGGER    = 64620,               // m_creature hits three 33856
    SPELL_BOMB_BOT_SUMMON   = 63811,

    // Bots
    NPC_ASSALT_BOT          = 34057,
    NPC_JUNK_BOT            = 33855,
    NPC_BOMB_BOT            = 33836,
    // hardmode
    MOB_FROST_BOMB          = 34149,
    MOB_EMERGENCY_FIRE_BOT  = 34147,


    SPELL_MAGNETIC_FIELD    = 64668,
    SPELL_MAGNETIC_CORE     = 64436, // increase dmg taken by 50%, used by magnetic core
    SPELL_MAGNETIC_CORE_SUMMON = 64444,
    NPC_MAGNETIC_CORE       = 34068,
    ITEM_MAGNETIC_CORE      = 46029,

    SPELL_BOMB_BOT          = 63767,


    // hard mode
    // summons fires
    SPELL_EMERGENCY_MODE    = 64582,
    SPELL_DEAFENING_SIREN   = 64616,
    SPELL_WATER_SPRAY       = 64619,

    SPELL_MIMIRONS_INFERNO  = 62910,  // maybe used by rocket
    SPELL_MIMIRONS_INFERNO2 = 62909,  // maybe hard mode
    SPELL_BERSERK           = 26662,

    // vehicle
    SEAT_FOR_ROBOT          = 3,

    ACHIEV_FIREFIGHTER      = 3180,
    ACHIEV_FIREFIGHTER_H    = 3189,
};

#define CENTER_X            2744.732f
#define CENTER_Y            2569.479f
#define CENTER_Z            364.312f 

const float PosTankHome[2]= {2794.86f, 2597.83f};
struct LocationsXY
{
    float x, y;
    uint32 id;
};

struct MANGOS_DLL_DECL boss_miniboss_mimironAI : public ScriptedAI
{
    boss_miniboss_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    instance_ulduar* m_pInstance;

    bool m_bStartAttack;
    bool m_bMustRepair;

    void Reset()
    {
        m_bStartAttack = false;
        m_bMustRepair = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = m_pInstance->GetSingleCreatureFromStorage(NPC_MIMIRON))
        {
            switch (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE))
            {
                case PHASE_MIMIRON_LEVIATHAN:
                    DoScriptText(urand(0,1) ? SAY_TANK_SLAY1 : SAY_TANK_SLAY2, pMimiron);
                    break;
                case PHASE_MIMIRON_VX001:
                    DoScriptText(urand(0,1) ? SAY_TORSO_SLAY1 : SAY_TORSO_SLAY2, pMimiron);
                    break;
                case PHASE_MIMIRON_AERIAL:
                    DoScriptText(urand(0,1) ? SAY_HEAD_SLAY1 : SAY_HEAD_SLAY2, pMimiron);
                    break;
                case PHASE_MIMIRON_ROBOT:
                    DoScriptText(urand(0,1) ? SAY_ROBOT_SLAY1 : SAY_ROBOT_SLAY2, pMimiron);
                    break;
                default: // should never appear
                    pMimiron->MonsterSay("Unknown Phase Kill Unit", LANG_UNIVERSAL);
                    break;
            }
        }
    }

    void StartThreeFireInHardmode()
    {
        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
        {
            // start again 3 fires
            for(uint8 i = 0; i < 3; i++)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTarget->InterruptNonMeleeSpells(true);
                    pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                }
            }
        }
    }

};
// Leviathan Mk script
struct MANGOS_DLL_DECL boss_leviathan_mkAI : public boss_miniboss_mimironAI
{
    boss_leviathan_mkAI(Creature* pCreature) : boss_miniboss_mimironAI(pCreature)
    {
        Reset();
    }


    uint32 m_uiMinesTimer;
    uint32 m_uiNapalmTimer;
    uint32 m_uiPlasmaBlastTimer;
    uint32 m_uiShockBlastTimer;

    bool   m_bHasSuppresed;
    uint32 m_uiSupressTimer;
    uint32 m_uiSetFireTimer;

    void Reset()
    {
        boss_miniboss_mimironAI::Reset();
        m_uiMinesTimer          = 5000;
        m_uiNapalmTimer         = 20000;
        m_uiPlasmaBlastTimer    = 10000;
        m_uiShockBlastTimer     = 30000;
        m_bHasSuppresed         = false;
        m_uiSetFireTimer        = 10000;
        m_uiSupressTimer        = 10000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if(m_pInstance)
            m_pInstance->SetDataMiniboss(TYPE_LEVIATHAN_MK, NOT_STARTED);

        if (m_creature->GetVehicleKit())
            m_creature->GetVehicleKit()->Reset();
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        boss_miniboss_mimironAI::AttackStart(pWho);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (m_pInstance)
        {
            if(m_creature->GetHealth() <= uiDamage)
            {
                uiDamage = 0;
                if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_LEVIATHAN)
                {
                    m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_TRANS_1);
                    m_pInstance->SetDataMiniboss(TYPE_LEVIATHAN_MK, DONE);

                    m_bStartAttack = false;
                    m_creature->RemoveAllAuras();
                    m_creature->DeleteThreatList();
                    m_creature->CombatStop(true);
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->SetHealth(m_creature->GetMaxHealth());
                    m_creature->GetMotionMaster()->MovePoint(0, PosTankHome[0], PosTankHome[1], CENTER_Z);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                // for feign death
                else if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_ROBOT)
                {
                    if (m_pInstance->GetDataMiniboss(TYPE_LEVIATHAN_MK) != SPECIAL)
                        m_creature->InterruptNonMeleeSpells(true);
                    m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                    m_bMustRepair   = true;
                    DoCast(m_creature, SPELL_SELF_REPAIR);
                    m_pInstance->SetDataMiniboss(TYPE_LEVIATHAN_MK, SPECIAL);
                }
            }
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SELF_REPAIR)
        {
            m_creature->MonsterSay("Self repair hit me.",0);
            m_bMustRepair = false;
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            m_uiMinesTimer      = 10000;
            m_uiShockBlastTimer = 30000;
            m_bStartAttack      = true;
            m_creature->SetInCombatWithZone();

            if(m_pInstance)
                m_pInstance->SetDataMiniboss(TYPE_LEVIATHAN_MK, IN_PROGRESS);
        }
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_INTRO)
        {
            m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_LEVIATHAN);

            m_bStartAttack = true;
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
        }
        else if (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_TRANS_3)
        {
            m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_ROBOT);

            m_bStartAttack = true;
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_creature->SetHealthPercent(50.0f);
            if(Creature* pTorso = m_pInstance->GetSingleCreatureFromStorage(NPC_VX001))
            {
                if (VehicleKitPtr pVehicleTank = m_creature->GetVehicleKit())
                {
                    pVehicleTank->RemoveAllPassengers();
                }
                pTorso->EnterVehicle(m_creature, SEAT_FOR_ROBOT);

                if(Creature* pHead = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
                {
                    pHead->EnterVehicle(pTorso, SEAT_FOR_ROBOT);
                }
                else
                    m_creature->MonsterSay("Head not found", LANG_UNIVERSAL);
            }
            else
                m_creature->MonsterSay("Torso not found.", LANG_UNIVERSAL);
        }

        StartThreeFireInHardmode();
    }

    void JustSummoned(Creature* pSummon)
    {
        pSummon->SetInCombatWithZone();
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, DEFAULT_VISIBILITY_INSTANCE);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // return if repairing
        if(m_bMustRepair)
            return;

        // proximity mines
        if(m_uiMinesTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_MINE_SUMMON_10) == CAST_OK)
                m_uiMinesTimer = 30000;
        }
        else
            m_uiMinesTimer -= uiDiff;

        // shock blast
        if(m_uiShockBlastTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHOCK_BLAST) == CAST_OK)
                m_uiShockBlastTimer = 50000;
        }
        else
            m_uiShockBlastTimer -= uiDiff;

        // hard mode script
        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
        {
            DoCastSpellIfCan(m_creature, SPELL_EMERGENCY_MODE, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

            if (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_LEVIATHAN)
            {
                if(m_creature->GetHealthPercent() < 50.0f && !m_bHasSuppresed)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FLAME_SUPRESSANT) == CAST_OK)
                    {
                        m_bHasSuppresed = true;
                        m_uiSupressTimer = 2000;
                        m_uiSetFireTimer = 10000;
                    }
                }

                if(m_uiSupressTimer < uiDiff && m_bHasSuppresed)
                {
                    SuppressFires();
                    m_uiSupressTimer = 600000;
                }
                else
                    m_uiSupressTimer -= uiDiff;

                if(m_uiSetFireTimer < uiDiff && m_bHasSuppresed)
                {
                    StartThreeFireInHardmode();
                    m_uiSetFireTimer = 600000;
                }
                else
                    m_uiSetFireTimer -= uiDiff;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// VX001 script
struct MANGOS_DLL_DECL boss_vx001AI : public boss_miniboss_mimironAI
{
    boss_vx001AI(Creature* pCreature) : boss_miniboss_mimironAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiAttackStartTimer;

    uint32 m_uiLaserBarrageTimer;
    uint32 m_uiLaserBarrageTick;
    uint32 m_uiRocketStrikeTimer;
    uint32 m_uiHeatWaveTimer;

    uint32 m_uiFlameSuppressTimer;
    uint32 m_uiFrostBombTimer;
    uint32 m_uiSpreadFiresTimer;

    uint32 m_uiStandardAttack;

    bool m_bFireLeft;

    void Reset()
    {
        boss_miniboss_mimironAI::Reset();
        SetCombatMovement(false);
        m_bFireLeft             = true;

        m_uiAttackStartTimer    = 12000;

        m_uiLaserBarrageTimer   = 60000;
        m_uiLaserBarrageTick    = 100;
        m_uiRocketStrikeTimer   = 25000;
        m_uiHeatWaveTimer       = 20000;

        m_uiStandardAttack      = 1000;

        m_uiFlameSuppressTimer  = urand(10000, 15000);
        m_uiFrostBombTimer      = urand(25000, 30000);
        m_uiSpreadFiresTimer    = urand(40000, 50000);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack && m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) != PHASE_MIMIRON_ROBOT)
            return;

        ScriptedAI::AttackStart(pWho);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if (m_pInstance)
        {
            if(m_creature->GetHealth() <= uiDamage)
            {
                uiDamage = 0;
                m_bMustRepair   = true;
                if (m_pInstance->GetDataMiniboss(TYPE_VX001) != SPECIAL)
                    m_creature->InterruptNonMeleeSpells(true);
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_VX001)
                {
                    m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_TRANS_2);
                    m_pInstance->SetDataMiniboss(TYPE_VX001, DONE);
                }
                else if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_ROBOT)
                {
                    DoCast(m_creature, SPELL_SELF_REPAIR);
                    m_pInstance->SetDataMiniboss(TYPE_VX001, SPECIAL);
                }
            }
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SELF_REPAIR)
        {
            m_bMustRepair = false;
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            m_uiLaserBarrageTimer   = 60000;
            m_uiRocketStrikeTimer   = 25000;
            m_creature->SetInCombatWithZone();

            if(m_pInstance)
                m_pInstance->SetDataMiniboss(TYPE_VX001, IN_PROGRESS);
        }
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, 10.0f);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, 10.0f);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    Creature* SelectRandomFire()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, DEFAULT_VISIBILITY_INSTANCE);

        //This should not appear!
        if (lFires.empty()){
            m_uiFrostBombTimer = 5000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lFires.begin();
        advance(iter, urand(0, lFires.size()-1));

        if((*iter)->isAlive())
            return *iter;
        else
        {
            m_uiFrostBombTimer = 500;
            return NULL;
        }
    }

    void JustSummoned(Creature* pSummend)
    {
        if (pSummend->GetEntry() == NPC_ROCKET_STRIKE)
        {
            pSummend->CastSpell(m_creature, SPELL_ROCKET_STRIKE, false);
            pSummend->ForcedDespawn(10 * IN_MILLISECONDS);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            if(GameObject* pLift = m_pInstance->GetSingleGameObjectFromStorage(GO_MIMIRON_ELEVATOR))
                pLift->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else
            m_uiAttackStartTimer -= uiDiff;

        // return if repairing
        if(m_bMustRepair)
            return;

        // turning if mob has dummy aura from Laser Barrage
        if (m_creature->HasAura(SPELL_LASER_VISUAL))
        {
            if (m_uiLaserBarrageTick < uiDiff)
            {
                // Sweep around
                float newAngle = m_creature->GetOrientation() +M_PI*0.015;
                if (newAngle > 2*M_PI_F)
                    newAngle -= 2*M_PI_F;

                m_creature->SetFacingTo(newAngle);
                m_uiLaserBarrageTick = 100;
            }
            else
                m_uiLaserBarrageTick -= uiDiff;
        }

        // no turn with SelectHostileTarget in Laser Barrage
        if (m_creature->HasAura(SPELL_LASER_SLAVE) || m_creature->HasAura(SPELL_LASER_VISUAL))
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_VX001)
        {
            if(m_uiHeatWaveTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_HEAT_WAVE : SPELL_HEAT_WAVE_H) == CAST_OK)
                    m_uiHeatWaveTimer = urand(18000,22000);
            }
            else
                m_uiHeatWaveTimer -=uiDiff;
        }

        if (m_uiLaserBarrageTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_LASER_SLAVE) == CAST_OK)
                {
                    m_creature->SetFacingTo(m_creature->GetAngle(pTarget));
                    m_uiLaserBarrageTimer = 46000;
                    return;
                }
            }
        }
        else
            m_uiLaserBarrageTimer -= uiDiff;

        if(m_uiRocketStrikeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_ROCKET_TRIGGER, SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_ROCKET_TRIGGER) == CAST_OK)
                    m_uiRocketStrikeTimer = urand(8000, 12000);
            }
        }
        else
            m_uiRocketStrikeTimer -= uiDiff;

        // hard mode
        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
        {

            DoCastSpellIfCan(m_creature, SPELL_EMERGENCY_MODE, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

            // only in VX001 phase
            if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_VX001)
            {
                if(m_uiFlameSuppressTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FLAME_SUPRESS) == CAST_OK)
                    {
                        SuppressFires();
                        m_uiFlameSuppressTimer = urand(9000, 10000);
                    }
                }
                else
                    m_uiFlameSuppressTimer -= uiDiff;
            }

            if(m_uiFrostBombTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FORST_BOMB) == CAST_OK)
                {
                    m_uiFrostBombTimer = urand(50000, 60000);
                    m_uiSpreadFiresTimer = urand(15000, 20000);
                }
            }
            else
                m_uiFrostBombTimer -= uiDiff;

            if(m_uiSpreadFiresTimer < uiDiff)
            {
                StartThreeFireInHardmode();
                m_uiSpreadFiresTimer = 60000;
            }
            else
                m_uiSpreadFiresTimer -= uiDiff;
        }

        if (m_uiStandardAttack < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_VX001)
                {
                    if (m_bFireLeft)
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_RAPID_BURST_LEFT : SPELL_RAPID_BURST_LEFT_H) == CAST_OK)
                        {
                            m_creature->SetInFront(pTarget);
                            m_creature->SetTargetGuid(pTarget->GetObjectGuid());
                            m_bFireLeft = false;
                        }
                    }
                    else
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_RAPID_BURST_RIGHT : SPELL_RAPID_BURST_RIGHT_H) ==CAST_OK)
                        {
                            m_creature->SetInFront(pTarget);
                            m_creature->SetTargetGuid(pTarget->GetObjectGuid());
                            m_bFireLeft = true;
                        }
                    }
                    m_uiStandardAttack = 500;
                }
                else if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_ROBOT)
                {
                    if (m_bFireLeft)
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HAND_PULSE_LEFT : SPELL_HAND_PULSE_LEFT_H) ==CAST_OK)
                        {
                            m_creature->SetInFront(pTarget);
                            m_creature->SetTargetGuid(pTarget->GetObjectGuid());
                            m_bFireLeft = false;
                        }
                    }
                    else
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HAND_PULSE_RIGHT : SPELL_HAND_PULSE_RIGHT_H)  ==CAST_OK)
                        {
                            m_creature->SetInFront(pTarget);
                            m_creature->SetTargetGuid(pTarget->GetObjectGuid());
                            m_bFireLeft = true;
                        }
                    }
                    m_uiStandardAttack = 1000;
                }
            }
        }
        else
            m_uiStandardAttack -= uiDiff;
    }
};

// Aerial command unit script
struct MANGOS_DLL_DECL boss_aerial_command_unitAI : public boss_miniboss_mimironAI
{
    boss_aerial_command_unitAI(Creature* pCreature) : boss_miniboss_mimironAI(pCreature)
    {
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    uint32 m_uiPlasmaBallTimer;
    uint32 m_uiSummonAssaultBotTimer;
    uint32 m_uiSummonScrapBotTimer;
    uint32 m_uiSummonFireBotTimer;
    uint32 m_uiBombBotTimer;
    uint32 m_uiGroundTimer;
    bool m_bIsGrounded;
    uint32 m_uiSpreadFiresTimer;


    bool m_bMustRepair;

    void Reset()
    {
        boss_miniboss_mimironAI::Reset();
        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 5000;
        m_uiSpreadFiresTimer    = urand(40000, 50000);

        m_uiPlasmaBallTimer     = 3000;
        m_uiSummonAssaultBotTimer = urand(20000, 30000);
        m_uiSummonScrapBotTimer = urand(10000, 15000);
        m_uiSummonFireBotTimer  = urand(30000,45000);
        m_uiBombBotTimer        = urand(14000, 17000);
        m_bIsGrounded           = false;

        m_bMustRepair           = false;

        MakeBossLand();
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance) 
            m_pInstance->SetDataMiniboss(TYPE_AERIAL_UNIT, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack && m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) != PHASE_MIMIRON_ROBOT)
            return;

        boss_miniboss_mimironAI::AttackStart(pWho);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_AERIAL)
            MakeBossFly();
    }

    void MakeBossLand()
    {
        m_creature->SetLevitate(false);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);
    }

    void MakeBossFly()
    {
        m_creature->SetLevitate(true, 10.0f);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_pInstance)
        {
            if(m_creature->GetHealth() <= uiDamage)
            {
                uiDamage = 0;
                m_bMustRepair = true;
                if (m_pInstance->GetDataMiniboss(TYPE_AERIAL_UNIT) != SPECIAL)
                    m_creature->InterruptNonMeleeSpells(true);
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_ROBOT)
                {
                    DoCast(m_creature, SPELL_SELF_REPAIR);
                    m_pInstance->SetDataMiniboss(TYPE_AERIAL_UNIT, SPECIAL);
                }
                else if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_AERIAL)
                {
                    m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_TRANS_3);
                    m_pInstance->SetDataMiniboss(TYPE_AERIAL_UNIT, DONE);
                }
            }
        }
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SELF_REPAIR)
        {
            MakeBossLand();
            m_bMustRepair = false;
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            m_uiPlasmaBallTimer = 3000;
            m_creature->SetInCombatWithZone();
            if(m_pInstance)
                m_pInstance->SetDataMiniboss(TYPE_AERIAL_UNIT, IN_PROGRESS);
        }
    }

    // get the boss down by the magnetic core
    void SetToGround()
    {
        m_creature->MonsterMoveWithSpeed(m_creature->GetPositionX(), m_creature->GetPositionY(), 364.0f, 24.0f, false, true);
        m_bIsGrounded = true;
        MakeBossLand();
        m_uiGroundTimer = 20000;
    }

    void JustSummoned(Creature* pSummon)
    {
        pSummon->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else
            m_uiAttackStartTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // return if repairing
        if(m_bMustRepair)
            return;

        // return if grounded
        if (m_creature->HasAura(SPELL_MAGNETIC_CORE))
            return;

        if (m_bIsGrounded)
        {
            m_bIsGrounded = false;
            MakeBossFly();
        }
        
        if(m_uiPlasmaBallTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BALL : SPELL_PLASMA_BALL_H) == CAST_OK)
                {
                    m_uiPlasmaBallTimer = urand(3000, 5000);
                }
            }
        }
        else
            m_uiPlasmaBallTimer -= uiDiff;

        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
        {
            DoCastSpellIfCan(m_creature, SPELL_EMERGENCY_MODE, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
        }

        // spawn adds in arena, only in phase 3
        if(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_AERIAL)
        {
            // mobs at a specific height, maybe someother work; i think best is hoverheight, but doesn't work
            if (m_creature->GetPositionZ() < 372.0f)
                m_creature->MonsterMoveWithSpeed(m_creature->GetPositionX(), m_creature->GetPositionY(), 372.0f, 24.0f, false, true);

            if (m_uiSummonAssaultBotTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_SCRAP_BOT_TRIGGER_FOR_ASSAULT_BOT) == CAST_OK)
                    m_uiSummonAssaultBotTimer = urand(20000, 30000);
            }
            else
                m_uiSummonAssaultBotTimer -= uiDiff;

            if (m_uiSummonScrapBotTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_SCRAP_BOT_TRIGGER_FOR_SCRAP_BOT) == CAST_OK)
                    m_uiSummonScrapBotTimer = urand(10000, 15000);
            }
            else
                m_uiSummonScrapBotTimer -= uiDiff;

            if (m_uiBombBotTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BOMB_BOT_SUMMON) == CAST_OK)
                    m_uiBombBotTimer = urand(14000, 17000);
            }
            else
                m_uiBombBotTimer -= uiDiff;

            if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
            {
                if (m_uiSummonFireBotTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_FIRE_BOT_TRIGGER) == CAST_OK)
                        m_uiSummonFireBotTimer = urand(30000,45000);
                }
                else
                    m_uiSummonFireBotTimer -= uiDiff;

                if(m_uiSpreadFiresTimer < uiDiff)
                {
                    StartThreeFireInHardmode();
                    m_uiSpreadFiresTimer = urand(40000, 50000);
                }
                else
                    m_uiSpreadFiresTimer -= uiDiff;
            }
        }
    }
};

// Mimiron, event controller
// boss should be placed inside the vehicles when they are supported by mangos
struct MANGOS_DLL_DECL boss_mimironAI : public ScriptedAI
{
    boss_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    instance_ulduar *m_pInstance;

    uint32 m_uiSelfDestructTimer;
    uint32 m_uiUseLiftTimer;
    uint32 m_uiPhaseDelayTimer;
    uint32 m_uiRobotDelayTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiOutroTimer;

    uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
    bool m_bIsIntro;
    bool m_bIsRobotReady;

    void Reset()
    {
        m_uiSelfDestructTimer   = 460000;  // 8 min
        m_bIsIntro              = true;
        m_uiPhaseDelayTimer     = 7000;
        m_uiUseLiftTimer        = 4000;
        m_uiBerserkTimer        = 900000;   // 15 min
        m_bIsRobotReady         = false;

        m_uiIntroTimer          = 10000;
        m_uiIntroStep           = 1;

        // reset button
        if(GameObject* pButton = m_pInstance->GetSingleGameObjectFromStorage(GO_MIMIRON_BUTTON))
            pButton->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

        // reset elevator
        if(GameObject* pLift = m_pInstance->GetSingleGameObjectFromStorage(GO_MIMIRON_ELEVATOR))
            pLift->SetGoState(GO_STATE_ACTIVE);

        // kill torso and Head
        if(Creature* pTorso = m_pInstance->GetSingleCreatureFromStorage(NPC_VX001))
            pTorso->ForcedDespawn();

        if(Creature* pHead = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
            pHead->ForcedDespawn();

        // reset tank
        if (Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
        {
            if(pTank->isAlive())
                pTank->AI()->EnterEvadeMode();
            else
                pTank->Respawn();
        }

        if(m_pInstance)
            m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_IDLE);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_MIMIRON, FAIL);
            m_pInstance->SetHardmode(TYPE_MIMIRON_HARD, FAIL);
        }
    }

    // start event
    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MIMIRON, IN_PROGRESS);
            // start intro
            if(m_pInstance->GetData(TYPE_MIMIRON) != DONE)
                m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_INTRO);
        }
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
                m_pInstance->SetHardmode(TYPE_MIMIRON_HARD, DONE);
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(m_pInstance) 
        {
            if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
                m_pInstance->SetHardmode(TYPE_MIMIRON_HARD, DONE);
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE))
        {
            case PHASE_MIMIRON_INTRO:
            case PHASE_MIMIRON_LEVIATHAN:
                break;
            case PHASE_MIMIRON_TRANS_1:
            {
                if(m_uiPhaseDelayTimer < uiDiff)
                {
                    if(/*Creature* pTorso = */m_creature->SummonCreature(NPC_VX001, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 180000))
                    {
                        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
                        {
                            for(uint8 i = 0; i < 3; i++)
                            {
                                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                {
                                    pTarget->InterruptNonMeleeSpells(true);
                                    pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                                }
                            }
                        }                        
                        m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_VX001);
                        m_uiPhaseDelayTimer = 10000;
                    }
                }
                else
                    m_uiPhaseDelayTimer -= uiDiff;

                break;
            }
            case PHASE_MIMIRON_VX001:
                break;
            case PHASE_MIMIRON_TRANS_2:
            {
                if(m_uiPhaseDelayTimer < uiDiff)
                {
                    if(/*Creature* pHead = */m_creature->SummonCreature(NPC_AERIAL_UNIT, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_DEAD_DESPAWN, 10000))
                    {
                        if(m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS)
                        {
                            // start again 3 fires
                            for(uint8 i = 0; i < 3; i++)
                            {
                                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                {
                                    pTarget->InterruptNonMeleeSpells(true);
                                    pTarget->CastSpell(pTarget, SPELL_FLAMES_SUMMON, false);
                                }
                            }

                        }
                        m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_AERIAL);
                        m_uiPhaseDelayTimer = 15000;
                    }
                }
                else
                    m_uiPhaseDelayTimer -= uiDiff;

                break;
            }
        case PHASE_MIMIRON_AERIAL:
            // Aerial Unit phase: see above script
            break;
        case PHASE_MIMIRON_TRANS_3:
            {
                if(m_uiPhaseDelayTimer < uiDiff && !m_bIsRobotReady)
                {
                    if (Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
                    {
                        if(pTank->isAlive())
                        {
                            pTank->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
                            if(Creature* pTorso = m_pInstance->GetSingleCreatureFromStorage(NPC_VX001))
                            {
                                pTorso->CastSpell(pTorso, SPELL_SELF_REPAIR, false);
                            }
                            if(Creature* pHead = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
                            {
                                pHead->CastSpell(pHead, SPELL_SELF_REPAIR, false);
                            }

                            m_bIsRobotReady = true;
                            m_uiRobotDelayTimer = 15000;
                        }
                        else
                            EnterEvadeMode();
                    }
                    m_uiPhaseDelayTimer = 100000;
                }
                else
                    m_uiPhaseDelayTimer -= uiDiff;

                if(m_uiRobotDelayTimer < uiDiff && m_bIsRobotReady)
                {

                    m_uiRobotDelayTimer = 100000;
                }
                else
                    m_uiRobotDelayTimer -= uiDiff;

                break;
            }
            case PHASE_MIMIRON_ROBOT:
            {
                if(m_pInstance->GetDataMiniboss(TYPE_LEVIATHAN_MK) == SPECIAL && m_pInstance->GetDataMiniboss(TYPE_VX001) == SPECIAL && m_pInstance->GetDataMiniboss(TYPE_AERIAL_UNIT) == SPECIAL)
                {
                    if(Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
                        m_creature->DealDamage(pTank, pTank->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pHead = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
                        m_creature->DealDamage(pHead, pHead->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pTorso = m_pInstance->GetSingleCreatureFromStorage(NPC_VX001))
                        m_creature->DealDamage(pTorso, pTorso->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    m_uiOutroTimer = 15000;
                    m_pInstance->SetGlobalPhase(TYPE_MIMIRON_PHASE, PHASE_MIMIRON_OUTRO);
                }
                break;
            }
            case PHASE_MIMIRON_OUTRO:
            {
                if(m_uiOutroTimer < uiDiff)
                {
                    DoOutro();
                    m_uiOutroTimer = 60000;
                }
                else m_uiOutroTimer -= uiDiff;

                break;
            }
            default:
                break;
        }

        // berserk
        if (m_uiBerserkTimer <= uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            if(Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
            {
                if(pTank && pTank->isAlive())
                    pTank->CastSpell(pTank, SPELL_BERSERK, false);
            }

            if(Creature* pTorso = m_pInstance->GetSingleCreatureFromStorage(NPC_VX001))
            {
                if(pTorso && pTorso->isAlive())
                    pTorso->CastSpell(pTorso, SPELL_BERSERK, false);
            }

            if(Creature* pHead = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
            {
                if(pHead && pHead->isAlive())
                    pHead->CastSpell(pHead, SPELL_BERSERK, false);
            }

            m_uiBerserkTimer = 330000;
        } 
        else
            m_uiBerserkTimer -= uiDiff;

        // self destruct platform in hard mode
        if (m_uiSelfDestructTimer < uiDiff && (m_pInstance->GetHardmode(TYPE_MIMIRON_HARD) == IN_PROGRESS))
        {
            m_creature->SummonCreature(NPC_MIMIRON_INFERNO, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
            // visual part, hacky way
            /*if(Creature* pTemp = m_creature->SummonCreature(NPC_MIMIRON_FOCUS, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
            {
                pTemp->GetMotionMaster()->MoveIdle();
                pTemp->CombatStop();
                pTemp->SetDisplayId(11686);     // make invisible
                pTemp->CastSpell(pTemp, SPELL_SELF_DESTRUCTION, false);
            }*/ 
            m_uiSelfDestructTimer = 60000;
        }
        else m_uiSelfDestructTimer -= uiDiff;
    }
};

// Leviathan MK turret
struct MANGOS_DLL_DECL leviathan_turretAI : public ScriptedAI
{
    leviathan_turretAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPlasmaBlastTimer;
    uint32 m_uiNapalmShellTimer;

    void Reset()
    {
        m_uiPlasmaBlastTimer = 20000;
        m_uiNapalmShellTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_LEVIATHAN)
        {
            if(m_uiPlasmaBlastTimer < uiDiff)
            {
                if (Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
                {
                    DoScriptText(EMOTE_PLASMA_BLAST, m_creature);
                    if (Unit* pTarget = pTank->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BLAST : SPELL_PLASMA_BLAST_H);
                }
                m_uiPlasmaBlastTimer = 30000;
            }
            else m_uiPlasmaBlastTimer -= uiDiff;

            if(m_uiNapalmShellTimer < uiDiff)
            {
                if (Creature* pTank = m_pInstance->GetSingleCreatureFromStorage(NPC_LEVIATHAN_MK))
                {
                    if (Unit* pTarget = pTank->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if(!pTank->IsWithinDistInMap(pTarget, 15))
                        {
                            DoCast(pTarget, m_bIsRegularMode ? SPELL_NAPALM_SHELL : SPELL_NAPALM_SHELL_H);
                            m_uiNapalmShellTimer = 7000;
                        }
                    }
                }
            }
            else m_uiNapalmShellTimer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL mob_bomb_botAI : public ScriptedAI
{
    mob_bomb_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiRangeCheckTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiRangeCheckTimer = 1000;
        m_uiDieTimer        = 600000;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            DoCast(m_creature, SPELL_BOMB_BOT);
            m_creature->SetHealth(m_creature->GetMaxHealth());
            uiDamage = 0;
            m_uiDieTimer = 500;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDieTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDieTimer -= uiDiff;

        if (m_uiRangeCheckTimer < uiDiff)
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 2))
            {
                DoCast(m_creature, SPELL_BOMB_BOT);
                m_uiDieTimer = 500;
                m_uiRangeCheckTimer = 5000;
            }
            else
                m_uiRangeCheckTimer = 500;
        }
        else m_uiRangeCheckTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_emergency_botAI : public ScriptedAI
{
    mob_emergency_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiWaterSprayTimer;

    void Reset()
    {
        m_uiWaterSprayTimer = urand(5000, 10000);
        if(!m_bIsRegularMode)
            DoCast(m_creature, SPELL_DEAFENING_SIREN);
    }

    void SuppressFires()
    {
        std::list<Creature*> lFires;
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34363, 15.0f);
        GetCreatureListWithEntryInGrid(lFires, m_creature, 34121, 15.0f);
        if (!lFires.empty())
        {
            for(std::list<Creature*>::iterator iter = lFires.begin(); iter != lFires.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiWaterSprayTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_WATER_SPRAY);
            SuppressFires();
            m_uiWaterSprayTimer = urand(7000, 12000);
        }
        else m_uiWaterSprayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Flames used in hard mode
struct MANGOS_DLL_DECL mob_mimiron_flamesAI : public ScriptedAI
{
    mob_mimiron_flamesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiFlamesSpreadTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_FLAMES);
        m_uiFlamesSpreadTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MIMIRON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        // spread flames
        if(m_uiFlamesSpreadTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAMES_SPREAD);
            m_uiFlamesSpreadTimer = urand(2000, 5000);
        }
        else m_uiFlamesSpreadTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_mimiron_infernoAI : public ScriptedAI
{
    mob_mimiron_infernoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiFlamesTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_SELF_DESTRUCTION);
        m_uiFlamesTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MIMIRON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(m_uiFlamesTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_SELF_DESTRUCT);
            m_uiFlamesTimer = 1000;
        }
        else m_uiFlamesTimer -= uiDiff;
    }
};

// item script, used to bring the aerial unit down
struct MANGOS_DLL_DECL mob_magnetic_coreAI : public ScriptedAI
{
    mob_magnetic_coreAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiSpellTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiSpellTimer = 2000;
        m_uiDieTimer = 23000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiSpellTimer < uiDiff)
        {
            if (m_pInstance->GetGlobalPhase(TYPE_MIMIRON_PHASE) == PHASE_MIMIRON_AERIAL)
            {
                if(Creature* pAerial = m_pInstance->GetSingleCreatureFromStorage(NPC_AERIAL_UNIT))
                {
                    DoCast(pAerial, SPELL_MAGNETIC_CORE);
                    ((boss_aerial_command_unitAI*)pAerial->AI())->SetToGround();
                }
                m_uiSpellTimer = 100000;
            }
        }
        else m_uiSpellTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff)
            m_creature->ForcedDespawn();
        else
            m_uiDieTimer -= uiDiff;
    }
};

// Red button -> used to start the hard mode
bool GOHello_go_red_button(Player* pPlayer, GameObject* pGo)
{
    instance_ulduar* m_pInstance = (instance_ulduar*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (Creature* pMimiron = m_pInstance->GetSingleCreatureFromStorage(NPC_MIMIRON))
    {
        m_pInstance->SetHardmode(TYPE_MIMIRON_HARD, IN_PROGRESS);
        m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_FIREFIGHTER, true);
        pMimiron->SetInCombatWithZone();
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT); 
        pPlayer->CastSpell(pPlayer, SPELL_FLAMES_SUMMON, false);
    }

    return false;
}

CreatureAI* GetAI_boss_mimiron(Creature* pCreature)
{
    return new boss_mimironAI(pCreature);
}

CreatureAI* GetAI_boss_leviathan_mk(Creature* pCreature)
{
    return new boss_leviathan_mkAI(pCreature);
}

CreatureAI* GetAI_boss_vx001(Creature* pCreature)
{
    return new boss_vx001AI(pCreature);
}

CreatureAI* GetAI_boss_aerial_command_unit(Creature* pCreature)
{
    return new boss_aerial_command_unitAI(pCreature);
}

CreatureAI* GetAI_leviathan_turret(Creature* pCreature)
{
    return new leviathan_turretAI(pCreature);
}

CreatureAI* GetAI_mob_bomb_bot(Creature* pCreature)
{
    return new mob_bomb_botAI(pCreature);
}

CreatureAI* GetAI_mob_emergency_bot(Creature* pCreature)
{
    return new mob_emergency_botAI(pCreature);
}

CreatureAI* GetAI_mob_mimiron_flames(Creature* pCreature)
{
    return new mob_mimiron_flamesAI(pCreature);
}

CreatureAI* GetAI_mob_mimiron_inferno(Creature* pCreature)
{
    return new mob_mimiron_infernoAI(pCreature);
}

CreatureAI* GetAI_mob_magnetic_core(Creature* pCreature)
{
    return new mob_magnetic_coreAI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mimiron";
    newscript->GetAI = &GetAI_boss_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk";
    newscript->GetAI = &GetAI_boss_leviathan_mk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "leviathan_turret";
    newscript->GetAI = &GetAI_leviathan_turret;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vx001";
    newscript->GetAI = &GetAI_boss_vx001;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_aerial_command_unit";
    newscript->GetAI = &GetAI_boss_aerial_command_unit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bomb_bot";
    newscript->GetAI = &GetAI_mob_bomb_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_emergency_bot";
    newscript->GetAI = &GetAI_mob_emergency_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mimiron_flames";
    newscript->GetAI = &GetAI_mob_mimiron_flames;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mimirons_inferno";
    newscript->GetAI = &GetAI_mob_mimiron_inferno;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_magnetic_core";
    newscript->GetAI = &GetAI_mob_magnetic_core;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_red_button";
    newscript->pGOUse = &GOHello_go_red_button;
    newscript->RegisterSelf();
}
