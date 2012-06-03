/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 - 2012 MangosR2 <http://github.com/mangosR2/>
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
SDName: instance_culling_of_stratholme
SD%Complete: %
SDComment:
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"
#include "escort_ai.h"
#include "WorldPacket.h"
#include "Weather.h"

/*###
## npc_arthas
###*/

enum
{
    SAY_INTRO01                           = -1594071,    //Arthas
    SAY_INTRO02                           = -1594072,    //Uther
    SAY_INTRO03                           = -1594073,    //Arthas
    SAY_INTRO04                           = -1594074,    //Arthas
    SAY_INTRO05                           = -1594075,    //Uther
    SAY_INTRO06                           = -1594076,    //Arthas
    SAY_INTRO07                           = -1594077,    //Uther
    SAY_INTRO08                           = -1594078,    //Arthas
    SAY_INTRO09                           = -1594079,    //Arthas
    SAY_INTRO10                           = -1594080,    //Uther
    SAY_INTRO11                           = -1594081,    //Arthas
    SAY_INTRO12                           = -1594082,    //Uther
    SAY_INTRO13                           = -1594083,    //Jaina
    SAY_INTRO14                           = -1594084,    //Arthas
    SAY_INTRO15                           = -1594085,    //Uther
    SAY_INTRO16                           = -1594086,    //Arthas
    SAY_INTRO17                           = -1594087,    //Jaina
    SAY_INTRO18                           = -1594088,    //Arthas

    SAY_ENTER01                           = -1594089,    //Arthas
    SAY_ENTER02                           = -1594090,    //Cityman
    SAY_ENTER03                           = -1594091,    //Arthas
    SAY_ENTER04                           = -1594092,    //Crazyman
    SAY_ENTER05                           = -1594093,    //Crazyman2
    SAY_ENTER06                           = -1594094,    //Arthas
    SAY_ENTER07                           = -1594095,    //Malganis
    SAY_ENTER08                           = -1594096,    //Malganis
    SAY_ENTER09                           = -1594097,    //Arthas
    SAY_ENTER10                           = -1594098,    //Arthas

    SAY_SALRAMM_SPAWN                     = -1594129,
    SAY_MEATHOOK_SPAWN                    = -1594110,

    SAY_PHASE501                          = -1594142,    //Arthas
    SAY_PHASE502                          = -1594143,    //Arthas
    SAY_PHASE503                          = -1594144,    //Human
    SAY_PHASE504                          = -1594145,    //Arthas
    SAY_PHASE505                          = -1594146,    //Arthas
    SAY_PHASE506                          = -1594147,    //Human
    SAY_PHASE507                          = -1594148,    //Arthas
    SAY_PHASE508                          = -1594149,    //Arthas
    SAY_PHASE509                          = -1594150,    //Arthas
    SAY_PHASE510                          = -1594151,    //Arthas
    SAY_PHASE511                          = -1594152,    //Arthas
    SAY_PHASE512                          = -1594153,    //Arthas
    SAY_PHASE513                          = -1594154,    //Arthas

    SAY_EPOCH_INTRO                       = -1594155,
    SAY_ARTHAS_INTRO                      = -1594156,
    SAY_EPOCH_AGGRO                       = -1594157,

    SAY_PHASE514                        = -1594158,   //Arthas Shkaf 01
    SAY_PHASE515                        = -1594159,   //Arthas Shkaf 02
    SAY_PHASE601                        = -1594160,  //Arthas Fire
    SAY_PHASE602                        = -1594161,  //Arthas Picnic
    SAY_PHASE603                        = -1594162,  //Arthas Picnic End
    SAY_PHASE605                        = -1594164,  //Arthas mall start
    SAY_PHASE606                        = -1594188,

    SAY_MALGANIS_ESCAPE02    = -1594180,
    SAY_ARTHAS_OUTRO01       = -1594181,
    SAY_MALGANIS_OUTRO       = -1594182,
    SAY_ARTHAS_OUTRO02       = -1594183,
    SAY_ARTHAS_OUTRO03       = -1594184,
    SAY_MALGANIS_ESCAPE01    = -1594187,

    /*SPELL*/
    SPELL_EXORCISM_N          = 52445,
    SPELL_EXORCISM_H          = 58822,
    SPELL_HOLY_LIGHT          = 52444,
    SPELL_ARTHAS_AURA         = 52442,

///-> marines
    SPELL_HEROIC_STRIKE  = 25710,
    SPELL_DEFEND         = 52317,
///->priest
    SPELL_SMITE         = 61923,
    SPELL_HEAL          = 62442,

    /*OTHER*/
    POINT_LAST_POINT    = 0xFFFFFF,
    FACTION             = 2076
};

const float SummonScourge[2][4] =
{
    {2340.058f, 1253.570f, 132.733f, 5.09f}, //right wing
    {2272.773f, 1331.824f, 124.171f, 3.12f}, //left wing
};

