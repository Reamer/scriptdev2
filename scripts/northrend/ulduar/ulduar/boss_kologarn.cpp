/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_kologarn
SD%Complete: achievment Wenn Blicke t�ten k�nnten
SDComment: grap
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603090,
    SAY_SHOCKWEAVE      = -1603091,
    SAY_GRAB            = -1603092,
    SAY_LEFT_ARM_LOST   = -1603093,
    SAY_RIGHT_ARM_LOST  = -1603094,
    SAY_SLAY1           = -1603095,
    SAY_SLAY2           = -1603096,
    SAY_BERSERK         = -1603097,
    SAY_DEATH           = -1603098,
    EMOTE_RIGHT_ARM     = -1603099,
    EMOTE_LEFT_ARM      = -1603100,
    EMOTE_STONE_GRIP    = -1603101,

    //kologarn
    SPELL_OVERHEAD_SMASH        = 63356,
    SPELL_OVERHEAD_SMASH_H      = 64003,
    SPELL_ONE_ARMED_SMASH       = 63573,
    SPELL_ONE_ARMED_SMASH_H     = 64006,
    SPELL_STONE_SHOUT           = 63716,
    SPELL_STONE_SHOUT_H         = 64005,
    SPELL_PETRIFYING_BREATH     = 62030,
    SPELL_PETRIFYING_BREATH_H   = 63980,
    // Focused Eyebeam
    SPELL_FOCUSED_EYEBEAM       = 63342,
    SPELL_FOCUSED_EYEBEAM_DUMMY_BUFF = 67351,
    NPC_FOCUSED_EYEBEAM_RIGHT   = 33802,
    NPC_FOCUSED_EYEBEAM_LEFT    = 33632,
    SPELL_EYEBEAM_PERIODIC      = 63347,
    SPELL_EYEBEAM_PERIODIC_H    = 63977,
    SPELL_EYEBEAM_PERIODIC_VIS  = 63369,
    SPELL_EYEBEAM_SUMMON_LEFT   = 63343,
    SPELL_EYEBEAM_SUMMON_RIGHT  = 63701,
    SPELL_EYEBEAM_VISUAL_LEFT   = 63352,
    SPELL_EYEBEAM_VISUAL_LEFT_2 = 63702,
    SPELL_EYEBEAM_VISUAL_RIGHT  = 63676,

    SPELL_ARM_DEAD_DAMAGE       = 63629,
    SPELL_ARM_DEAD_DAMAGE_H     = 63979,
    SPELL_ENRAGE                = 26662,
    //left arm
    SPELL_ARM_SWEEP             = 63766,
    SPELL_ARM_SWEEP_H           = 63983,
    SPELL_SHOCKWAVE             = 63783,
    SPELL_SHOCKWAVE_H           = 63982,
    //right arm
    SPELL_STONE_GRIP_GRAB       = 62166,
    SPELL_STONE_GRIP_GRAB_H     = 63981,
    SPELL_STONE_GRIP            = 64290,
    SPELL_STONE_GRIP_H          = 64292,
    SPELL_STONE_GRIP_VEH        = 62056,
    SPELL_STONE_GRIP_VEH_H      = 63985,
    //both
    SPELL_ARM_VISUAL            = 64753,
    //rubble
    SPELL_SUMMON_RUMBLE         = 63633,
    SPELL_RUMBLE                = 63818,    // on 10 man
    SPELL_STONE_NOVA            = 63978,    // on 25 man
    //NPC ids
    MOB_RUBBLE                  = 33768,

    RIGHT_ARM_SLOT              = 1,
    LEFT_ARM_SLOT               = 0,


    ACHIEV_RUBBLE_AND_ROLL      = 2959,
    ACHIEV_RUBBLE_AND_ROLL_H    = 2960,
    ACHIEV_WITH_OPEN_ARMS       = 2951,
    ACHIEV_WITH_OPEN_ARMS_H     = 2952,
    ACHIEV_DISARMED             = 2953,
    ACHIEV_DISARMED_H           = 2954,
    ACHIEV_IF_LOOKS_COULD_KILL  = 2955,
    ACHIEV_IF_LOOKS_COULD_KILL_H= 2956,
};

// Rubble
struct MANGOS_DLL_DECL mob_ulduar_rubbleAI : public ScriptedAI
{
    mob_ulduar_rubbleAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStone_Nova_Timer;

