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
SDName: instance_icecrown_spire
SD%Complete: 90%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"
#include "World.h"

enum
{
    // Marrowgar
    SAY_MARROWGAR_INTRO = -1631001,

    // Deathwhisper
    SAY_DEATHWHISPER_SPEECH_1 = -1631011,
    SAY_DEATHWHISPER_SPEECH_2 = -1631012,
    SAY_DEATHWHISPER_SPEECH_3 = -1631013,
    SAY_DEATHWHISPER_SPEECH_4 = -1631014,
    SAY_DEATHWHISPER_SPEECH_5 = -1631015,
    SAY_DEATHWHISPER_SPEECH_6 = -1631016,
    SAY_DEATHWHISPER_SPEECH_7 = -1631017,

    // Deathbringer Saurfang
    SAY_SAURFANG_INTRO_ALLY_0            = -1631035,
    SAY_SAURFANG_INTRO_ALLY_1            = -1631036,
    SAY_SAURFANG_INTRO_ALLY_2            = -1631037,
    SAY_SAURFANG_INTRO_ALLY_3            = -1631038,
    SAY_SAURFANG_INTRO_ALLY_4            = -1631039,
    SAY_SAURFANG_INTRO_ALLY_5            = -1631040,
    SAY_SAURFANG_INTRO_HORDE_1           = -1631041,
    SAY_SAURFANG_INTRO_HORDE_2           = -1631042,
    SAY_SAURFANG_INTRO_HORDE_3           = -1631043,
    SAY_SAURFANG_INTRO_HORDE_4           = -1631044,
    SAY_SAURFANG_INTRO_HORDE_5           = -1631045,
    SAY_SAURFANG_INTRO_HORDE_6           = -1631046,
    SAY_SAURFANG_INTRO_HORDE_7           = -1631047,
    SAY_SAURFANG_INTRO_HORDE_8           = -1631048,
    SAY_SAURFANG_INTRO_HORDE_9           = -1631049,
    SAY_SAURFANG_OUTRO_ALLY_1            = -1631050,
    SAY_SAURFANG_OUTRO_ALLY_2            = -1631051,
    SAY_SAURFANG_OUTRO_ALLY_3            = -1631052,
    SAY_SAURFANG_OUTRO_ALLY_4            = -1631053,
    SAY_SAURFANG_OUTRO_ALLY_5            = -1631054,
    SAY_SAURFANG_OUTRO_ALLY_6            = -1631055,
    SAY_SAURFANG_OUTRO_ALLY_7            = -1631056,
    SAY_SAURFANG_OUTRO_ALLY_8            = -1631057,
    SAY_SAURFANG_OUTRO_ALLY_9            = -1631058,
    SAY_SAURFANG_OUTRO_ALLY_10           = -1631059,
    SAY_SAURFANG_OUTRO_ALLY_11           = -1631060,
    SAY_SAURFANG_OUTRO_ALLY_12           = -1631061,
    SAY_SAURFANG_OUTRO_ALLY_13           = -1631062,
    SAY_SAURFANG_OUTRO_ALLY_14           = -1631063,
    SAY_SAURFANG_OUTRO_ALLY_15           = -1631064,
    SAY_SAURFANG_OUTRO_ALLY_16           = -1631065,
    SAY_SAURFANG_OUTRO_HORDE_1           = -1631066,
    SAY_SAURFANG_OUTRO_HORDE_2           = -1631067,
    SAY_SAURFANG_OUTRO_HORDE_3           = -1631068,
    SAY_SAURFANG_OUTRO_HORDE_4           = -1631069,

    // Festergut
    SAY_STINKY_DIES = -1631081,
    // Rotface
    SAY_PRECIOUS_DIES = -1631070,
};