struct MANGOS_DLL_DECL npc_arthasAI : public npc_escortAI
{
    npc_arthasAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetActiveObjectState(true);
        m_bStartEvent = false;
        ResetInstanceVars();
        Reset();
    }

    instance_culling_of_stratholme* m_pInstance;
    bool m_bIsRegularMode;

    ObjectGuid m_uiHuman01GUID;
    ObjectGuid m_uiHuman02GUID;
    ObjectGuid m_uiHuman03GUID;
    ObjectGuid m_EnterEventPersonOne;
    ObjectGuid m_EnterEventPersonTwo;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiHealTimer;
    uint32 m_uiExorcismTimer;
    uint32 m_uiSummonZombie;
    uint32 m_uiStrongerNPC;

    bool m_bStartEvent;
    bool m_bTalkWithUther;
    bool m_bStartEnterEvent;
    bool m_bInHouseEvent;
    bool m_bInEpocEvent;
    bool m_bStartEscort;

    void Reset()
    {
        m_uiHealTimer       = 20 * IN_MILLISECONDS;
        m_uiExorcismTimer   = 7300;

        if (!m_pInstance) return;

        m_creature->SetSpeedRate(MOVE_RUN, 1);

        if (m_pInstance->GetInstancePosition() == POS_ARTHAS_INTRO)
        {
            RemoveGossip();
        }

        if (m_pInstance->GetInstancePosition() == POS_INSTANCE_FINISHED)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
        }
    }

    void JustRespawned()
    {
        npc_escortAI::JustRespawned();
        ResetInstanceVars();
    }

    void ResetInstanceVars()
    {
        m_bTalkWithUther    = false;
        m_bStartEnterEvent  = false;
        m_bInHouseEvent     = false;
        m_bInEpocEvent      = false;
        m_bStartEscort      = false;
        m_uiSummonZombie    = 2 * IN_MILLISECONDS;
        m_uiStrongerNPC     = 10 * IN_MILLISECONDS;
        m_uiStep            = 0;
        m_uiStepTimer       = 100;
    }

    void RemoveGossip()
    {
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void SetGossip()
    {
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void EnableEscort()
    {
        SetEscortPaused(false);
    }

    void SummonPeople()
    {
        if (Creature* pCityman = m_creature->SummonCreature(NPC_STRATHOLME_CITIZEN,2091.977f,1275.021f,140.757f,0.558f,TEMPSUMMON_DEAD_DESPAWN,30000))
            m_EnterEventPersonOne = pCityman->GetObjectGuid();
        if (Creature* pCrazyman = m_creature->SummonCreature(NPC_STRATHOLME_RESIDENT,2093.514f,1275.842f,140.408f,3.801f,TEMPSUMMON_DEAD_DESPAWN,30000))
            m_EnterEventPersonTwo = pCrazyman->GetObjectGuid();
    }

    void SummonHousePeople()
    {
        if (Creature* pHuman01 = m_creature->SummonCreature(NPC_STRATHOLME_RESIDENT_HOUSE,2397.308f,1207.565f,134.038f,5.593f,TEMPSUMMON_DEAD_DESPAWN,30000))
            m_uiHuman01GUID = pHuman01->GetObjectGuid();
        if (Creature* pHuman02 = m_creature->SummonCreature(NPC_STRATHOLME_RESIDENT_HOUSE,2400.770f,1207.362f,134.038f,3.454f,TEMPSUMMON_DEAD_DESPAWN,30000))
            m_uiHuman02GUID = pHuman02->GetObjectGuid();
        if (Creature* pHuman03 = m_creature->SummonCreature(NPC_STRATHOLME_RESIDENT_HOUSE,2400.547f,1204.892f,134.038f,2.479f,TEMPSUMMON_DEAD_DESPAWN,30000))
            m_uiHuman03GUID = pHuman03->GetObjectGuid();
    }

    void StartAI()
    {
         m_uiStep           = 0;
         m_uiStepTimer      = 100;
         m_bStartEvent      = true;
         m_bTalkWithUther   = true;
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_SALRAMM_THE_FLESHCRAFTER)
        {
            EnableEscort();
            SetRun(true);
        }
    }

    void Aggro(Unit* who)
    {
        DoCast(m_creature, SPELL_ARTHAS_AURA);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 2:                                         // in Front of Bridge
                DoScriptText(SAY_INTRO18, m_creature);
                SetRun(true);
                break;
            case 8:                                         // Entrance to Town; Wait for ready talk from player
                SummonPeople();
                SetSpawnPoint(8);
                SetEscortPaused(true);
                SetGossip();
                SetRun(false);
                break;
            case 9:                                         // Entrance to Town
                DoScriptText(SAY_ENTER01, m_creature);
                break;
            case 10:                                        // Start Position for EnterEvent
                m_bStartEnterEvent = true;
                SetEscortPaused(true);
                ResetStep(2000);
                if (Unit* pCityman = m_pInstance->instance->GetCreature(m_EnterEventPersonOne))
                {
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pCityman->GetObjectGuid());
                    pCityman->SetGuidValue(UNIT_FIELD_TARGET, m_creature->GetObjectGuid());
                    pCityman->GetMotionMaster()->MovePoint(0, 2088.625f,1279.191f,140.743f);
                }
                break;
            case 14:                                        // Some Steps into Town, Summon City NPC's for HouseEvent
                DoScriptText(SAY_PHASE501, m_creature);
                break;
            case 20:                                        // Start Position for Entering House; Wait for ready talk from player
                SetSpawnPoint(20);
                SummonHousePeople();
                SetEscortPaused(true);
                SetGossip();
                SetRun(false);
                break;
            case 21:                                        // Some Steps away from  Start Position for Entering House after talk to Player
                DoScriptText(SAY_PHASE502, m_creature);
                break;
            case 22:                                        // Start Position for House Event
                m_bInHouseEvent = true;
                SetEscortPaused(true);
                ResetStep(1000);
                break;
            case 25:                                        // After the stair
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                m_creature->SummonCreature(NPC_TIME_RIFT,2428.901f, 1192.164f, 148.076f, 5.09f,TEMPSUMMON_TIMED_DESPAWN,10000);
                DoScriptText(SAY_PHASE508, m_creature);
                break;
            case 26:                                        // After the stair again
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                DoScriptText(SAY_PHASE509, m_creature);
                break;
            case 29:                                        // in Trap
                m_creature->SummonCreature(NPC_TIME_RIFT,2413.773f, 1137.820f, 148.076f, 5.09f,TEMPSUMMON_TIMED_DESPAWN,10000);
                m_creature->SummonCreature(NPC_TIME_RIFT,2404.990f, 1175.121f, 148.076f, 5.09f,TEMPSUMMON_TIMED_DESPAWN,10000);
                DoScriptText(SAY_PHASE510, m_creature);
                break;
            case 30:                                        // Near to Epoch
                DoScriptText(SAY_PHASE513, m_creature);
                break;
            case 31:                                        // Near to Epoch; Start Epoch Event - If this waypoint reached a big pause (29 seconds - for Epoch Event)
                ResetStep(1000);
                m_bInEpocEvent = true;
                break;
            case 32:                                        // After Epoch fight; Wait for ready talk from player
                SetSpawnPoint(32);
                SetEscortPaused(true);
                SetGossip();
                SetRun(false);
                break;
            case 36:                                        // Befor bookshelf
                DoScriptText(SAY_PHASE514, m_creature);
                break;
            case 37:                                        // Open bookshelf
                if (GameObject* pGate = m_pInstance->GetSingleGameObjectFromStorage(GO_DOOR_BOOKCASE))
                    pGate->SetGoState(GO_STATE_ACTIVE);
                SetRun(true);
                DoScriptText(SAY_PHASE515, m_creature);
                break;
            case 45:                                        // After House at begin of burning stratholm
                m_bStartEscort = true;
                DoScriptText(SAY_PHASE601, m_creature);
                break;
            case 48:                                        // In middle of burning stratholm
                DoScriptText(SAY_PHASE602, m_creature);
                m_uiSummonZombie = 20000;
                m_uiStrongerNPC  = 20000;
                break;
            case 51:                                        // Befor Malganis
                SetSpawnPoint(51);
                ResetStep(1000);
                SetEscortPaused(true);
                m_pInstance->SetData(TYPE_ARTHAS_ESCORT_EVENT, DONE);
                DoScriptText(SAY_PHASE606, m_creature);
                SetGossip();
                break;
            case 53:
                SetEscortPaused(true);
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->MovementExpired(false);
                DoScriptText(SAY_PHASE605, m_creature);
                if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                {
                    m_creature->AI()->AttackStart(pMalganis);
                    pMalganis->AI()->AttackStart(m_creature);
                }
                break;
            default:
                break;
        }
    }

    void JumpNextStep(uint32 Timer)
    {
        m_uiStepTimer = Timer;
        m_uiStep++;
    }

    void ResetStep(uint32 Timer)
    {
        m_uiStep = 0;
        m_uiStepTimer = Timer;
    }

    void IntroEvent()
    {
        switch(m_uiStep)
        {
            case 0:
                DoScriptText(SAY_INTRO01, m_creature);
                JumpNextStep(2000);
                break;
            case 1:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                DoScriptText(SAY_INTRO02, pUther);
                JumpNextStep(8000);
                break;
            case 2:
                m_creature->SetWalk(true);
                DoScriptText(SAY_INTRO03, m_creature);
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                m_creature->GetMotionMaster()->MovePoint(0, 1907.313f, 1310.917f, 149.551f);
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    pUther->GetMotionMaster()->MovePoint(0, 1903.600f, 1296.678f, 143.383f);
                JumpNextStep(2000);
                break;
            case 3:
                if (Creature* pJaina = m_pInstance->GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE))
                    pJaina->GetMotionMaster()->MovePoint(0, 1899.641f, 1298.684f, 143.831f);
                JumpNextStep(7000);
                break;
            case 4:
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MovePoint(0, 1911.087f, 1314.263f, 150.026f);
                JumpNextStep(1000);
                break;
            case 5:
                DoScriptText(SAY_INTRO04, m_creature);
                JumpNextStep(10000);
                break;
            case 6:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    DoScriptText(SAY_INTRO05, pUther);
                JumpNextStep(1000);
                break;
            case 7:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pUther->GetObjectGuid());
                DoScriptText(SAY_INTRO06, m_creature);
                JumpNextStep(4000);
                break;
            case 8:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    DoScriptText(SAY_INTRO07, pUther);
                JumpNextStep(6000);
                break;
            case 9:
                DoScriptText(SAY_INTRO08, m_creature);
                JumpNextStep(4000);
                break;
            case 10:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    DoScriptText(SAY_INTRO09, pUther);
                JumpNextStep(8000);
                break;
            case 11:
                DoScriptText(SAY_INTRO10, m_creature);
                JumpNextStep(4000);
                break;
            case 12:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    DoScriptText(SAY_INTRO11, pUther);
                JumpNextStep(4000);
                break;
            case 13:
               DoScriptText(SAY_INTRO12, m_creature);
               JumpNextStep(11000);
               break;
            case 14:
                if (Creature* pJaina = m_pInstance->GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE))
                    DoScriptText(SAY_INTRO13, pJaina);
                JumpNextStep(3000);
                break;
            case 15:
                DoScriptText(SAY_INTRO14, m_creature);
                JumpNextStep(9000);
                break;
            case 16:
                if (Creature* pUther = m_pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    DoScriptText(SAY_INTRO15, pUther);
                JumpNextStep(5000);
                break;
            case 17:
                if (Creature* pJaina = m_pInstance->GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE))
                {
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pJaina->GetObjectGuid());
                    pJaina->GetMotionMaster()->MovePoint(0, 1794.357f,1272.183f,140.558f);
                 }
                 JumpNextStep(1000);
                 break;
            case 18:
                DoScriptText(SAY_INTRO16, m_creature);
                JumpNextStep(1000);
                break;
            case 19:
                if (Creature* pJaina = m_pInstance->GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE))
                    DoScriptText(SAY_INTRO17, pJaina);
                JumpNextStep(3000);
                break;
            case 20:
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                ((npc_arthasAI*)m_creature->AI())->Start(false);
                JumpNextStep(3000);
                m_bTalkWithUther = false;
                break;
        }
    }

    void EnterEvent()
    {
        switch(m_uiStep)
        {
            case 0:
                if (Unit* pCityman = m_pInstance->instance->GetCreature(m_EnterEventPersonOne))
                DoScriptText(SAY_ENTER02, pCityman);
                JumpNextStep(4000);
                break;
            case 1:
                m_creature->GetMotionMaster()->MovePoint(0, 2087.689f,1280.344f,140.73f);
                DoScriptText(SAY_ENTER03, m_creature);
                JumpNextStep(3000);
                break;
            case 2:
                if (Unit* pCityman = m_pInstance->instance->GetCreature(m_EnterEventPersonOne))
                   DoScriptText(SAY_ENTER04, pCityman);
                m_creature->HandleEmoteCommand(37);
                JumpNextStep(1000);
                break;
            case 3:
                if (Unit* pCityman = m_pInstance->instance->GetCreature(m_EnterEventPersonOne))
                    pCityman->DealDamage(pCityman, pCityman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                if (Unit* pCrazyman = m_pInstance->instance->GetCreature(m_EnterEventPersonTwo))
                {
                    DoScriptText(SAY_ENTER05, pCrazyman);
                    pCrazyman->SetGuidValue(UNIT_FIELD_TARGET, m_creature->GetObjectGuid());
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pCrazyman->GetObjectGuid());
                    m_creature->GetMotionMaster()->MovePoint(0, 2092.154f,1276.645f,140.52f);
                }
                JumpNextStep(3000);
                break;
            case 4:
                m_creature->HandleEmoteCommand(37);
                JumpNextStep(1000);
                break;
            case 5:
                if (Unit* pCrazyman = m_pInstance->instance->GetCreature(m_EnterEventPersonTwo))
                   pCrazyman->DealDamage(pCrazyman, pCrazyman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                JumpNextStep(1000);
                break;
            case 6:
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                m_creature->GetMotionMaster()->MovePoint(0, 2091.179f,1278.065f,140.476f);
                DoScriptText(SAY_ENTER06, m_creature);
                JumpNextStep(3000);
                break;
            case 7:
                if (Creature* pStalkerM = m_pInstance->GetSingleCreatureFromStorage(NPC_INVIS_STALKER))
                    pStalkerM->CastSpell(pStalkerM,63793,false);
                JumpNextStep(1000);
                break;
            case 8:
                m_pInstance->TeleportMalganisAndSpawnIfNeeded(true);
                //DoScriptText(SAY_ENTER07, pTempMalganis); DONE in ACID
                JumpNextStep(11000);
                break;
            case 9:
                //if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                    //DoScriptText(SAY_ENTER08, pMalganis); // Done in ACID
                JumpNextStep(17000);
                break;
            case 10:
                if (Creature* pStalkerM = m_pInstance->GetSingleCreatureFromStorage(NPC_INVIS_STALKER))
                {
                    if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                    {
                        pStalkerM->CastSpell(pStalkerM,63793,false);
                        pMalganis->SetVisibility(VISIBILITY_OFF);
                    }
                }
                m_pInstance->ConvertCityToDeath();
                DoScriptText(SAY_ENTER09, m_creature);
                JumpNextStep(7000);
                break;
            case 11:
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                DoScriptText(SAY_ENTER10, m_creature);
                JumpNextStep(12000);
                break;
            case 12:
                m_creature->GetMotionMaster()->MovePoint(0, 2084.584f,1278.331f,141.479f);
                JumpNextStep(4000);
                break;
            case 13:
                m_creature->GetMotionMaster()->MovePoint(0, 2087.414f,1279.293f,140.933f);
                JumpNextStep(2000);
                break;
            case 14:
                m_bStartEnterEvent = false;
                SetRun(true);
                m_pInstance->StartWaveEvent();
                m_pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, DONE);
                break;
            default:
                break;
        }
    }

    void HouseEvent()
    {
        switch(m_uiStep)
        {
            case 0:
                if (Creature* pHuman = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
                {
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pHuman->GetObjectGuid());
                    pHuman->SetGuidValue(UNIT_FIELD_TARGET, m_creature->GetObjectGuid());
                    DoScriptText(SAY_PHASE503, pHuman);
                }
                JumpNextStep(4000);
                break;
            case 1:
                DoScriptText(SAY_PHASE504, m_creature);
                m_creature->GetMotionMaster()->MovePoint(0, 2396.035f, 1206.942f, 134.038f);
                JumpNextStep(3000);
                break;
            case 2:
                m_creature->HandleEmoteCommand(37);
                JumpNextStep(2000);
                break;
            case 3:
                DoScriptText(SAY_PHASE505, m_creature);
                JumpNextStep(2000);
                break;
            case 4:
                if (Creature* pHuman = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
                    DoScriptText(SAY_PHASE506, pHuman);
                JumpNextStep(6000);
                break;
            case 5:
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                if (Creature* pHuman = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
                {
                    pHuman->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                    pHuman->UpdateEntry(NPC_INFINITE_ADVERSARY);
                }
                if (Creature* pHuman2 = m_pInstance->instance->GetCreature(m_uiHuman02GUID))
                    pHuman2->UpdateEntry(NPC_INFINITE_HUNTER);
                if (Creature* pHuman3 = m_pInstance->instance->GetCreature(m_uiHuman03GUID))
                    pHuman3->UpdateEntry(NPC_INFINITE_AGENT);
                JumpNextStep(1000);
                break;
            case 6:
                SetRun(true);
                m_creature->GetMotionMaster()->MovePoint(0, 2384.320f, 1202.779f, 134.040f);
                DoScriptText(SAY_PHASE507, m_creature);
                JumpNextStep(5000);
                break;
            case 7:
                m_bInHouseEvent = false;
                EnableEscort();
                //m_creature->setFaction(FACTION);
                JumpNextStep(1000);
                break;
            }
    }

    void EpochEvent()
    {
        switch(m_uiStep)
        {
            case 0:
                m_creature->SummonCreature(NPC_TIME_RIFT_2,2445.629f,1111.500f,148.076f,3.229f,TEMPSUMMON_TIMED_DESPAWN,9000);
                JumpNextStep(2000);
                break;
            case 1:
               if (Creature* pEpoch = m_creature->SummonCreature(NPC_CHRONO_LORD_EPOCH,2445.629f,1111.500f,148.076f,3.229f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000))
               {
                   //pEpoch->setFaction(35);
                   //DoScriptText(SAY_EPOCH_INTRO, pEpoch); Done in ACID
                   m_creature->SetGuidValue(UNIT_FIELD_TARGET, pEpoch->GetObjectGuid());
               }
               JumpNextStep(20000);
               break;
            case 2:
                DoScriptText(SAY_ARTHAS_INTRO, m_creature);
                JumpNextStep(6000);
                break;
            case 3:
                if (Creature* pEpoch = m_pInstance->GetSingleCreatureFromStorage(NPC_CHRONO_LORD_EPOCH))
                {
                    pEpoch->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
                    DoScriptText(SAY_EPOCH_AGGRO, pEpoch);
                    pEpoch->AI()->AttackStart(m_creature);
                    m_creature->AI()->AttackStart(pEpoch);
                    //pEpoch->setFaction(14);
                }
                m_bInEpocEvent = false;
                SetRun(false);
                JumpNextStep(6000);
                break;
            default:
                break;
        }
    }

    void MalganisEvent()
    {
         Map::PlayerList const &PlayerList = m_pInstance->instance->GetPlayers();
         bool bNeedSpawn = false;

         switch(m_uiStep)
         {
            case 0:
                // KillCredit with Spell, i have no idea how
                if (Player* pPlayer = m_pInstance->GetPlayerInMap(true))
                {
                    pPlayer->RewardPlayerAndGroupAtEvent(31006, pPlayer);
                }
                m_creature->GetMotionMaster()->MovePoint(0, 2302.326f, 1491.386f, 128.362f);
                if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                {
                    DoScriptText(SAY_MALGANIS_ESCAPE01, pMalganis);
                    m_creature->SetGuidValue(UNIT_FIELD_TARGET, pMalganis->GetObjectGuid());
                    pMalganis->SetGuidValue(UNIT_FIELD_TARGET, m_creature->GetObjectGuid());
                }
                JumpNextStep(10000);
                break;
             case 1:
                if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                   DoScriptText(SAY_MALGANIS_ESCAPE02, pMalganis);
                JumpNextStep(10000);
                break;
             case 2:
                DoScriptText(SAY_ARTHAS_OUTRO01, m_creature);
                JumpNextStep(5000);
                break;
             case 3:
                if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                   DoScriptText(SAY_MALGANIS_OUTRO, pMalganis);
                JumpNextStep(20000);
                break;
             case 4:
                if (Creature* pMalganis = m_pInstance->GetSingleCreatureFromStorage(NPC_MALGANIS))
                {
                    pMalganis->SetVisibility(VISIBILITY_OFF);
                    m_creature->GetMotionMaster()->MovePoint(0, pMalganis->GetPositionX(), pMalganis->GetPositionY(), pMalganis->GetPositionZ());
                }
                m_creature->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                m_creature->SetWalk(false);
                JumpNextStep(3000);
                break;
            case 5:
                DoScriptText(SAY_ARTHAS_OUTRO02, m_creature);
                JumpNextStep(6000);
                break;
            case 6:
                m_creature->GetMotionMaster()->MovePoint(0, 2298.298f,1500.362f,128.362f);
                DoScriptText(SAY_ARTHAS_OUTRO03, m_creature);
                JumpNextStep(11000);
                break;
            case 7:
                m_creature->GetMotionMaster()->MovePoint(0, 2243.311f, 1476.025f, 132.352f);
                JumpNextStep(11000);
                break;
            case 8:
                m_creature->SetVisibility(VISIBILITY_OFF);
                break;
         }
    }

    uint32 GetRandomWaveNPC()
    {
        uint32 entry = NPC_PATCHWORK_CONSTRUCT;
        switch (urand(0,8))
        {
            case 0: entry = NPC_ENTAGING_GHOUL; break;
            case 1: entry = NPC_DEVOURING_GHOUL; break;
            case 2: entry = NPC_DARK_NECROMANCER; break;
            case 3: entry = NPC_DARK_NECROMANCER; break;
            case 4: entry = NPC_CRYPT_FIEND; break;
            case 5: entry = NPC_TOMB_STALKER; break;
            case 6: entry = NPC_ACOLYTE; break;
            case 7: entry = NPC_BILE_GOLEM; break;
            case 8: entry = NPC_PATCHWORK_CONSTRUCT; break;
            default:
                break;
        }
        return entry;
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_bStartEvent == true)
        {
            switch(m_pInstance->GetInstancePosition())
            {
                case POS_ARTHAS_INTRO:
                {
                    if (m_bTalkWithUther)
                    {
                        if (m_uiStepTimer < uiDiff)
                        {
                            IntroEvent();
                        }
                        else
                            m_uiStepTimer -= uiDiff;
                    }
                    else if(m_bStartEnterEvent)
                    {
                        if (m_uiStepTimer < uiDiff)
                        {
                            EnterEvent();
                        }
                        else
                            m_uiStepTimer -= uiDiff;
                    }

                    break;
                }
                case POS_ARTHAS_WAVES:
                {
                    // nothing
                     break;
                }
                case POS_ARTHAS_TOWNHALL: // Arthas move to pub
                {
                    if (m_bInHouseEvent)
                    {
                        if(m_uiStepTimer < uiDiff)
                        {
                            HouseEvent();
                        }
                        else
                            m_uiStepTimer -= uiDiff;
                    }
                    else if (m_bInEpocEvent)
                    {
                        if(m_uiStepTimer < uiDiff)
                        {
                            EpochEvent();
                        }
                        else
                            m_uiStepTimer -= uiDiff;
                    }
                    break;
                }
                case POS_ARTHAS_ESCORTING:
                {
                    if (m_bStartEscort)
                    {
                        if (m_uiSummonZombie <= uiDiff)
                        {
                            float x, y, z;
                            float distX, distY, distZ;
                            float dist = frand(0, 10.0f);
                            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), dist, x, y,z);
                            m_creature->GetTerrain()->GetHitPosition(x, y, z, distX, distY, distZ, m_creature->GetPhaseMask(), -0.1f);
                            m_creature->SummonCreature(NPC_ZOMBIE, distX, distY, distZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 3 * MINUTE * IN_MILLISECONDS);
                            m_uiSummonZombie = 3 * IN_MILLISECONDS;
                        }
                        else
                            m_uiSummonZombie -= uiDiff;

                        if (m_uiStrongerNPC <= uiDiff)
                        {
                            float x, y, z;
                            float distX, distY, distZ;
                            float dist = frand(0, 10.0f);
                            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), dist, x, y,z);
                            m_creature->GetTerrain()->GetHitPosition(x, y, z, distX, distY, distZ, m_creature->GetPhaseMask(), -0.1f);
                            m_creature->SummonCreature(GetRandomWaveNPC(), distX, distY, distZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 3 * MINUTE * IN_MILLISECONDS);
                            m_uiStrongerNPC = 20 * IN_MILLISECONDS;
                        }
                        else
                            m_uiStrongerNPC -= uiDiff;
                    }
                    break;
                }
                case POS_ARTHAS_MALGANIS:
                {
                    // Do Nothing
                    break;
                }
                case POS_INSTANCE_FINISHED:
                {
                    if (m_uiStepTimer < uiDiff)
                    {
                        MalganisEvent();
                    }
                    else
                        m_uiStepTimer -= uiDiff;
                    return;
                }
                default:
                {
                    break;
                }
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiExorcismTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                if (DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_EXORCISM_N : SPELL_EXORCISM_H) == CAST_OK)
                    m_uiExorcismTimer = 7300;
            }
        }
        else
            m_uiExorcismTimer -= uiDiff;

        if (m_uiHealTimer < uiDiff)
        {
            if (m_creature->GetHealthPercent() < 70.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_HOLY_LIGHT) == CAST_OK)
                    m_uiHealTimer = 20000;
            }
        }
        else
            m_uiHealTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*###