    void Reset()
    {
        m_uiStone_Nova_Timer = urand(8000, 12000);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KOLOGARN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStone_Nova_Timer < diff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_RUMBLE : SPELL_STONE_NOVA) == CAST_OK)
                m_uiStone_Nova_Timer = urand(7000, 9000);
        }
        else
            m_uiStone_Nova_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ulduar_rubble(Creature* pCreature)
{
    return new mob_ulduar_rubbleAI(pCreature);
}

// Kologarn
struct MANGOS_DLL_DECL boss_kologarnAI : public ScriptedAI
{
    boss_kologarnAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpell_Timer;
    uint32 m_uiRange_Timer;
    uint32 m_uiEyebeam_Timer;
    uint32 m_uiRespawnRightTimer;
    uint32 m_uiRespawnLeftTimer;
    uint32 m_uiEnrageTimer;

    uint32 m_uiRubbleNo;
    uint32 m_uiDisarmedTimer;
    bool m_bOpenArms;
    bool m_bReset;

    void Reset()
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_uiSpell_Timer         = 10000;;
        m_uiRange_Timer         = 1000;
        m_uiEyebeam_Timer       = 10000 + urand(1000, 5000);
        m_uiRespawnRightTimer   = 0;
        m_uiRespawnLeftTimer    = 0;
        m_uiEnrageTimer         = 600000;;

        m_bReset            = false;