static const DialogueEntry aCitadelDialogue[] =
{
    {SAY_DEATHWHISPER_SPEECH_1, NPC_LADY_DEATHWHISPER, 12000},
    {SAY_DEATHWHISPER_SPEECH_2, NPC_LADY_DEATHWHISPER, 11000},
    {SAY_DEATHWHISPER_SPEECH_3, NPC_LADY_DEATHWHISPER, 10000},
    {SAY_DEATHWHISPER_SPEECH_4, NPC_LADY_DEATHWHISPER, 9000},
    {SAY_DEATHWHISPER_SPEECH_5, NPC_LADY_DEATHWHISPER, 10000},
    {SAY_DEATHWHISPER_SPEECH_6, NPC_LADY_DEATHWHISPER, 10000},
    {SAY_DEATHWHISPER_SPEECH_7, NPC_LADY_DEATHWHISPER, 0},
    {EVENT_START_SAURFANG_INTRO_ALLY, 0, 4000},
    {SAY_SAURFANG_INTRO_ALLY_0, NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_INTRO_ALLY_1, NPC_DEATHBRINGER_SAURFANG, 4000},
    {SAY_SAURFANG_INTRO_ALLY_2, NPC_DEATHBRINGER_SAURFANG, 4000},
    {SAY_SAURFANG_INTRO_ALLY_3, NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_INTRO_ALLY_4, NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_INTRO_ALLY_5, NPC_DEATHBRINGER_SAURFANG, 1000},
    {EVENT_STOP_SAURFANG_INTRO_ALLY, 0, 0},
    {EVENT_START_SAURFANG_INTRO_HORDE, 0, 1000},
    {SAY_SAURFANG_INTRO_HORDE_1, NPC_OVERLORD_SAURFANG      , 6000},
    {SAY_SAURFANG_INTRO_HORDE_2, NPC_DEATHBRINGER_SAURFANG  , 13000},
    {SAY_SAURFANG_INTRO_HORDE_3, NPC_OVERLORD_SAURFANG      , 7000},
    {SAY_SAURFANG_INTRO_HORDE_4, NPC_DEATHBRINGER_SAURFANG  , 11000},
    {SAY_SAURFANG_INTRO_HORDE_5, NPC_OVERLORD_SAURFANG      , 16000},
    {SAY_SAURFANG_INTRO_HORDE_6, NPC_OVERLORD_SAURFANG      , 15000},
    {SAY_SAURFANG_INTRO_HORDE_7, NPC_OVERLORD_SAURFANG      , 3000},
    {SAY_SAURFANG_INTRO_HORDE_8, NPC_OVERLORD_SAURFANG      , 1500},
    {SAY_SAURFANG_INTRO_HORDE_9, NPC_DEATHBRINGER_SAURFANG  , 10000},
    {EVENT_STOP_SAURFANG_INTRO_HORDE, 0, 0},
    {EVENT_START_SAURFANG_OUTRO_ALLY, 0, 1000},
    {SAY_SAURFANG_OUTRO_ALLY_1,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_2,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_3,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_4,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_5,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_6,  NPC_OVERLORD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_7,  NPC_MURADIN_BRONZEBEARD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_8,  NPC_KING_VARIAN_WRYNN, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_9,  NPC_OVERLORD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_10, NPC_OVERLORD_SAURFANG, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_11, NPC_KING_VARIAN_WRYNN, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_12, NPC_LADY_JAINA_PROUDMOORE, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_13, NPC_KING_VARIAN_WRYNN, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_14, NPC_LADY_JAINA_PROUDMOORE, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_15, NPC_KING_VARIAN_WRYNN, 4000},
    {SAY_SAURFANG_OUTRO_ALLY_16, NPC_MURADIN_BRONZEBEARD_SAURFANG, 1000},
    {EVENT_STOP_SAURFANG_OUTRO_ALLY, 0, 0},
    {EVENT_START_SAURFANG_OUTRO_HORDE, 0, 1000},
    {SAY_SAURFANG_OUTRO_HORDE_1, NPC_OVERLORD_SAURFANG, 2000},
    {EVENT_SAURFANG_OUTRO_HORDE_ONE, 0, 10000},
    {SAY_SAURFANG_OUTRO_HORDE_2, NPC_OVERLORD_SAURFANG, 3000},
    {SAY_SAURFANG_OUTRO_HORDE_3, NPC_OVERLORD_SAURFANG, 11000},
    {SAY_SAURFANG_OUTRO_HORDE_4, NPC_OVERLORD_SAURFANG, 10000},
    {EVENT_STOP_SAURFANG_OUTRO_HORDE, 0, 0},
    {0, 0, 0},
};

static LOCATION fSaurfangPositions[12]=
{
    {-468.05f, 2211.69f, 541.11f, 3.16f}, // Deathbringer teleport point
    {-491.30f, 2211.35f, 541.11f, 3.16f}, // Deathbringer dest point
    {-559.82f, 2211.29f, 539.30f, 6.28f}, // main npc summon/despawn point
    {-561.87f, 2215.94f, 539.30f, 6.28f}, // guard npc1 summon/despawn point
    {-560.17f, 2214.17f, 539.30f, 6.28f}, // guard npc2 summon/despawn point
    {-560.17f, 2207.71f, 539.30f, 6.28f}, // guard npc3 summon/despawn point
    {-561.87f, 2205.68f, 539.30f, 6.28f}, // guard npc4 summon/despawn point
    {-534.82f, 2211.29f, 539.30f, 6.28f}, // main npc first move
    {-536.87f, 2215.94f, 539.30f, 6.28f}, // guard npc1 first move
    {-535.17f, 2214.17f, 539.30f, 6.28f}, // guard npc2 first move
    {-535.17f, 2207.71f, 539.30f, 6.28f}, // guard npc3 first move
    {-536.87f, 2205.68f, 539.30f, 6.28f} // guard npc4 first move
};
/*
static Locations SpawnLoc[]=
{
    {-446.788971f, 2003.362915f, 191.233948f, 0.0f},  // 0 Horde ship enter
    {-428.140503f, 2421.336914f, 191.233078f, 0.0f},  // 1 Alliance ship enter
};
*/
instance_icecrown_citadel::instance_icecrown_citadel(Map* pMap) : ScriptedInstance(pMap), DialogueHelper(aCitadelDialogue),
m_uiTeam(0),
m_uiPutricideValveTimer(0),
m_bHasMarrowgarIntroYelled(false),
m_bHasDeathwhisperIntroYelled(false),
m_bHasRimefangLanded(false),
m_bHasSpinestalkerLanded(false)
{
    Initialize();
}

void instance_icecrown_citadel::Initialize()
{
    InitializeDialogueHelper(this);
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    for (uint8 i = 0; i < MAX_ACHIEVEMENT; ++i)
        m_abAchievement[i] = false;
}

bool instance_icecrown_citadel::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_icecrown_citadel::DoHandleCitadelAreaTrigger(uint32 uiTriggerId, Player* pPlayer)
{
    if (uiTriggerId == AREATRIGGER_MARROWGAR_INTRO && !m_bHasMarrowgarIntroYelled)
    {
        if (Creature* pMarrowgar = GetSingleCreatureFromStorage(NPC_LORD_MARROWGAR))
        {
            DoScriptText(SAY_MARROWGAR_INTRO, pMarrowgar);
            m_bHasMarrowgarIntroYelled = true;
        }
    }
    else if (uiTriggerId == AREATRIGGER_DEATHWHISPER_INTRO && !m_bHasDeathwhisperIntroYelled)
    {
        StartNextDialogueText(SAY_DEATHWHISPER_SPEECH_1);
        m_bHasDeathwhisperIntroYelled = true;
    }
    else if (uiTriggerId == AREATRIGGER_SINDRAGOSA_PLATFORM)
    {
        if (Creature* pSindragosa = GetSingleCreatureFromStorage(NPC_SINDRAGOSA))
        {
            if (pSindragosa->isAlive() && !pSindragosa->isInCombat())
                pSindragosa->SetInCombatWithZone();
        }
        else
        {
            if (!m_bHasRimefangLanded)
            {
                if (Creature* pRimefang = GetSingleCreatureFromStorage(NPC_RIMEFANG))
                {
                    pRimefang->AI()->AttackStart(pPlayer);
                    m_bHasRimefangLanded = true;
                }
            }

            if (!m_bHasSpinestalkerLanded)
            {
                if (Creature* pSpinestalker = GetSingleCreatureFromStorage(NPC_SPINESTALKER))
                {
                    pSpinestalker->AI()->AttackStart(pPlayer);
                    m_bHasSpinestalkerLanded = true;
                }
            }
        }
    }
}