## npc_arthas_dialog
###*/

enum
{
   GOSSIP_MENU_ARTHAS_1                        = 100001,
   GOSSIP_MENU_ARTHAS_2                        = 100002,
   GOSSIP_MENU_ARTHAS_3                        = 100003,
   GOSSIP_MENU_ARTHAS_4                        = 100004,
   GOSSIP_MENU_ARTHAS_5                        = 100005
};

#define GOSSIP_ITEM_ARTHAS_0 "I'm ready to start Culling of Stratholme."
#define GOSSIP_ITEM_ARTHAS_1 "Yes, my Prince. We're ready."
#define GOSSIP_ITEM_ARTHAS_2 "We're only doing what is best for Loarderon your Highness."
#define GOSSIP_ITEM_ARTHAS_3 "I'm ready."
#define GOSSIP_ITEM_ARTHAS_4 "For Lordaeron!"
#define GOSSIP_ITEM_ARTHAS_5 "I'm ready to battle the dreadlord, sire."

bool GossipHello_npc_arthas(Player* pPlayer, Creature* pCreature)
{
    instance_culling_of_stratholme* pInstance = ((instance_culling_of_stratholme*)pCreature->GetInstanceData());

    if (pInstance && pInstance->GetInstancePosition() == POS_ARTHAS_INTRO)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_1, pCreature->GetObjectGuid());
        if (!pInstance->instance->IsRegularDifficulty())
            pInstance->SetData(TYPE_INFINITE_CORRUPTER, IN_PROGRESS); // Start timer for Strat bonus Boss
    }

    if (pInstance && pInstance->GetInstancePosition() == POS_ARTHAS_TOWNHALL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_2, pCreature->GetObjectGuid());
    }

    if (pInstance && pInstance->GetInstancePosition() == POS_ARTHAS_ESCORTING)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_3, pCreature->GetObjectGuid());
    }

    if (pInstance && pInstance->GetInstancePosition() == POS_ARTHAS_MALGANIS)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_4, pCreature->GetObjectGuid());
    }

    return true;
}

