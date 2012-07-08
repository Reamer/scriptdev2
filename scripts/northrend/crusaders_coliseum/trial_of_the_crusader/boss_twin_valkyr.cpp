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
SDName: trial_of_the_crusader
SD%Complete: 80%
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum BossSpells
{
    // Fjola - with Bullet Summon Timer and Instance Set functions
    SPELL_TWIN_EMPATHY_TO_DARK    = 66132,              // use spell_script_target DB entry
    SPELL_TWIN_SPIKE_LIGHT        = 66075,
    SPELL_LIGHT_SURGE             = 65766,
    SPELL_LIGHT_SHIELD            = 65858,
    SPELL_TWIN_PACT_LIGHT         = 65876,              // TODO: test the third effekt SPELL_EFFECT_SCRIPT_EFFECT
    SPELL_LIGHT_VORTEX            = 66046,
    SPELL_LIGHT_TOUCH             = 67296,

    SPELL_SUMMON_LIGHT_BULLET     = 66140,      // every 30 seconds; target= 34720; limit to 10 target (need core support) - source sniff;
    SPELL_SUMMON_DARK_BULLET      = 66140,      // every 30 seconds; target= 34704; limit to 10 target (need core support); together with above spell - source sniff

    // Eydis
    SPELL_TWIN_EMPATHY_TO_LIGHT   = 66133,              // use spell_script_target DB entry
    SPELL_TWIN_SPIKE_DARK         = 66069,
    SPELL_DARK_SURGE              = 65768,
    SPELL_DARK_SHIELD             = 65874,
    SPELL_TWIN_PACT_DARK          = 65875,              // TODO: test the third effekt SPELL_EFFECT_SCRIPT_EFFECT
    SPELL_DARK_VORTEX             = 66058,
    SPELL_DARK_TOUCH              = 67281,

    // both
    SPELL_TWIN_POWER              = 65916,

    // Essence Spells
    SPELL_LIGHT_ESSENCE           = 65686,
    SPELL_DARK_ESSENCE            = 65684,

    // Bullet Spells
    SPELL_POWERING_UP             = 67590,

    SPELL_UNLEASHED_DARK          = 65808,
    SPELL_UNLEASHED_LIGHT         = 65795,

    SPELL_LIGHT_BALL_PASSIVE      = 65794,              // triggers every 500 milliseconds 66312
    SPELL_LIGHT_BALL_PASSIVE_AREA = 66312,
    SPELL_DARK_BALL_PASSIVE       = 65796,              // triggers every 500 milliseconds 66314
    SPELL_DARK_BALL_PASSIVE_AREA  = 66314,

    // misc
    SPELL_BERSERK                 = 64238,
    SPELL_REMOVE_TOUCH            = 68084,
    SPELL_ACTIVATE_BULLET_STALKER = 68497,              // not used in sniff, but possible
    SPELL_INSTAKILL_DARK_VALKYR   = 68400,
    SPELL_INSTAKILL_LIGHT_VALKYR  = 68401,
    SPELL_VALKYR_QUICKNESS        = 65949,
};

enum Equipment
{
    EQUIP_MAIN_1                  = 49303,
    EQUIP_OFFHAND_1               = 47146,
    EQUIP_RANGED_1                = 47267,
    EQUIP_MAIN_2                  = 45990,
    EQUIP_OFFHAND_2               = 47470,
    EQUIP_RANGED_2                = 47267,
    EQUIP_DONE                    = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_UNLEASHED_DARK            = 34628,
    NPC_UNLEASHED_LIGHT           = 34630,
};

enum Yells
{
    SAY_AGGRO                       = -1649056,
    SAY_BERSERK                     = -1649057,
    SAY_COLORSWITCH                 = -1649058, // say on twin pakt
    SAY_DEATH                       = -1649059,
    SAY_SLAY_1                      = -1649060,
    SAY_SLAY_2                      = -1649061,
    SAY_TO_BLACK                    = -1649062, // say on dark vortex
    SAY_TO_WHITE                    = -1649063, // say on light vortex
};

enum ValkyrPhases
{
    PHASE_NORMAL                  = 0,
    PHASE_SPECIAL                 = 1,
};

enum ValkyrSpecials
{
    LIGHT_VORTEX                  = 0,
    LIGHT_PACT                    = 1,
    DARK_VORTEX                   = 2,
    DARK_PACT                     = 3,
};