void instance_icecrown_citadel::SetSpecialAchievementCriteria(IcecrownAchievments uiType, bool bIsMet)
{
    if (uiType < MAX_ACHIEVEMENT)
        m_abAchievement[uiType] = bIsMet;
}


void instance_icecrown_citadel::OnPlayerEnter(Player* pPlayer)
{
    if (!m_uiTeam) // very first player to enter
        m_uiTeam = pPlayer->GetTeam();
}

void instance_icecrown_citadel::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_LORD_MARROWGAR:
        case NPC_LADY_DEATHWHISPER:
        // Sourfang encounter
        case NPC_DEATHBRINGER_SAURFANG:
        case NPC_MURADIN_BRONZEBEARD_SAURFANG:
        case NPC_KING_VARIAN_WRYNN:
        case NPC_OVERLORD_SAURFANG:
        case NPC_LADY_JAINA_PROUDMOORE:
        // Putricide encounter
        case NPC_FESTERGUT:
        case NPC_ROTFACE:
        case NPC_PROFESSOR_PUTRICIDE:
        case NPC_MUTATED_ABOMINATION:
        case NPC_TALDARAM:
        case NPC_VALANAR:
        case NPC_KELESETH:
        case NPC_LANATHEL_INTRO:
        case NPC_VALITHRIA:
        case NPC_SINDRAGOSA:
        case NPC_LICH_KING:
        case NPC_TIRION:
        case NPC_RIMEFANG:
        case NPC_SPINESTALKER:
        case NPC_VALITHRIA_COMBAT_TRIGGER:
        case NPC_BLOOD_ORB_CONTROL:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case NPC_DEATHWHISPER_SPAWN_STALKER:
            m_lDeathwhisperStalkersGuids.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_KORKRON_REAVER:
        case NPC_ALLIANCE_MARINE:
            m_lSaurfangGuardGuids.push_back(pCreature->GetObjectGuid());
            return;
        default:
            return;
    }
}