bool GossipSelect_npc_arthas(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    instance_culling_of_stratholme* m_pInstance = ((instance_culling_of_stratholme*)pCreature->GetInstanceData());

    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            ((npc_arthasAI*)pCreature->AI())->EnableEscort();
            ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
        case GOSSIP_ACTION_INFO_DEF + 3:
            ((npc_arthasAI*)pCreature->AI())->EnableEscort();
            ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
            break;
        case GOSSIP_ACTION_INFO_DEF + 4:
            ((npc_arthasAI*)pCreature->AI())->EnableEscort();
            ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
            m_pInstance->TeleportMalganisAndSpawnIfNeeded(false);
            break;
        default:
            break;
    }
    return true;
}

/*###
## npc_uther
###*/

struct MANGOS_DLL_DECL npc_utherAI : public npc_escortAI
{
    npc_utherAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    instance_culling_of_stratholme* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    bool m_bStartEvent;

    void Reset()
    {
          m_creature->SetVisibility(VISIBILITY_OFF);
          m_uiStep = 0;
          m_uiStepTimer = 100;
    }

    void StartAI()
    {
           //m_pInstance->SetWeather(WEATHER_STATE_MEDIUM_RAIN, 0.9999f);
           m_bStartEvent = true;
           m_creature->SetVisibility(VISIBILITY_ON);
           Start(true);

           if (Creature* pKnight01 = m_creature->SummonCreature(NPC_KNIGHT_OF_UTHER,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,140000))
           {
               pKnight01->SetWalk(false);
               pKnight01->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,(M_PI_F/2.0f)*3);
           }

