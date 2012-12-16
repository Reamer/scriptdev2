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
SDName: boss_deathbringer_saurfang
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

// talks
enum
{
    SAY_AGGRO                   = -1631028,
    SAY_FALLENCHAMPION          = -1631029,
    SAY_BLOODBEASTS             = -1631030,
    SAY_SLAY_1                  = -1631031,
    SAY_SLAY_2                  = -1631032,
    SAY_BERSERK                 = -1631033,
    SAY_DEATH                   = -1631034,
};

enum
{
    // Blood Power
    SPELL_BLOOD_POWER                       = 72371,
    // Blood Beasts
    SPELL_BLOOD_LINK_BEAST                  = 72176, // proc aura for Blood Beasts
    SPELL_BLOOD_LINK                        = 72202, // cast on Saurfang to give 1 Blood Power

    // Mark of the Fallen Champion
    SPELL_MARK_OF_FALLEN_CHAMPION_SEARCH    = 72254,
    SPELL_MARK_OF_FALLEN_CHAMPION           = 72256, // proc on melee hit, dmg to marked targets
    SPELL_MARK_OF_FALLEN_CHAMPION_DEBUFF    = 72293, // proc on death - heal Saurfang
    SPELL_REMOVE_MARKS                      = 72257,

    // Rune of Blood
    SPELL_RUNE_OF_BLOOD                     = 72408, // cast on self on aggro
    SPELL_RUNE_OF_BLOOD_DEBUFF              = 72410,

    // Blood Nova
    SPELL_BLOOD_NOVA                        = 72378,

    // Boiling Blood
    SPELL_BOILING_BLOOD                     = 72385,

    // Blood Beasts
    SPELL_CALL_BLOOD_BEAST_1                = 72172,
    SPELL_CALL_BLOOD_BEAST_2                = 72173,
    SPELL_CALL_BLOOD_BEAST_3                = 72356,
    SPELL_CALL_BLOOD_BEAST_4                = 72357,
    SPELL_CALL_BLOOD_BEAST_5                = 72358,

    SPELL_SCENT_OF_BLOOD                    = 72769,
    SPELL_SCENT_OF_BLOOD_TRIGGERED          = 72771, // doesn't trigger, so cast in script...
    SPELL_RESISTANT_SKIN                    = 72723,

    // enrage
    SPELL_BERSERK                           = 26662,
    SPELL_FRENZY                            = 72737,

    //summons
    NPC_BLOOD_BEAST                         = 38508
};