void instance_icecrown_citadel::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ICEWALL_1:
        case GO_ICEWALL_2:
        case GO_ORATORY_DOOR:
            if (m_auiEncounter[TYPE_MARROWGAR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DEATHWHISPER_ELEVATOR:
            if (m_auiEncounter[TYPE_LADY_DEATHWHISPER] == DONE)
            {
                /* Disable till Battleship Encounter works
                pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                pGo->SetGoState(GO_STATE_READY);
                */
            }
            break;
        case GO_SAURFANG_DOOR:
        case GO_SCIENTIST_DOOR:
        case GO_CRIMSON_HALL_DOOR:
        case GO_GREEN_DRAGON_ENTRANCE:
            if (m_auiEncounter[TYPE_DEATHBRINGER_SAURFANG] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ORANGE_TUBE:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_GREEN_TUBE:
            if (m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SCIENTIST_DOOR_GREEN:
            // If both Festergut and Rotface are DONE, set as ACTIVE_ALTERNATIVE
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE && m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            else if (m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_SCIENTIST_DOOR_ORANGE:
            // If both Festergut and Rotface are DONE, set as ACTIVE_ALTERNATIVE
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE && m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            else if (m_auiEncounter[TYPE_FESTERGUT] == DONE)
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_SCIENTIST_DOOR_COLLISION:
            if (m_auiEncounter[TYPE_FESTERGUT] == DONE && m_auiEncounter[TYPE_ROTFACE] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_COUNCIL_DOOR_1:
        case GO_COUNCIL_DOOR_2:
            if (m_auiEncounter[TYPE_BLOOD_PRINCE_COUNCIL] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_GREEN_DRAGON_EXIT:
            if (m_auiEncounter[TYPE_VALITHRIA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_SAURFANG_CACHE:
        case GO_SAURFANG_CACHE_25:
        case GO_SAURFANG_CACHE_10_H:
        case GO_SAURFANG_CACHE_25_H:
            m_mGoEntryGuidStore[GO_SAURFANG_CACHE] = pGo->GetObjectGuid();
            return;
        case GO_GUNSHIP_ARMORY_A:
        case GO_GUNSHIP_ARMORY_A_25:
        case GO_GUNSHIP_ARMORY_A_10H:
        case GO_GUNSHIP_ARMORY_A_25H:
            m_mGoEntryGuidStore[GO_GUNSHIP_ARMORY_A] = pGo->GetObjectGuid();
            return;
        case GO_GUNSHIP_ARMORY_H:
        case GO_GUNSHIP_ARMORY_H_25:
        case GO_GUNSHIP_ARMORY_H_10H:
        case GO_GUNSHIP_ARMORY_H_25H:
            m_mGoEntryGuidStore[GO_GUNSHIP_ARMORY_H] = pGo->GetObjectGuid();
            return;
        case GO_DREAMWALKER_CACHE:
        case GO_DREAMWALKER_CACHE_25:
        case GO_DREAMWALKER_CACHE_10_H:
        case GO_DREAMWALKER_CACHE_25_H:
            m_mGoEntryGuidStore[GO_DREAMWALKER_CACHE] = pGo->GetObjectGuid();
            return;
        case GO_ICESHARD_1:
        case GO_ICESHARD_2:
        case GO_ICESHARD_3:
        case GO_ICESHARD_4:
        case GO_FROSTY_WIND:
        case GO_FROSTY_EDGE:
        case GO_SNOW_EDGE:
        case GO_ARTHAS_PLATFORM:
        case GO_ARTHAS_PRECIPICE:
        case GO_MARROWGAR_DOOR:
        case GO_BLOODPRINCE_DOOR:
        case GO_SINDRAGOSA_ENTRANCE:
        case GO_VALITHRIA_DOOR_1:
        case GO_VALITHRIA_DOOR_2:
        case GO_VALITHRIA_DOOR_3:
        case GO_VALITHRIA_DOOR_4:
        case GO_ICECROWN_GRATE:
        case GO_SINDRAGOSA_SHORTCUT_ENTRANCE:
        case GO_SINDRAGOSA_SHORTCUT_EXIT:
        case GO_ORANGE_PLAGUE:
        case GO_GREEN_PLAGUE:
        case GO_ORANGE_VALVE:
        case GO_GREEN_VALVE:
        case GO_DRINK_ME_TABLE:
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_icecrown_citadel::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_STINKY:
            if (Creature* pFestergut = GetSingleCreatureFromStorage(NPC_FESTERGUT))
            {
                if (pFestergut->isAlive())
                    DoScriptText(SAY_STINKY_DIES, pFestergut);
            }
            break;
        case NPC_PRECIOUS:
            if (Creature* pRotface = GetSingleCreatureFromStorage(NPC_ROTFACE))
            {
                if (pRotface->isAlive())
                    DoScriptText(SAY_PRECIOUS_DIES, pRotface);
            }
            break;
        default:
            break;
    }
}

void instance_icecrown_citadel::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_MARROWGAR:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MARROWGAR_DOOR);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_BONED, true);
            if (uiData == FAIL)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_BONED, false);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ICEWALL_1);
                DoUseDoorOrButton(GO_ICEWALL_2);

                // Note: this door use may not be correct. In theory the door should be already opened
                DoUseDoorOrButton(GO_ORATORY_DOOR);
            }
            break;
        case TYPE_LADY_DEATHWHISPER:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ORATORY_DOOR);
            if (uiData == DONE)
            {
                /* Disable till Battleship Encounter works
                if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_DEATHWHISPER_ELEVATOR))
                {
                      pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                      pGo->SetGoState(GO_STATE_READY);
                }*/
            }
            break;
        case TYPE_GUNSHIP_BATTLE:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiTeam == ALLIANCE ? GO_GUNSHIP_ARMORY_A : GO_GUNSHIP_ARMORY_H, 60 * MINUTE);
            break;
        case TYPE_DEATHBRINGER_SAURFANG:
            m_auiEncounter[uiType] = uiData;
            switch(uiData)
            {
                case SPECIAL:
                {
                    StartNextDialogueText(m_uiTeam == ALLIANCE ? EVENT_START_SAURFANG_INTRO_ALLY : EVENT_START_SAURFANG_INTRO_HORDE);
                    break;
                }
                case DONE:
                {
                    StartNextDialogueText(m_uiTeam == ALLIANCE ? EVENT_START_SAURFANG_OUTRO_ALLY : EVENT_START_SAURFANG_OUTRO_HORDE);
                    DoUseDoorOrButton(GO_SAURFANG_DOOR);
                    DoRespawnGameObject(GO_SAURFANG_CACHE, 60 * MINUTE);
                     // Note: these doors may not be correct. In theory the doors should be already opened
                    DoUseDoorOrButton(GO_SCIENTIST_DOOR);
                    DoUseDoorOrButton(GO_CRIMSON_HALL_DOOR);
                    DoUseDoorOrButton(GO_GREEN_DRAGON_ENTRANCE);
                    break;
                }
                case IN_PROGRESS:
                {
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_IVE_GONE_AND_MADE_A_MESS, true);
                    break;
                }
                case FAIL:
                {
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_IVE_GONE_AND_MADE_A_MESS, false);
                    break;
                }
                default:
                    break;
            }
            break;
        case TYPE_FESTERGUT:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ORANGE_PLAGUE);
            if (uiData == DONE)
                DoToggleGameObjectFlags(GO_ORANGE_VALVE, GO_FLAG_NO_INTERACT, false);
            else if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_FLU_SHOT_SHORTAGE, true);
            else if (uiData == FAIL)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_FLU_SHOT_SHORTAGE, false);
            break;
        case TYPE_ROTFACE:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_GREEN_PLAGUE);
            if (uiData == DONE)
                DoToggleGameObjectFlags(GO_GREEN_VALVE, GO_FLAG_NO_INTERACT, false);
            else if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_DANCES_WITH_OOZES, true);
            else if (uiData == FAIL)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_DANCES_WITH_OOZES, false);
            break;
        case TYPE_PROFESSOR_PUTRICIDE:
        {
            m_auiEncounter[uiType] = uiData;
            //DoUseDoorOrButton(GO_SCIENTIST_DOOR);
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_NAUSEA, true);
            else if (uiData == FAIL)
            {
                if (GameObject* pGOTable = GetSingleGameObjectFromStorage(GO_DRINK_ME_TABLE))
                    pGOTable->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_NAUSEA, false);
            }
            // Proff sometimes does't trigger door, so let's check it explicitly
           GameObject* pDoor = GetSingleGameObjectFromStorage(GO_SCIENTIST_DOOR);
           if (pDoor)
           {
               switch (uiData)
               {
                   case IN_PROGRESS:
                   case SPECIAL:
                   {
                       // Close door if it's open
                       if (pDoor->getLootState() != GO_ACTIVATED)
                           DoUseDoorOrButton(GO_SCIENTIST_DOOR);
                       break;
                   }
                   case NOT_STARTED:
                   case FAIL:
                   case DONE:
                   {
                       // Open door if it's closed
                       if (pDoor->getLootState() != GO_READY)
                           DoUseDoorOrButton(GO_SCIENTIST_DOOR);
                       break;
                   }
                   default: break;
               }
           }
           break;
        }
        case TYPE_BLOOD_PRINCE_COUNCIL:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_CRIMSON_HALL_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_COUNCIL_DOOR_1);
                DoUseDoorOrButton(GO_COUNCIL_DOOR_2);
            }
            else if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_ORB_WHISPERER, true);
            else if (uiData == FAIL)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_ORB_WHISPERER, false);
            break;
        case TYPE_QUEEN_LANATHEL:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_BLOODPRINCE_DOOR);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_ICECROWN_GRATE);
            break;
        case TYPE_VALITHRIA:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_GREEN_DRAGON_ENTRANCE);
            // Side doors
            DoUseDoorOrButton(GO_VALITHRIA_DOOR_1);
            DoUseDoorOrButton(GO_VALITHRIA_DOOR_2);
            if (Is25ManDifficulty())
            {
                DoUseDoorOrButton(GO_VALITHRIA_DOOR_3);
                DoUseDoorOrButton(GO_VALITHRIA_DOOR_4);
            }
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_GREEN_DRAGON_EXIT);
                DoUseDoorOrButton(GO_SINDRAGOSA_ENTRANCE);
                DoUseDoorOrButton(GO_SINDRAGOSA_SHORTCUT_ENTRANCE);
                DoUseDoorOrButton(GO_SINDRAGOSA_SHORTCUT_EXIT);
                DoRespawnGameObject(GO_DREAMWALKER_CACHE, 60 * MINUTE);
            }
            else if (uiData == IN_PROGRESS)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_PORTAL_JOCKEY, false);
            }
            else if (uiData == FAIL)
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_PORTAL_JOCKEY, false);
            }
            break;
        case TYPE_SINDRAGOSA:
             m_auiEncounter[uiType] = uiData;
             DoUseDoorOrButton(GO_SINDRAGOSA_ENTRANCE);
             if (uiData == IN_PROGRESS)
                 SetSpecialAchievementCriteria(TYPE_ACHIEVE_ALL_YOU_CAN_EAT, true);
             else if (uiData == FAIL)
                 SetSpecialAchievementCriteria(TYPE_ACHIEVE_ALL_YOU_CAN_EAT, false);
            break;
        case TYPE_LICH_KING:
            m_auiEncounter[TYPE_LICH_KING] = uiData;
            if (uiData == IN_PROGRESS)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_BEEN_WAITING_A_LONG_TIME, false);
            else if (uiData == FAIL)
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_BEEN_WAITING_A_LONG_TIME, false);
            break;
            /*
         case TYPE_FROSTMOURNE_ROOM:
             m_auiEncounter[TYPE_FROSTMOURNE_ROOM] = uiData;
             break;
         case TYPE_ICECROWN_QUESTS:
            m_auiEncounter[TYPE_ICECROWN_QUESTS] = uiData;
            break;*/
        default:
            script_error_log("Instance Icecrown Citadel: ERROR SetData = %u for type %u does not exist/not implemented.", uiType, uiData);
            return;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                   << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                   << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                   << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_icecrown_citadel::GetData(uint32 uiType)
{
    if (uiType == TYPE_KINGS_OF_ICC)
    {
        return GetData(TYPE_SINDRAGOSA) == DONE && GetData(TYPE_QUEEN_LANATHEL) == DONE && GetData(TYPE_PROFESSOR_PUTRICIDE) == DONE;
    }
    if (uiType == TYPE_TELEPORT_ALWAYS_ENABLE)
        return DONE;
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

bool instance_icecrown_citadel::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscvalue1)
{
    switch (uiCriteriaId)
    {
            // Lord Marrowgar
        case CRITERIA_BONED_10N:
        case CRITERIA_BONED_25N:
        case CRITERIA_BONED_10H:
        case CRITERIA_BONED_25H:
            return m_abAchievement[TYPE_ACHIEVE_BONED];
            // Lady Deathwhispher
        case CRITERIA_FULL_HOUSE_10N:
        case CRITERIA_FULL_HOUSE_25N:
        case CRITERIA_FULL_HOUSE_10H:
        case CRITERIA_FULL_HOUSE_25H:
            return m_abAchievement[TYPE_ACHIEVE_FULL_HOUSE];
            // Gunship Battle
        case CRITERIA_IM_ON_A_BOAT_10N:
        case CRITERIA_IM_ON_A_BOAT_25N:
        case CRITERIA_IM_ON_A_BOAT_10H:
        case CRITERIA_IM_ON_A_BOAT_25H:
            return m_abAchievement[TYPE_ACHIEVE_IM_ON_A_BOAT];
            // Deathbringer Saurfang
        case CRITERIA_IVE_GONE_AND_MADE_A_MESS_10N:
        case CRITERIA_IVE_GONE_AND_MADE_A_MESS_25N:
        case CRITERIA_IVE_GONE_AND_MADE_A_MESS_10H:
        case CRITERIA_IVE_GONE_AND_MADE_A_MESS_25H:
            return m_abAchievement[TYPE_ACHIEVE_IVE_GONE_AND_MADE_A_MESS];
            // Festergut
        case CRITERIA_FLU_SHOT_SHORTAGE_10N:
        case CRITERIA_FLU_SHOT_SHORTAGE_25N:
        case CRITERIA_FLU_SHOT_SHORTAGE_10H:
        case CRITERIA_FLU_SHOT_SHORTAGE_25H:
            return m_abAchievement[TYPE_ACHIEVE_FLU_SHOT_SHORTAGE];
            // Rotface
        case CRITERIA_DANCES_WITH_OOZES_10N:
        case CRITERIA_DANCES_WITH_OOZES_25N:
        case CRITERIA_DANCES_WITH_OOZES_10H:
        case CRITERIA_DANCES_WITH_OOZES_25H:
            return m_abAchievement[TYPE_ACHIEVE_DANCES_WITH_OOZES];
            // Professor Putricide
        case CRITERIA_NAUSEA_10N:
        case CRITERIA_NAUSEA_25N:
        case CRITERIA_NAUSEA_10H:
        case CRITERIA_NAUSEA_25H:
            return m_abAchievement[TYPE_ACHIEVE_NAUSEA];
            // Blood Prince Council
        case CRITERIA_ORB_WHISPERER_10N:
        case CRITERIA_ORB_WHISPERER_25N:
        case CRITERIA_ORB_WHISPERER_10H:
        case CRITERIA_ORB_WHISPERER_25H:
            return m_abAchievement[TYPE_ACHIEVE_ORB_WHISPERER];
        case CRITERIA_ONCE_BITTEN_TWICE_SHY_10N:
            return !Is25ManDifficulty() && !pSource->HasAuraOfDifficulty(SPELL_ESSENCE_OF_THE_BLOOD_QUEEN);
        case CRITERIA_ONCE_BITTEN_TWICE_SHY_25N:
            return Is25ManDifficulty() && pSource->HasAuraOfDifficulty(SPELL_ESSENCE_OF_THE_BLOOD_QUEEN);
        case CRITERIA_ONCE_BITTEN_TWICE_SHY_10V:
            return !Is25ManDifficulty() && pSource->HasAuraOfDifficulty(SPELL_ESSENCE_OF_THE_BLOOD_QUEEN);
        case CRITERIA_ONCE_BITTEN_TWICE_SHY_25V:
            return Is25ManDifficulty() && pSource->HasAuraOfDifficulty(SPELL_ESSENCE_OF_THE_BLOOD_QUEEN);
            // Valithria Dreamwalker
        case CRITERIA_PORTAL_JOCKEY_10N:
        case CRITERIA_PORTAL_JOCKEY_25N:
        case CRITERIA_PORTAL_JOCKEY_10H:
        case CRITERIA_PORTAL_JOCKEY_25H:
            return m_abAchievement[TYPE_ACHIEVE_PORTAL_JOCKEY];
            // Sindragosa
        case CRITERIA_ALL_YOU_CAN_EAT_10N:
        case CRITERIA_ALL_YOU_CAN_EAT_25N:
        case CRITERIA_ALL_YOU_CAN_EAT_10V:
        case CRITERIA_ALL_YOU_CAN_EAT_25V:
            return m_abAchievement[TYPE_ACHIEVE_ALL_YOU_CAN_EAT];
            // Lich King
        case CRITERIA_BEEN_WAITING_A_LONG_TIME_10N:
        case CRITERIA_BEEN_WAITING_A_LONG_TIME_25N:
        case CRITERIA_BEEN_WAITING_A_LONG_TIME_10H:
        case CRITERIA_BEEN_WAITING_A_LONG_TIME_25H:
            return m_abAchievement[TYPE_ACHIEVE_BEEN_WAITING_A_LONG_TIME];
        default:
            break;
    }
    return false;
}