           if (Creature* pKnight02 = m_creature->SummonCreature(NPC_KNIGHT_OF_UTHER,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,140000))
           {
               pKnight02->SetWalk(false);
               pKnight02->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,(M_PI_F/2.0f)*2);
           }

           if (Creature* pKnight03 = m_creature->SummonCreature(NPC_KNIGHT_OF_UTHER,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,140000))
           {
               pKnight03->SetWalk(false);
               pKnight03->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,(M_PI_F/2.0f));
           }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 3:
                if (Creature* pArthas = m_pInstance->GetSingleCreatureFromStorage(NPC_ARTHAS))
                {
                    pArthas->SetWalk(false);
                    pArthas->SetGuidValue(UNIT_FIELD_TARGET, m_creature->GetObjectGuid());
                    pArthas->GetMotionMaster()->MovePoint(0, 1902.974f, 1291.635f, 143.337f);
                }
                break;
            case 4:
                SetRun(false);
                if (Creature *pArthas = m_pInstance->GetSingleCreatureFromStorage(NPC_ARTHAS))
                   ((npc_arthasAI*)pArthas->AI())->StartAI();
                break;
            case 6:
                m_creature->SetVisibility(VISIBILITY_OFF);
                if (Creature* pJaina = m_pInstance->GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE))
                    pJaina->SetVisibility(VISIBILITY_OFF);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
       npc_escortAI::UpdateAI(uiDiff);

       if (!m_pInstance) return;
    }
};