#define    MAX_X                         = 598.80f;
#define    MIN_X                         = 527.54f;
#define    MAX_Y                         = 176.33f;
#define    MIN_Y                         = 104.54f;

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        m_uiMapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsHeroic = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_10MAN_HEROIC);
        m_bIs25Man = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;
    Difficulty m_uiMapDifficulty;
    bool m_bIsHeroic;
    bool m_bIs25Man;

    ValkyrPhases m_Phase;
    uint32 m_uiTwinSpikeLTimer;
    uint32 m_uiSpecialAbilityTimer;
    uint32 m_uiLightTouchTimer;
    uint32 m_uiOrbsSummonTimer;
    ValkyrSpecials m_NextSpecialSpell;

    void Reset() 
    {
        m_Phase                    = PHASE_NORMAL;
        m_NextSpecialSpell         = urand(0,1) ? LIGHT_VORTEX : LIGHT_PACT;
        m_uiSpecialAbilityTimer    = 45000;
        m_uiTwinSpikeLTimer        = 10000;
        m_uiLightTouchTimer        = 15000;
        m_uiOrbsSummonTimer        = 2500;

        SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_OFFHAND_1, EQUIP_RANGED_1);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TWIN_VALKYR, FAIL);
        m_creature->ForcedDespawn();
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiData == POINT_COMBAT_POSITION)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TWIN_VALKYR, DONE);
        DoCastSpellIfCan(m_creature, SPELL_INSTAKILL_DARK_VALKYR, CAST_TRIGGERED);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {

        DoScriptText(urand(0,1) ? SAY_SLAY_1 : SAY_SLAY_2 ,m_creature,pVictim);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TWIN_VALKYR, IN_PROGRESS);

        DoCastSpellIfCan(m_creature, SPELL_LIGHT_SURGE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_TWIN_EMPATHY_TO_DARK, CAST_TRIGGERED);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiOrbsSummonTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_DARK_BULLET, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_LIGHT_BULLET, CAST_TRIGGERED);
            m_uiOrbsSummonTimer = 30 * IN_MILLISECONDS;
        }
        else
            m_uiOrbsSummonTimer -= uiDiff;

        switch (m_Phase)
        {
            case PHASE_NORMAL:
                if (m_uiTwinSpikeLTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_TWIN_SPIKE_LIGHT) == CAST_OK)
                        m_uiTwinSpikeLTimer = 10000;
                }
                else
                    m_uiTwinSpikeLTimer -= uiDiff;

                if (m_bIsHeroic)
                {
                    if (m_uiLightTouchTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_LIGHT_TOUCH) == CAST_OK)
                            m_uiLightTouchTimer = 20000;
                     }
                    else
                        m_uiLightTouchTimer -= uiDiff;
                }

                if (m_uiSpecialAbilityTimer <= uiDiff)
                {
                    switch(m_NextSpecialSpell)
                    {
                        case LIGHT_VORTEX: // Vortex
                            if (DoCastSpellIfCan(m_creature, SPELL_LIGHT_VORTEX) == CAST_OK)
                            {
                                m_creature->SetLevitate(true);
                                float x, y, z;
                                m_creature->GetPosition(x, y, z);
                                m_creature->GetMotionMaster()->MovePoint(0, x, y, z + 5.0f);
                                DoScriptText(SAY_TO_WHITE, m_creature);
                                m_Phase = PHASE_SPECIAL;
                                m_NextSpecialSpell = LIGHT_PACT;
                            }
                            break;
                        case LIGHT_PACT: // Pact
                            if (DoCastSpellIfCan(m_creature, SPELL_TWIN_PACT_LIGHT) == CAST_OK)
                            {
                                //pSister->CastSpell(pSister, SPELL_TWIN_POWER, true);
                                DoCastSpellIfCan(m_creature, SPELL_LIGHT_SHIELD, CAST_TRIGGERED);
                                m_creature->SetLevitate(true);
                                float x, y, z;
                                m_creature->GetPosition(x, y, z);
                                m_creature->GetMotionMaster()->MovePoint(0, x, y, z + 5.0f);
                                DoScriptText(SAY_COLORSWITCH, m_creature);
                                m_Phase = PHASE_SPECIAL;
                                m_NextSpecialSpell = LIGHT_VORTEX;
                            }
                            break;
                        default:
                            break;
                    }
                    m_uiSpecialAbilityTimer = 90000;
                }
                else
                    m_uiSpecialAbilityTimer -= uiDiff;

                break;
            case PHASE_SPECIAL:
                if (m_creature->IsNonMeleeSpellCasted(true))
                    return;

                m_creature->SetLevitate(false);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_Phase = PHASE_NORMAL;
                m_uiSpecialAbilityTimer -= uiDiff;
                break;
            default:
                break;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fjola(Creature* pCreature)
{
    return new boss_fjolaAI(pCreature);
}