void instance_icecrown_citadel::CheckSpecialAchievements(IcecrownAchievments uiType)
{
    switch(uiType)
    {
        case TYPE_ACHIEVE_FULL_HOUSE:
        {
            if (Creature* pLadyDeathWhispher = GetSingleCreatureFromStorage(NPC_LADY_DEATHWHISPER))
            {
                SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, true);

                std::list<Creature*> summonEntryList;
                summonEntryList.clear();
                GetCreatureListWithEntryInGrid(summonEntryList, pLadyDeathWhispher, NPC_CULT_ADHERENT, 250.0f);
                if (summonEntryList.empty())
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, false);
                summonEntryList.clear();

                GetCreatureListWithEntryInGrid(summonEntryList, pLadyDeathWhispher, NPC_CULT_FANATIC, 250.0f);
                if (summonEntryList.empty())
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, false);
                summonEntryList.clear();

                GetCreatureListWithEntryInGrid(summonEntryList, pLadyDeathWhispher, NPC_REANIMATED_FANATIC, 250.0f);
                if (summonEntryList.empty())
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, false);

                summonEntryList.clear();
                GetCreatureListWithEntryInGrid(summonEntryList, pLadyDeathWhispher, NPC_REANIMATED_ADHERENT, 250.0f);
                if (summonEntryList.empty())
                   SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, false);

                summonEntryList.clear();
                GetCreatureListWithEntryInGrid(summonEntryList, pLadyDeathWhispher, NPC_DEFORMED_FANATIC, 250.0f);
                if (summonEntryList.empty())
                    SetSpecialAchievementCriteria(TYPE_ACHIEVE_FULL_HOUSE, false);

            }
            break;
        }
        default:
            break;
    }
}