enum
{
    QUEST_DISPELLING_ILLUSIONS  = 13149,
    QUEST_A_ROYAL_ESCORT        = 13151,

    ITEM_ARCANE_DISRUPTOR       = 37888,

    GOSSIP_ITEM_ENTRANCE_1      = -3595000,
    GOSSIP_ITEM_ENTRANCE_2      = -3595001,
    GOSSIP_ITEM_ENTRANCE_3      = -3595002,

    TEXT_ID_ENTRANCE_1          = 12992,
    TEXT_ID_ENTRANCE_2          = 12993,
    TEXT_ID_ENTRANCE_3          = 12994,
    TEXT_ID_ENTRANCE_4          = 12995,

    GOSSIP_ITEM_INN_1           = -3595003,
    GOSSIP_ITEM_INN_2           = -3595004,
    GOSSIP_ITEM_INN_3           = -3595005,

    TEXT_ID_INN_1               = 12939,
    TEXT_ID_INN_2               = 12949,
    TEXT_ID_INN_3               = 12950,
    TEXT_ID_INN_4               = 12952,
};

bool GossipHello_npc_chromie(Player *pPlayer, Creature *pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
    {
        switch (pCreature->GetEntry())
        {
            case NPC_CHROMIE_INN:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
                {
                    if (pPlayer->GetQuestRewardStatus(QUEST_DISPELLING_ILLUSIONS) && !pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                }
                pPlayer->SEND_GOSSIP_MENU(TEXT_ID_INN_1, pCreature->GetObjectGuid());
                break;
            case NPC_CHROMIE_ENTRANCE:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == DONE && m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED && pPlayer->GetQuestRewardStatus(QUEST_A_ROYAL_ESCORT))
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(TEXT_ID_ENTRANCE_1, pCreature->GetObjectGuid());
                break;
        }
    }
    return true;
}