struct MANGOS_DLL_DECL boss_eydisAI : public ScriptedAI
{
    boss_eydisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        m_uiMapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsHeroic = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_10MAN_HEROIC);
        m_bIs25Man = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;
    Difficulty m_uiMapDifficulty;
    bool m_bIsHeroic;
    bool m_bIs25Man;

    ValkyrPhases m_Phase;
    uint32 m_uiTwinSpikeDTimer;
    uint32 m_uiSpecialAbilityTimer;
    uint32 m_uiDarkTouchTimer;
    ValkyrSpecials m_NextSpecialSpell;

    void Reset() 
    {
        m_Phase                    = PHASE_NORMAL;
        m_NextSpecialSpell         = urand(0,1) ? DARK_VORTEX : DARK_PACT;
        m_uiTwinSpikeDTimer        = 10000;
        m_uiSpecialAbilityTimer    = 90000;
        m_uiDarkTouchTimer         = 5000;

        SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_OFFHAND_2, EQUIP_RANGED_2);
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiData == POINT_COMBAT_POSITION)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_INSTAKILL_LIGHT_VALKYR, CAST_TRIGGERED);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText( urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2,m_creature,pVictim);
    }

    void Aggro(Unit* pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_DARK_SURGE, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_TWIN_EMPATHY_TO_LIGHT);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_Phase)
        {
            case PHASE_NORMAL:
                if (m_uiTwinSpikeDTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_TWIN_SPIKE_DARK) == CAST_OK)
                        m_uiTwinSpikeDTimer = 10000;
                }
                else
                    m_uiTwinSpikeDTimer -= uiDiff;

                if (m_bIsHeroic)
                {
                    if (m_uiDarkTouchTimer <= uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_DARK_TOUCH) == CAST_OK)
                            m_uiDarkTouchTimer = 20000;
                    }
                    else
                        m_uiDarkTouchTimer -= uiDiff;
                }

                if (m_uiSpecialAbilityTimer <= uiDiff)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    switch(m_NextSpecialSpell)
                    {
                        case DARK_VORTEX:
                            if (DoCastSpellIfCan(m_creature, SPELL_DARK_VORTEX) == CAST_OK)
                            {
                                m_creature->SetLevitate(true);
                                float x, y, z;
                                m_creature->GetPosition(x, y, z);
                                m_creature->GetMotionMaster()->MovePoint(0, x, y, z + 5.0f);
                                DoScriptText(SAY_TO_BLACK, m_creature);
                                m_Phase = PHASE_SPECIAL;
                                m_NextSpecialSpell = DARK_PACT;
                            }
                            break;
                        case DARK_PACT:
                            if (DoCastSpellIfCan(m_creature, SPELL_TWIN_PACT_DARK) == CAST_OK)
                            {
                                DoCastSpellIfCan(m_creature, SPELL_DARK_SHIELD, CAST_TRIGGERED);
//                                pSister->CastSpell(pSister, SPELL_TWIN_POWER, true);
                                m_creature->SetLevitate(true);
                                float x, y, z;
                                m_creature->GetPosition(x, y, z);
                                m_creature->GetMotionMaster()->MovePoint(0, x, y, z + 5.0f);
                                DoScriptText(SAY_COLORSWITCH, m_creature);
                                m_Phase = PHASE_SPECIAL;
                                m_NextSpecialSpell = DARK_VORTEX;
                            }
                            break;
                        default:
                            break;
                    }
                    m_uiSpecialAbilityTimer = 90000;
                }
                else
                    m_uiSpecialAbilityTimer -= uiDiff;

                break;
            case PHASE_SPECIAL:
                if (m_creature->IsNonMeleeSpellCasted(true))
                    return;

                m_creature->SetLevitate(false);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_Phase = PHASE_NORMAL;
                m_uiSpecialAbilityTimer -= uiDiff;
                break;
            default:
                break;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