void instance_icecrown_citadel::Load(const char* strIn)
{
    if (!strIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(strIn);

    std::istringstream loadStream(strIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
               >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8]
               >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_icecrown_citadel::Update(uint32 uiDiff)
{
    DialogueUpdate(uiDiff);

    if (m_uiPutricideValveTimer)
    {
        if (m_uiPutricideValveTimer <= uiDiff)
        {
            // Open the pathway to Putricide when the timer expires
            DoUseDoorOrButton(GO_SCIENTIST_DOOR_COLLISION);
            if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_SCIENTIST_DOOR_GREEN))
                pDoor->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            if (GameObject* pDoor = GetSingleGameObjectFromStorage(GO_SCIENTIST_DOOR_ORANGE))
                pDoor->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

            m_uiPutricideValveTimer = 0;
        }
        else
            m_uiPutricideValveTimer -= uiDiff;
    }
}

void instance_icecrown_citadel::JustDidDialogueStep(int32 iEntry)
{
    switch(iEntry)
    {
        case EVENT_START_SAURFANG_INTRO_ALLY:
        case EVENT_START_SAURFANG_INTRO_HORDE:
        {
            if (Player* pPlayer = GetPlayerInMap())
            {
                pPlayer->SummonCreature(m_uiTeam == ALLIANCE ? NPC_MURADIN_BRONZEBEARD_SAURFANG : NPC_OVERLORD_SAURFANG, fSaurfangPositions[2].x, fSaurfangPositions[2].y, fSaurfangPositions[2].z, fSaurfangPositions[2].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                pPlayer->SummonCreature(m_uiTeam == ALLIANCE ? NPC_ALLIANCE_MARINE : NPC_KORKRON_REAVER, fSaurfangPositions[3].x, fSaurfangPositions[3].y, fSaurfangPositions[3].z, fSaurfangPositions[3].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                pPlayer->SummonCreature(m_uiTeam == ALLIANCE ? NPC_ALLIANCE_MARINE : NPC_KORKRON_REAVER, fSaurfangPositions[4].x, fSaurfangPositions[4].y, fSaurfangPositions[4].z, fSaurfangPositions[4].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                pPlayer->SummonCreature(m_uiTeam == ALLIANCE ? NPC_ALLIANCE_MARINE : NPC_KORKRON_REAVER, fSaurfangPositions[5].x, fSaurfangPositions[5].y, fSaurfangPositions[5].z, fSaurfangPositions[5].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                pPlayer->SummonCreature(m_uiTeam == ALLIANCE ? NPC_ALLIANCE_MARINE : NPC_KORKRON_REAVER, fSaurfangPositions[6].x, fSaurfangPositions[6].y, fSaurfangPositions[6].z, fSaurfangPositions[6].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
            }
            DoUseDoorOrButton(GO_SAURFANG_DOOR, 0, false); // Open Saurfang door and move to destination Point
            if (Creature *pSaurfang = GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG))
            {
                pSaurfang->GetMotionMaster()->MovePoint(0, fSaurfangPositions[1].x, fSaurfangPositions[1].y, fSaurfangPositions[1].z);
                pSaurfang->SetRespawnCoord(fSaurfangPositions[1].x, fSaurfangPositions[1].y, fSaurfangPositions[1].z, fSaurfangPositions[1].o);
            }
            break;
        }
        case SAY_SAURFANG_INTRO_ALLY_0:
        case SAY_SAURFANG_INTRO_HORDE_1:
        {
            DoUseDoorOrButton(GO_SAURFANG_DOOR, 0, true); // close Door
            if (Creature* pOverlordSaurfangOrMuradin = GetSingleCreatureFromStorage(m_uiTeam == ALLIANCE ? NPC_MURADIN_BRONZEBEARD_SAURFANG : NPC_OVERLORD_SAURFANG))
                pOverlordSaurfangOrMuradin->GetMotionMaster()->MovePoint(0, fSaurfangPositions[7].x, fSaurfangPositions[7].y, fSaurfangPositions[7].z);

            // move guards
            int8 n = 8;
            for (GuidList::const_iterator itr = m_lSaurfangGuardGuids.begin(); itr != m_lSaurfangGuardGuids.end() && n < 12; ++itr, ++n)
            {
                if (Creature *pGuard = instance->GetCreature(*itr))
                    pGuard->GetMotionMaster()->MovePoint(0, fSaurfangPositions[n].x, fSaurfangPositions[n].y, fSaurfangPositions[n].z);
            }
            break;
        }
        case SAY_SAURFANG_INTRO_ALLY_4:
        case SAY_SAURFANG_INTRO_HORDE_8:
        {
            float x, y, z;
            if (Creature* pSaurfang = GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG))
            {
                pSaurfang->GetPosition(x, y, z);
                pSaurfang->CastSpell(pSaurfang, SPELL_GRIP_OF_AGONY, true);
            }

            if (Creature* pOverlordSaurfangOrMuradin = GetSingleCreatureFromStorage(m_uiTeam == ALLIANCE ? NPC_MURADIN_BRONZEBEARD_SAURFANG : NPC_OVERLORD_SAURFANG))
            {
                pOverlordSaurfangOrMuradin->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
                pOverlordSaurfangOrMuradin->SetWalk(false);
                pOverlordSaurfangOrMuradin->GetMotionMaster()->MovePoint(0, x, y, z);
            }

            // move guards
            for (GuidList::const_iterator i = m_lSaurfangGuardGuids.begin(); i != m_lSaurfangGuardGuids.end(); ++i)
            {
                if (Creature *pGuard = instance->GetCreature(*i))
                {
                    pGuard->SetWalk(false);
                    pGuard->GetMotionMaster()->MovePoint(0, x, y, z);
                    pGuard->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
                }
            }
            break;
        }
        case SAY_SAURFANG_INTRO_ALLY_5:
        case SAY_SAURFANG_INTRO_HORDE_9:
        {
            if (Creature* pOverlordSaurfangOrMuradin = GetSingleCreatureFromStorage(m_uiTeam == ALLIANCE ? NPC_MURADIN_BRONZEBEARD_SAURFANG : NPC_OVERLORD_SAURFANG))
            {
                float x, y, z;
                // npcs float in air
                pOverlordSaurfangOrMuradin->SetWalk(true);
                pOverlordSaurfangOrMuradin->SetSpeedRate(MOVE_WALK, 3.0f);
                pOverlordSaurfangOrMuradin->SetLevitate(true);
                pOverlordSaurfangOrMuradin->GetPosition(x, y, z);
                pOverlordSaurfangOrMuradin->GetMotionMaster()->MovePoint(0, x, y, z + frand(5.0f, 7.0f));
            }

            // move guards
            for (GuidList::iterator i = m_lSaurfangGuardGuids.begin(); i != m_lSaurfangGuardGuids.end(); ++i)
            {
                if (Creature *pGuard = instance->GetCreature(*i))
                {
                    float x, y, z;
                    pGuard->SetWalk(true);
                    pGuard->SetSpeedRate(MOVE_WALK, 3.0f);
                    pGuard->SetLevitate(true);
                    pGuard->GetPosition(x, y, z);
                    pGuard->GetMotionMaster()->MovePoint(0, x, y, z + frand(5.0f, 7.0f));
                }
            }
            break;
        }
        case EVENT_STOP_SAURFANG_INTRO_ALLY:
        case EVENT_STOP_SAURFANG_INTRO_HORDE:
        {
            if (Creature *pSaurfang = GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG))
            {
                pSaurfang->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE);
                SetData(TYPE_DEATHBRINGER_SAURFANG, NOT_STARTED);
            }
            break;
        }
        case EVENT_START_SAURFANG_OUTRO_HORDE:
        {
            if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
            {
                float x, y, z;
                pOverlordSaurfang->HandleEmote(EMOTE_ONESHOT_NONE);
//                pOverlordSaurfang->GetPosition(x, y, z);
  //              pOverlordSaurfang->UpdateAllowedPositionZ(x, y, z);
    //            pOverlordSaurfang->GetMotionMaster()->MovePoint(0, x, y, z);
                pOverlordSaurfang->GetMotionMaster()->MoveFall();
                pOverlordSaurfang->SetLevitate(false);
                pOverlordSaurfang->SetSpeedRate(MOVE_WALK, 1.0f);
                pOverlordSaurfang->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);
                // move guards
                for (GuidList::const_iterator itr = m_lSaurfangGuardGuids.begin(); itr != m_lSaurfangGuardGuids.end(); ++itr)
                {
                    if (Creature* pGuard = instance->GetCreature(*itr))
                    {
                        pGuard->HandleEmote(EMOTE_ONESHOT_NONE);
                        pGuard->SetLevitate(false);
                        pGuard->SetSpeedRate(MOVE_WALK, 1.0f);
                        pGuard->GetMotionMaster()->MoveFollow(pOverlordSaurfang, frand(2.0f, 5.0f), frand(M_PI_F / 2, 1.5f * M_PI_F));
                        pGuard->RemoveAurasDueToSpell(SPELL_GRIP_OF_AGONY);
                    }
                }
            }
            break;
        }
        case SAY_SAURFANG_OUTRO_HORDE_1:
        {
            if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
                pOverlordSaurfang->HandleEmote(EMOTE_ONESHOT_KNEEL);
            break;
        }
        case EVENT_SAURFANG_OUTRO_HORDE_ONE:
        {
            if (Creature* pSaurfang = GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG))
            {
                if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
                {
                    float x, y, z;
                    pSaurfang->GetContactPoint(pOverlordSaurfang, x, y, z, 1.0f);
                    pOverlordSaurfang->GetMotionMaster()->MovePoint(0, x, y, z);
                }
            }
            break;
        }
        case SAY_SAURFANG_OUTRO_HORDE_2:
        {
            if (Creature* pSaurfang = GetSingleCreatureFromStorage(NPC_DEATHBRINGER_SAURFANG))
            {
                if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
                {
                    pOverlordSaurfang->SetFacingToObject(pSaurfang);
                    pOverlordSaurfang->HandleEmote(EMOTE_ONESHOT_KNEEL);
                    pSaurfang->CastSpell(pOverlordSaurfang, SPELL_VEHICLE_HARDCODED, true);
                }
            }
            break;
        }
        case SAY_SAURFANG_OUTRO_HORDE_4:
        {
            if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
                pOverlordSaurfang->GetMotionMaster()->MovePoint(0, fSaurfangPositions[2].x, fSaurfangPositions[2].y, fSaurfangPositions[2].z);
            break;
        }
        case EVENT_STOP_SAURFANG_OUTRO_HORDE:
        {
            if (Creature* pOverlordSaurfang = GetSingleCreatureFromStorage(NPC_OVERLORD_SAURFANG))
                pOverlordSaurfang->ForcedDespawn();
            for (GuidList::const_iterator itr = m_lSaurfangGuardGuids.begin(); itr != m_lSaurfangGuardGuids.end(); ++itr)
            {
                if (Creature *pGuard = instance->GetCreature(*itr))
                    pGuard->ForcedDespawn();
            }
            break;
        }
        // TODO: alliance outro
        default:
            break;
    }
}