bool GossipSelect_npc_chromie(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 uiAction)
{
    pPlayer->PlayerTalkClass->ClearMenus();
    switch (pCreature->GetEntry())
    {
        case NPC_CHROMIE_INN:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_INN_2, pCreature->GetObjectGuid());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_INN_3, pCreature->GetObjectGuid());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_INN_4, pCreature->GetObjectGuid());
                    if (!pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                    {
                        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_ARCANE_DISRUPTOR, 1))
                        {
                            pPlayer->SendNewItem(pItem, 1, true, false);
                            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                            {
                                if (pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                                    pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
                            }
                        }
                    }
                    break;
            }
            break;
        case NPC_CHROMIE_ENTRANCE:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_ENTRANCE_2, pCreature->GetObjectGuid());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_ENTRANCE_3, pCreature->GetObjectGuid());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(TEXT_ID_ENTRANCE_4, pCreature->GetObjectGuid());
                    if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                    {
                        if (pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
                        {
                            pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, IN_PROGRESS);
                            pInstance->DoSpawnArthasIfNeeded();
                            pInstance->DoSpawnUtherAndJainaIfNeeded();
                            if (Creature* pUther = pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                            {
                                ((npc_utherAI*)pUther->AI())->StartAI();
                            }
                        }
                    }
                    break;
            }
            break;
    }
    return true;
}