enum Equipment
{
    EQUIP_MAIN           = 50798,
    EQUIP_OFFHAND        = EQUIP_NO_CHANGE,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum SaurfangEvent
{
    NPC_ALLIANCE_MASON          = 37902,
};

struct Locations
{
    float x,y,z,o;
};

// positions
static Locations fSaurfangPositions[]=
{
    {-491.30f, 2211.35f, 541.11f, 3.16f}, // Deathbringer dest point
};

// passive guards AI
struct MANGOS_DLL_DECL npc_deathbringer_event_guards_iccAI : public ScriptedAI
{
    npc_deathbringer_event_guards_iccAI(Creature* pCreature) : ScriptedAI(pCreature){}

    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}
    void AttackStart(Unit *pWho){}
};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public ScriptedAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_icecrown_citadel*)pCreature->GetInstanceData();
        m_powerBloodPower = m_creature->getPowerType(); // don't call this function multiple times in script
        m_bIsIntroStarted = false;
        Reset();
    }

    instance_icecrown_citadel* m_pInstance;

    uint32 m_uiRuneOfBloodTimer;
    uint32 m_uiBoilingBloodTimer;
    uint32 m_uiBloodNovaTimer;
    uint32 m_uiBloodBeastsTimer;
    uint32 m_uiScentOfBloodTimer;
    uint32 m_uiBerserkTimer;

    bool m_bIsIntroStarted;
    bool m_bIsAlliance;

    Powers m_powerBloodPower;

    void Reset()
    {
        m_uiRuneOfBloodTimer    = 20000;
        m_uiBoilingBloodTimer   = urand(10000, 35000);
        m_uiBloodNovaTimer      = urand(16000, 35000);
        m_uiBloodBeastsTimer    = 40000;
        m_uiScentOfBloodTimer   = 47000; // 5 seconds after beasts engage in combat
        m_uiBerserkTimer        = (m_pInstance->IsHeroicDifficulty() ? 6 : 8) * MINUTE * IN_MILLISECONDS;

        m_creature->SetPower(m_powerBloodPower, 0);
    }

    void EnterEvadeMode()
    {
        // need removeAll Passengers before RemoveAllAuras, because ride aura also deleted
       if (m_creature->GetVehicleKit())
           m_creature->GetVehicleKit()->RemoveAllPassengers();
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive())
            m_creature->GetMotionMaster()->MovePoint(1, fSaurfangPositions[0].x, fSaurfangPositions[0].y, fSaurfangPositions[0].z);

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (!m_bIsIntroStarted && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster() && m_creature->GetDistance2d(pWho) < 50.0f)
        {
            if (m_pInstance)
                m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, SPECIAL);
            m_bIsIntroStarted = true;
        }

        // Intro ends with "SetData(TYPE_DEATHBRINGER_SAURFANG, NOT_STARTED);"
        if (m_pInstance->GetData(TYPE_DEATHBRINGER_SAURFANG) != SPECIAL)
            ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        DoCastSpellIfCan(m_creature, SPELL_BLOOD_POWER, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_RUNE_OF_BLOOD, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_MARK_OF_FALLEN_CHAMPION, CAST_TRIGGERED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, FAIL);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
        m_creature->SetFacingTo(fSaurfangPositions[0].o);
    }

    // used for unlocking bugged encounter
    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHBRINGER_SAURFANG, DONE);

        DoScriptText(SAY_DEATH, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_REMOVE_MARKS, CAST_TRIGGERED);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_SLAY_1 - urand(0,1), m_creature, pVictim);
    }

    void JustSummoned(Creature *pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_BLOOD_BEAST)
        {
            pSummoned->CastSpell(pSummoned, SPELL_BLOOD_LINK_BEAST, true);
            pSummoned->CastSpell(pSummoned, SPELL_RESISTANT_SKIN, true);
//            pSummoned->setFaction(m_creature->getFaction());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Mark of the Fallen Champion
        if (m_creature->GetPower(m_powerBloodPower) >= 100)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_MARK_OF_FALLEN_CHAMPION_SEARCH) == CAST_OK)
            {
                m_creature->SetPower(m_powerBloodPower, 0); // reset Blood Power
                // decrease the buff
                m_creature->RemoveAurasDueToSpell(SPELL_BLOOD_POWER);
                int32 power = m_creature->GetPower(m_powerBloodPower);
                m_creature->CastCustomSpell(m_creature, SPELL_BLOOD_POWER, &power, &power, NULL, true);
                DoScriptText(SAY_FALLENCHAMPION, m_creature);
            }
        }

        // Frenzy (soft enrage)
        if (m_creature->GetHealthPercent() <= 30.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FRENZY, CAST_AURA_NOT_PRESENT) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
            }
        }

        // Berserk (hard enrage)
        if (m_uiBerserkTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                m_uiBerserkTimer = (m_pInstance->IsHeroicDifficulty() ? 6 : 8) * MINUTE * IN_MILLISECONDS;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;

        // Rune of Blood
        if (m_uiRuneOfBloodTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_DEBUFF) == CAST_OK)
                m_uiRuneOfBloodTimer = 20000;
        }
        else
            m_uiRuneOfBloodTimer -= uiDiff;

        // Boiling Blood
        if (m_uiBoilingBloodTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BOILING_BLOOD) == CAST_OK)
                m_uiBoilingBloodTimer = urand(10000, 35000);
        }
        else
            m_uiBoilingBloodTimer -= uiDiff;

        // Blood Nova
        if (m_uiBloodNovaTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_BLOOD_NOVA, SELECT_FLAG_NOT_IN_MELEE_RANGE ))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_BLOOD_NOVA) == CAST_OK)
                    m_uiBloodNovaTimer = urand(16000, 35000);
            }
        }
        else
            m_uiBloodNovaTimer -= uiDiff;

        // Call Blood Beasts
        if (m_uiBloodBeastsTimer <= uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_1, true);
            m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_2, true);

            if (m_pInstance->Is25ManDifficulty())
            {
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_3, true);
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_4, true);
                m_creature->CastSpell(m_creature, SPELL_CALL_BLOOD_BEAST_5, true);
            }

            m_uiBloodBeastsTimer  = 40000;

            if (m_pInstance->IsHeroicDifficulty())
                m_uiScentOfBloodTimer = 7000; // 5 seconds after beasts engage in combat

            DoScriptText(SAY_BLOODBEASTS, m_creature);
        }
        else
            m_uiBloodBeastsTimer -= uiDiff;

        // Scent of Blood
        if (m_pInstance->IsHeroicDifficulty())
        {
            if (m_uiScentOfBloodTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SCENT_OF_BLOOD, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_SCENT_OF_BLOOD_TRIGGERED, CAST_TRIGGERED);
                m_uiScentOfBloodTimer = 40000;
                // via DB
                m_creature->MonsterTextEmote("Deathbringer Saurfang's Blood Beasts gain the scent of blood!", m_creature->getVictim(), true);
            }
            else
                m_uiScentOfBloodTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_deathbringer_saurfang(Creature* pCreature)
{
    return new boss_deathbringer_saurfangAI(pCreature);
}

CreatureAI* GetAI_npc_deathbringer_event_guards_iccAI(Creature* pCreature)
{
    return new npc_deathbringer_event_guards_iccAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_blood_beastAI : public ScriptedAI
{
    mob_blood_beastAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiScentOfBloodTimer;
    uint32 m_uiReadyTimer;

    bool m_bIsReady;

    void Reset()
    {
        m_uiScentOfBloodTimer = 5000;

        m_uiReadyTimer = 2000;
        m_bIsReady = false;

        SetCombatMovement(false);
        m_creature->SetWalk(true);
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsReady)
        {
            if (m_uiReadyTimer <= uiDiff)
            {
                m_bIsReady = true;
                SetCombatMovement(true);
                m_creature->SetInCombatWithZone();
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else
                m_uiReadyTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blood_beast(Creature* pCreature)
{
    return new mob_blood_beastAI(pCreature);
}

void AddSC_boss_deathbringer_saurfang()
{
    Script* pNewscript;
    pNewscript = new Script;
    pNewscript->Name = "boss_deathbringer_saurfang";
    pNewscript->GetAI = &GetAI_boss_deathbringer_saurfang;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_deathbringer_event_guards_iccAI";
    pNewscript->GetAI = &GetAI_npc_deathbringer_event_guards_iccAI;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_blood_beast";
    pNewscript->GetAI = &GetAI_mob_blood_beast;
    pNewscript->RegisterSelf();
}