        m_uiRubbleNo        = 0;
        m_bOpenArms         = true;
        m_uiDisarmedTimer   = 0;
        if (m_creature->GetVehicleKit())
            m_creature->GetVehicleKit()->InstallAllAccessories(m_creature->GetEntry());
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, DONE);

    }

    void LeftArmDead()
    {
        DoScriptText(SAY_LEFT_ARM_LOST, m_creature);
        m_uiRespawnLeftTimer = 48000;
        m_uiRubbleNo = m_uiRubbleNo +5;
        if (m_pInstance)
        {
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_OPEN_ARMS, false);
            if (m_uiRubbleNo >= 25)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_RUBBLE_AND_ROLL, true);
            if (m_uiRespawnRightTimer > 0)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, true);
            else
                m_uiDisarmedTimer = 0;
        }
    }

    void RightArmDead()
    {
        DoScriptText(SAY_RIGHT_ARM_LOST, m_creature);
        m_uiRespawnRightTimer = 48000;
        m_bOpenArms = false;
        m_uiRubbleNo += 5;
        if (m_pInstance)
        {
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_OPEN_ARMS, false);
            if (m_uiRubbleNo >= 25)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_RUBBLE_AND_ROLL, true);
            if (m_uiRespawnLeftTimer > 0)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, true);
            else
                m_uiDisarmedTimer = 0;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);
        //aggro yell
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }


    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_ARM_DEAD_DAMAGE || pSpell->Id == SPELL_ARM_DEAD_DAMAGE_H)
        {
            switch (pCaster->GetEntry())
            {
                case NPC_RIGHT_ARM: RightArmDead(); break;
                case NPC_LEFT_ARM: LeftArmDead(); break;
                default: m_creature->MonsterSay("An unknown unit hit me with arm damage", 0); break;
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0,1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, FAIL);
    }

    void PassengerBoarded(Unit* pWho, int8 /*seatId*/, bool apply)
    {
        switch (pWho->GetEntry())
        {
            case NPC_RIGHT_ARM:
            case NPC_LEFT_ARM:
                if (!apply)
                {
                    pWho->DealDamage(pWho, pWho->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                }
                break;
            default:                            // should never appear
                m_creature->MonsterSay("Unknown Units have boarded", LANG_UNIVERSAL);
                break;
        }
    }

    void InstallRightArm()
    {
        if (m_creature->GetVehicleKit())
            m_creature->GetVehicleKit()->InstallAccessoryWithSpecificEntry(m_creature->GetEntry(), NPC_RIGHT_ARM);
        DoScriptText(EMOTE_RIGHT_ARM, m_creature);
        m_uiRespawnRightTimer = 0;
    }
    
    void InstallLeftArm()
    {
        if (m_creature->GetVehicleKit())
            m_creature->GetVehicleKit()->InstallAccessoryWithSpecificEntry(m_creature->GetEntry(), NPC_LEFT_ARM);
        DoScriptText(EMOTE_LEFT_ARM, m_creature);
        m_uiRespawnLeftTimer = 0;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (pDoneBy->GetObjectGuid() == m_creature->GetObjectGuid())
            uiDamage = 0;
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // disarmed achiev check
        if (m_uiRespawnLeftTimer > 0 || m_uiRespawnRightTimer > 0)
            m_uiDisarmedTimer += uiDiff;

        if (m_uiDisarmedTimer > 12 * IN_MILLISECONDS)
        {
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, false);
        }

        if (m_uiRange_Timer < uiDiff)
        {
            m_bReset = true;
            if (Map* pMap = m_creature->GetMap())
            {
                Map::PlayerList const& pPlayers = pMap->GetPlayers();
                if (!pPlayers.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        Player* pTarget = itr->getSource();
                        if (pTarget && pTarget->isAlive() && pTarget->IsWithinDistInMap(m_creature, 45))
                        {
                            m_bReset = false;
                            break;
                        }
                    }
                }
            }
            if (m_bReset)
                EnterEvadeMode();
            m_uiRange_Timer = 1000;
        }
        else
            m_uiRange_Timer -= uiDiff;

        if (m_uiSpell_Timer < uiDiff)
        {
            if (m_uiRespawnRightTimer == 0 && m_uiRespawnLeftTimer == 0)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OVERHEAD_SMASH : SPELL_OVERHEAD_SMASH_H) == CAST_OK)
                    m_uiSpell_Timer = 20000;
            }
            else if (m_uiRespawnRightTimer > 0 && m_uiRespawnLeftTimer > 0)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STONE_SHOUT : SPELL_STONE_SHOUT_H) == CAST_OK)
                    m_uiSpell_Timer = 2000;
            }
            else
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ONE_ARMED_SMASH : SPELL_ONE_ARMED_SMASH_H) == CAST_OK)
                    m_uiSpell_Timer = 20000;
            }
        }
        else
            m_uiSpell_Timer -= uiDiff;

        if (m_uiEyebeam_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FOCUSED_EYEBEAM) == CAST_OK)
            {
                m_uiEyebeam_Timer = 10000 + urand(1000, 5000);
            }
        }
        else
            m_uiEyebeam_Timer -= uiDiff;

        // respawn arms
        if (m_uiRespawnLeftTimer)
        {
            if (m_uiRespawnLeftTimer < uiDiff)
                InstallLeftArm();
            else
                m_uiRespawnLeftTimer -= uiDiff;
        }
        if (m_uiRespawnRightTimer)
        {
            if (m_uiRespawnRightTimer < uiDiff)
                InstallRightArm();
            else
                m_uiRespawnRightTimer -= uiDiff; 
        }
        
        //Petrifying breath
        if (m_creature->GetCombatDistance(m_creature->getVictim()) >= ATTACK_DISTANCE)
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PETRIFYING_BREATH : SPELL_PETRIFYING_BREATH_H);

        if (m_uiEnrageTimer)
        {
            if(m_uiEnrageTimer < uiDiff)
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

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

// Left Arm
struct MANGOS_DLL_DECL boss_left_armAI : public ScriptedAI
{
    boss_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShockwave;

    void Reset()
    {
        m_uiShockwave = 30000;
        DoCastSpellIfCan(m_creature, SPELL_ARM_VISUAL, CAST_TRIGGERED);
    }

    void JustDied(Unit* pKiller)
    {
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_RUMBLE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARM_DEAD_DAMAGE : SPELL_ARM_DEAD_DAMAGE_H, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShockwave < uiDiff)
        {
            if (Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARM_SWEEP : SPELL_ARM_SWEEP_H))
                {
                    DoScriptText(SAY_SHOCKWEAVE, pKologarn);
                    m_uiShockwave = 17000;
                }
            }
        }
        else
            m_uiShockwave -= uiDiff;
    }
};

CreatureAI* GetAI_boss_left_arm(Creature* pCreature)
{
    return new boss_left_armAI(pCreature);
}

// Right Arm
struct MANGOS_DLL_DECL boss_right_armAI : public ScriptedAI
{
    boss_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStoneGrip;

    void Reset()
    {
        m_uiStoneGrip           = 20000;
        DoCastSpellIfCan(m_creature, SPELL_ARM_VISUAL, CAST_TRIGGERED);
    }