bool QuestAccept_npc_chromie(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    switch (pQuest->GetQuestId())
    {
        case QUEST_DISPELLING_ILLUSIONS:
            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
            {
                if (pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                    pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
            }
            break;
        case QUEST_A_ROYAL_ESCORT:
            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
            {
                if (pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
                {
                    pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, IN_PROGRESS);
                    pInstance->DoSpawnArthasIfNeeded();
                    pInstance->DoSpawnUtherAndJainaIfNeeded();
                    if (Creature* pUther = pInstance->GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER))
                    {
                        ((npc_utherAI*)pUther->AI())->StartAI();
                    }
                }
            }
            break;
    }
    return true;
}

/* *************
** npc_crates_bunny (spell aura effect dummy)
************* */

enum
{
    SPELL_ARCANE_DISRUPTION     = 49590,
    SPELL_CRATES_KILL_CREDIT    = 58109,
};

bool EffectAuraDummy_spell_aura_dummy_npc_crates_dummy(const Aura* pAura, bool bApply)
{
    if (pAura->GetId() == SPELL_ARCANE_DISRUPTION && pAura->GetEffIndex() == EFFECT_INDEX_0 && bApply)
    {
        if (Creature* pTarget = (Creature*)pAura->GetTarget())
        {
            std::list<Creature*> lCrateBunnyList;
            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pTarget->GetInstanceData())
            {
                pInstance->GetCratesBunnyOrderedList(lCrateBunnyList);
                uint8 i = 0;
                for (std::list<Creature*>::const_iterator itr = lCrateBunnyList.begin(); itr != lCrateBunnyList.end(); ++itr)
                {
                    ++i;
                    if (*itr == pTarget)
                        break;
                }

                bool bEventAlreadyInProgress = false;
                switch (i)
                {
                    case 1:
                        // Start NPC_ROGER_OWENS Event
                        bEventAlreadyInProgress = pInstance->StartCratesEvent(NPC_ROGER_OWENS);
                        break;
                    case 2:
                        // Start NPC_SERGEANT_MORIGAN  Event
                        bEventAlreadyInProgress = pInstance->StartCratesEvent(NPC_SERGEANT_MORIGAN);
                        break;
                    case 3:
                        // Start NPC_JENA_ANDERSON Event
                        bEventAlreadyInProgress = pInstance->StartCratesEvent(NPC_JENA_ANDERSON);
                        break;
                    case 4:
                        // Start NPC_MALCOM_MOORE Event
                        bEventAlreadyInProgress = pInstance->StartCratesEvent(NPC_MALCOM_MOORE);
                        break;
                    case 5:
                        // Start NPC_BARTLEBY_BATTSON Event
                        bEventAlreadyInProgress = pInstance->StartCratesEvent(NPC_BARTLEBY_BATTSON);
                        break;
                    default:
                        break;
                }

                if (pInstance->GetData(TYPE_GRAIN_EVENT) != DONE && !bEventAlreadyInProgress)
                {
                    pInstance->SetData(TYPE_GRAIN_EVENT, IN_PROGRESS);

                    // Finished event, give killcredit
                    if (pInstance->GetData(TYPE_GRAIN_EVENT) == DONE)
                    {
                        pTarget->CastSpell(pTarget, SPELL_CRATES_KILL_CREDIT, true);
                        // Hack
                        if (Player* pPlayer = pInstance->GetPlayerInMap(true))
                        {
                            pPlayer->RewardPlayerAndGroupAtEvent(30996, pPlayer);
                        }
                    }
                }

                pTarget->ForcedDespawn(6000);    // direct despawn has influence on visual effects,
                                                // but despawning makes it impossible to multi-use the spell at the same place
                // perhaps some add. GO-Visual
            }
        }
    }
    return true;
}

CreatureAI* GetAI_npc_uther(Creature* pCreature)
{
    return new npc_utherAI(pCreature);
}

CreatureAI* GetAI_npc_arthas(Creature* pCreature)
{
    return new npc_arthasAI(pCreature);
}

bool AreaTrigger_at_culling_of_stratholme_event_inn(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_INN)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pPlayer->GetInstanceData();

        if (!pInstance)
            return false;

        pInstance->StartCratesEvent(NPC_MICHAEL_BELFAST);
    }
    return true;
}

void AddSC_culling_of_stratholme()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_chromie";
    pNewScript->pGossipHello = &GossipHello_npc_chromie;
    pNewScript->pGossipSelect = &GossipSelect_npc_chromie;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_chromie;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "spell_dummy_npc_crates_bunny";
    pNewScript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_npc_crates_dummy;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_uther";
    pNewScript->GetAI = &GetAI_npc_uther;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_arthas";
    pNewScript->GetAI = &GetAI_npc_arthas;
    pNewScript->pGossipHello =  &GossipHello_npc_arthas;
    pNewScript->pGossipSelect = &GossipSelect_npc_arthas;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_culling_of_stratholme";
    pNewScript->pAreaTrigger = &AreaTrigger_at_culling_of_stratholme_event_inn;
    pNewScript->RegisterSelf();
}