InstanceData* GetInstanceData_instance_icecrown_spire(Map* pMap)
{
    return new instance_icecrown_citadel(pMap);
}

bool AreaTrigger_at_icecrown_citadel(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_MARROWGAR_INTRO || pAt->id == AREATRIGGER_DEATHWHISPER_INTRO ||
            pAt->id == AREATRIGGER_SINDRAGOSA_PLATFORM)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        if (instance_icecrown_citadel* pInstance = (instance_icecrown_citadel*)pPlayer->GetInstanceData())
            pInstance->DoHandleCitadelAreaTrigger(pAt->id, pPlayer);
    }

    return false;
}

bool ProcessEventId_event_gameobject_citadel_valve(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (instance_icecrown_citadel* pInstance = (instance_icecrown_citadel*)((Player*)pSource)->GetInstanceData())
        {
            // Note: the Tubes and doors are activated by DB script
            if (pInstance->GetData(TYPE_FESTERGUT) == DONE && pInstance->GetData(TYPE_ROTFACE) == DONE)
                pInstance->DoPreparePutricideDoor();

            return false;
        }
    }
    return false;
}

void AddSC_instance_icecrown_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_citadel";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_spire;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_icecrown_citadel";
    pNewScript->pAreaTrigger = &AreaTrigger_at_icecrown_citadel;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_gameobject_citadel_valve";
    pNewScript->pProcessEventId = &ProcessEventId_event_gameobject_citadel_valve;
    pNewScript->RegisterSelf();
}