    void JustDied(Unit* pKiller)
    {
        DoCastSpellIfCan(m_creature, SPELL_SUMMON_RUMBLE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARM_DEAD_DAMAGE : SPELL_ARM_DEAD_DAMAGE_H, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStoneGrip < uiDiff)
        {
            if (Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
            {
                if (Unit* pTarget = pKologarn->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, uint32(0), SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STONE_GRIP_GRAB : SPELL_STONE_GRIP_GRAB_H) == CAST_OK)
                    {
                        //DoScriptText(SAY_GRAB, pKologarn);
                        DoScriptText(EMOTE_STONE_GRIP, m_creature);
                        m_uiStoneGrip = 30000;
                    }
                }
            }
        }
        else
            m_uiStoneGrip -= uiDiff;
    }
};

CreatureAI* GetAI_boss_right_arm(Creature* pCreature)
{
    return new boss_right_armAI(pCreature);
}

// Focused Eyebeam trigger mobs - just make them not attack in melee - Move to EventAI
struct MANGOS_DLL_DECL mob_eyebeam_triggerAI : public ScriptedAI
{
    mob_eyebeam_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;

    void Reset(){}
    void UpdateAI(const uint32 uiDiff)
    {
        switch (m_creature->GetEntry())
        {
            case NPC_FOCUSED_EYEBEAM_LEFT:
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_EYEBEAM_PERIODIC : SPELL_EYEBEAM_PERIODIC_H, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
                DoCastSpellIfCan(m_creature, SPELL_EYEBEAM_VISUAL_LEFT_2, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
                break;
            }
            case NPC_FOCUSED_EYEBEAM_RIGHT:
            {
                DoCastSpellIfCan(m_creature, SPELL_EYEBEAM_PERIODIC_VIS, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
                DoCastSpellIfCan(m_creature, SPELL_EYEBEAM_VISUAL_RIGHT, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
                break;
            }
            default:
                m_creature->MonsterSay("Unknow Creature-id for Eyebeam Script", 0);
                break;
        }
        if (!m_creature->getVictim())
        {
            if (Map* pMap = m_creature->GetMap())
            {
                Map::PlayerList const& pPlayers = pMap->GetPlayers();
                if (!pPlayers.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        Player* pTarget = itr->getSource();
                        if (pTarget->HasAura(SPELL_FOCUSED_EYEBEAM_DUMMY_BUFF))
                        {
                            AttackStart(pTarget);
                        }
                    }
                }
            }
        }
    }
};

CreatureAI* GetAI_mob_eyebeam_trigger(Creature* pCreature)
{
    return new mob_eyebeam_triggerAI(pCreature);
}

// kologarn kill pit bunny - kills players that fall down into the pit. also handling bridge respawn after server restarts
struct MANGOS_DLL_DECL mob_kologarn_pit_kill_bunnyAI : public ScriptedAI
{
    mob_kologarn_pit_kill_bunnyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_fPositionZ = m_creature->GetPositionZ();
        m_bBridgeLocked = false;
    }
    instance_ulduar *m_pInstance;
    float m_fPositionZ;
    bool m_bBridgeLocked;
    void Reset(){}
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bBridgeLocked)
        {
            if (m_pInstance)
            {
                if (Creature *pKolo = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
                {
                    if (!pKolo->isAlive())
                    {  
                        if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_KOLOGARN_BRIDGE))
                        {
                            pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                            pGo->SetGoState(GO_STATE_READY);
                        }
                        if (Creature *pBridge = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN_BRIDGE_DUMMY))
                            pBridge->SetVisibility(VISIBILITY_ON);
                        m_bBridgeLocked = true;
                    }
                }
            }
        }
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER && !pWho->GetVehicle())
            if (m_creature->IsWithinLOSInMap(pWho) && pWho->GetPositionZ() - m_fPositionZ <= 15.0f)
                pWho->DealDamage(pWho, pWho->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
    }
};
CreatureAI* GetAI_mob_kologarn_pit_kill_bunny(Creature* pCreature)
{
    return new mob_kologarn_pit_kill_bunnyAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kologarn";
    NewScript->GetAI = GetAI_boss_kologarn;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_ulduar_rubble";
    NewScript->GetAI = &GetAI_mob_ulduar_rubble;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_left_arm";
    NewScript->GetAI = &GetAI_boss_left_arm;
    NewScript->RegisterSelf();
 
    NewScript = new Script;
    NewScript->Name = "boss_right_arm";
    NewScript->GetAI = &GetAI_boss_right_arm;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_eyebeam_trigger";
    NewScript->GetAI = &GetAI_mob_eyebeam_trigger;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_kologarn_pit_kill_bunny";
    NewScript->GetAI = &GetAI_mob_kologarn_pit_kill_bunny;
    NewScript->RegisterSelf();
}