struct MANGOS_DLL_DECL npc_light_or_dark_essenceAI : public ScriptedAI
{
    npc_light_or_dark_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsLightEssence;

    void Reset() 
    {
        m_bIsLightEssence = m_creature->GetEntry() == NPC_LIGHT_ESSENCE ? true : false;
        m_creature->SetWalk(true);
        SetCombatMovement(false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) 
            m_creature->ForcedDespawn();

        if (m_pInstance->GetData(TYPE_TWIN_VALKYR) != IN_PROGRESS)
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                 Unit* pPlayer = itr->getSource();
                 if (!pPlayer) continue;
                 if (pPlayer)
                 {
                     pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
                     pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
                 }
            }

            m_creature->ForcedDespawn();
        }
        return;
    }
};

CreatureAI* GetAI_npc_light_or_dark_essence(Creature* pCreature)
{
    return new npc_light_or_dark_essenceAI(pCreature);
};

bool GossipHello_npc_light_or_dark_essence(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if (!pInstance) 
        return true;

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetObjectGuid());
    if (pCreature->GetEntry() == NPC_LIGHT_ESSENCE)
    {
        pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
        pPlayer->CastSpell(pPlayer,SPELL_LIGHT_ESSENCE, true);
        pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_TOUCH);
    }
    else if (pCreature->GetEntry() == NPC_DARK_ESSENCE)
    {
        pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
        pPlayer->CastSpell(pPlayer,SPELL_DARK_ESSENCE, false);
        pPlayer->RemoveAurasDueToSpell(SPELL_DARK_TOUCH);
    }

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
};

struct MANGOS_DLL_DECL npc_light_or_dark_bulletAI : public ScriptedAI
{
    npc_light_or_dark_bulletAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_crusader*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_crusader* m_pInstance;
    bool m_bIsLightBullet;
    bool m_bHasCastUnleashedSpell;

    void Reset()
    {
        m_bIsLightBullet = m_creature->GetEntry() == NPC_UNLEASHED_LIGHT ? true : false;
        m_bHasCastUnleashedSpell = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        MoveRandom();
        DoCastSpellIfCan(m_creature, m_bIsLightBullet ? SPELL_LIGHT_BALL_PASSIVE : SPELL_DARK_BALL_PASSIVE);
        SetCombatMovement(false); 
    }

    // Move to a Point with a 50.0f distance from middle
    void MoveRandom()
    {
        float x, y,z;
        m_creature->GetRandomPoint(563.8941f, 137.3333f, m_creature->GetPositionZ(), 50.0f, x, y, z);
        m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        switch (pSpell->Id)
        {
            case SPELL_UNLEASHED_DARK:
            case SPELL_UNLEASHED_LIGHT:
            {
                if (m_bIsLightBullet ? pTarget->HasAura(SPELL_LIGHT_ESSENCE) : pTarget->HasAura(SPELL_DARK_ESSENCE))
                {
                    for (int i = 0; i < 6; ++i)
                        pTarget->CastSpell(pTarget, SPELL_POWERING_UP, true);
                }
                break;
            }
            case SPELL_LIGHT_BALL_PASSIVE_AREA:
            case SPELL_DARK_BALL_PASSIVE_AREA:
            {
                if (pTarget->isAlive() && !m_bHasCastUnleashedSpell)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsLightBullet ? SPELL_UNLEASHED_LIGHT : SPELL_UNLEASHED_DARK) == CAST_OK)
                    {
                        m_creature->ForcedDespawn(700);
                        m_bHasCastUnleashedSpell = true;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_TWIN_VALKYR) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_npc_light_or_dark_bullet(Creature *pCreature)
{
    return new npc_light_or_dark_bulletAI(pCreature);
}

void AddSC_twin_valkyr()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_fjola";
    pNewScript->GetAI = &GetAI_boss_fjola;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_eydis";
    pNewScript->GetAI = &GetAI_boss_eydis;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_light_or_dark_bullet";
    pNewScript->GetAI = &GetAI_npc_light_or_dark_bullet;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_light_or_dark_essence";
    pNewScript->GetAI = &GetAI_npc_light_or_dark_essence;
    pNewScript->pGossipHello = &GossipHello_npc_light_or_dark_essence;
    pNewScript->RegisterSelf();
}
