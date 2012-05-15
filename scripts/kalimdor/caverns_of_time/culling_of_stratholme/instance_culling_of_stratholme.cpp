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

enum
{
    MAX_ARTHAS_SPAWN_POS = 5,
    SAY_CHROMIE_HURRY    = -1000000                         // TODO
};

struct sSpawnLocation
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const sSpawnLocation m_aArthasSpawnLocs[] =          // need tuning
{
    {1969.73f, 1287.12f, 145.48f, 3.14f},
    {2049.43f, 1287.43f, 142.75f, 0.06f},
    {2365.54f, 1194.85f, 131.98f, 0.47f},
    {2534.46f, 1125.99f, 130.75f, 0.27f},
    {2363.77f, 1406.31f, 128.64f, 3.23f}
};

static const sSpawnLocation m_aChromieSpawnLocs[] =         // need tuning, escpecially EndPositions!
{
    {1814.46f, 1283.97f, 142.30f, 4.32f},                   // near bridge
    {2311.0f, 1502.4f, 127.9f, 0.0f},                       // End
    {1811.52f, 1285.92f, 142.37f, 4.47f},                   // Hourglass, near bridge
    {2186.42f, 1323.77f, 129.91f, 0.0f},                    // Hourglass, End
};

static const sSpawnLocation m_aUtherAndJainaSpawnLoc[] =
{
    {1761.42f, 1285.75f, 139.945f, 4.93f},                  // Spawning Position for Uther
    {1896.39f, 1292.91f, 143.711f, 0.0f},                   // Spawning Position for Jaina Proudmoore
};

static const sSpawnLocation m_aMalcomAndScruffySpawnLoc[] =
{
    {1603.38f, 805.988f, 123.272f, 1.9f},                   // Spawning Position for Malcom
    {1602.30f, 809.385f, 123.454f, 5.0f},                   // Spawning Position for Scuffy
};

static const sSpawnLocation m_aWaveSpawnLoc[] =
{
    {2193.82f, 1226.140f, 137.40f, 2.3f},                   // Right Spawn Location
    {2224.79f, 1331.750f, 127.16f, 3.5f},                   // Left Spawn Location
};

struct StratholmWave
{
    uint32 m_uiSide;                                        // Stores the side wher mobs spawn
    uint32 m_auiMobEntry[MAX_WAVE_MOB];                     // Stores Creature Entries to be summoned in Waves
};
// Waves that will be summoned
static const StratholmWave aStratholmWaveHero[]=
{
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0}}, // Wave 1
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_ENTAGING_GHOUL, NPC_DARK_NECROMANCER, 0, 0, 0, 0, 0, 0, 0}}, // Wave 2
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_ENTAGING_GHOUL, NPC_DARK_NECROMANCER, NPC_CRYPT_FIEND, 0, 0, 0, 0, 0, 0}}, // Wave 3
    {RIGHT, {NPC_DARK_NECROMANCER, NPC_ACOLYTE, NPC_ACOLYTE, NPC_ACOLYTE, NPC_ACOLYTE, NPC_CRYPT_FIEND, 0, 0, 0, 0}}, // Wave 4
    {RIGHT, {NPC_MEATHOOK, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 5
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_DARK_NECROMANCER, NPC_CRYPT_FIEND, NPC_TOMB_STALKER, 0, 0, 0, 0, 0, 0}}, // Wave 6
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_ENTAGING_GHOUL, NPC_ENTAGING_GHOUL, NPC_PATCHWORK_CONSTRUCT, 0, 0, 0, 0, 0, 0}}, // Wave 7
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_ENTAGING_GHOUL, NPC_DARK_NECROMANCER, NPC_PATCHWORK_CONSTRUCT, 0, 0, 0, 0, 0, 0}}, // Wave 8
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DARK_NECROMANCER, NPC_CRYPT_FIEND, NPC_PATCHWORK_CONSTRUCT, 0, 0, 0, 0, 0, 0}}, // Wave 9
    {LEFT , {NPC_SALRAMM_THE_FLESHCRAFTER, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 10
};

static const StratholmWave aStratholmWaveNormal[]=
{
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 1
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 2
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 3
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 4
    {RIGHT, {NPC_MEATHOOK, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 5
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 6
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 7
    {LEFT , {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 8
    {RIGHT, {NPC_DEVOURING_GHOUL, NPC_DEVOURING_GHOUL, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 9
    {LEFT , {NPC_SALRAMM_THE_FLESHCRAFTER, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, // Wave 10
};

instance_culling_of_stratholme::instance_culling_of_stratholme(Map* pMap) : ScriptedInstance(pMap),
    m_uiGrainCrateCount(0),
    m_uiRemoveCrateStateTimer(0),
    m_uiArthasRespawnTimer(0)
{
    Initialize();
}

void instance_culling_of_stratholme::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_culling_of_stratholme::OnCreatureCreate(Creature* pCreature)
{
    pCreature->SetActiveObjectState(true);
    switch(pCreature->GetEntry())
    {
        case NPC_CHROMIE_ENTRANCE:
        case NPC_CHROMIE_END:
        case NPC_ARTHAS:
        case NPC_JAINA_PROUDMOORE:
        case NPC_UTHER_THE_LIGHTBRINGER:
        case NPC_MICHAEL_BELFAST:
        case NPC_HEARTHSINGER_FORRESTEN:
        case NPC_FRAS_SIABI:
        case NPC_SILVIO_PERELLI:
        case NPC_MARTHA_GOSLIN:
        case NPC_SCRUFFY:
        case NPC_FOOTMAN_JAMES:
        case NPC_MAL_CORRICKS:
        case NPC_GRYAN_STOUTMANTLE:
        case NPC_ROGER_OWENS:
        case NPC_SERGEANT_MORIGAN:
        case NPC_JENA_ANDERSON:
        case NPC_MALCOM_MOORE:
        case NPC_BARTLEBY_BATTSON:
        case NPC_PATRICIA_O_REILLY:
        case NPC_LORDAERON_CRIER:
        case NPC_INFINITE_CORRUPTER:
        case NPC_MALGANIS:
        case NPC_INVIS_STALKER:
        case NPC_CHRONO_LORD_EPOCH:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;

        case NPC_CRATES_BUNNY:                  m_luiCratesBunnyGUIDs.push_back(pCreature->GetObjectGuid()); break;
        case NPC_LORDAERON_FOOTMAN:             m_luiFootmanGUIDs.push_back(pCreature->GetObjectGuid());     break;

        case NPC_STRATHOLME_CITIZEN:
        case NPC_STRATHOLME_RESIDENT:
            if (m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] == DONE)
                pCreature->UpdateEntry(NPC_ZOMBIE);
            else
                m_luiResidentGUIDs.push_back(pCreature->GetObjectGuid());
            break;
        case NPC_AGIATED_STRATHOLME_CITIZEN:    m_lAgiatedCitizenGUIDList.push_back(pCreature->GetObjectGuid());  break;
        case NPC_AGIATED_STRATHOLME_RESIDENT:   m_lAgiatedResidentGUIDList.push_back(pCreature->GetObjectGuid()); break;
    }
}

void instance_culling_of_stratholme::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_BOOKCASE:
            if (m_auiEncounter[TYPE_EPOCH_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_EXIT:
            if (m_auiEncounter[TYPE_MALGANIS_EVENT] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DARK_RUNED_CHEST:
        case GO_DARK_RUNED_CHEST_H:
        case GO_MALGANIS_GATE1:
        case GO_MALGANIS_GATE2:
            break;

        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_culling_of_stratholme::DoChromieHurrySpeech()
{
    if (Creature* pChromie = GetSingleCreatureFromStorage(NPC_CHROMIE_ENTRANCE))
    {
        Map::PlayerList const& players = instance->GetPlayers();
        if (!players.isEmpty())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                    DoScriptText(SAY_CHROMIE_HURRY, pChromie, pPlayer);
            }
        }
    }
}

void instance_culling_of_stratholme::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_GRAIN_EVENT:
            m_auiEncounter[TYPE_GRAIN_EVENT] = uiData;
            if (uiData == SPECIAL)
                DoUpdateWorldState(WORLD_STATE_CRATES, 1);
            else if (uiData == IN_PROGRESS)
            {
                if (m_uiGrainCrateCount >= 5)
                    return;

                ++m_uiGrainCrateCount;
                DoUpdateWorldState(WORLD_STATE_CRATES_COUNT, m_uiGrainCrateCount);

                if (m_uiGrainCrateCount == 5)
                {
                    m_uiRemoveCrateStateTimer = 20000;
                    SetData(TYPE_GRAIN_EVENT, DONE);
                }
            }
            break;
        case TYPE_ARTHAS_INTRO_EVENT:
            m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] = uiData;
            break;
        case TYPE_ARTHAS_ESCORT_EVENT:
            m_auiEncounter[TYPE_ARTHAS_ESCORT_EVENT] = uiData;
            break;
        case TYPE_MEATHOOK_EVENT:
            m_auiEncounter[TYPE_MEATHOOK_EVENT] = uiData;
            if (uiData == DONE)
                SetData(TYPE_SALRAMM_EVENT, IN_PROGRESS);
            break;
        case TYPE_SALRAMM_EVENT:
            m_auiEncounter[TYPE_SALRAMM_EVENT] = uiData;
            if (uiData == DONE)
            {
                DoUpdateWorldState(WORLD_STATE_WAVE, 0);    // Remove WaveCounter
            }
            break;
        case TYPE_EPOCH_EVENT:
            m_auiEncounter[TYPE_EPOCH_EVENT] = uiData;
            break;
        case TYPE_MALGANIS_EVENT:
            m_auiEncounter[TYPE_MALGANIS_EVENT] = uiData;
            if (uiData == DONE)
            {
                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_DARK_RUNED_CHEST : GO_DARK_RUNED_CHEST_H, 30*MINUTE);
                DoSpawnChromieIfNeeded();
            }
            break;
        case TYPE_INFINITE_CORRUPTER_TIME:
            m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] = uiData;
            if (!uiData)
            {
                DoUpdateWorldState(WORLD_STATE_TIME, 0);    // Remove Timer
                DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 0);
            }
            else
                DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, uiData/(MINUTE*IN_MILLISECONDS));
            break;
        case TYPE_INFINITE_CORRUPTER:
            m_auiEncounter[uiType] = uiData;
            switch(uiData)
            {
                case IN_PROGRESS:
                    if (!m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
                        SetData(TYPE_INFINITE_CORRUPTER_TIME, MINUTE*25*IN_MILLISECONDS);
                    DoUpdateWorldState(WORLD_STATE_TIME, 1);// Show Timer
                    break;
                case DONE:
                    SetData(TYPE_INFINITE_CORRUPTER_TIME, 0);
                    break;
                case SPECIAL:
                    DoChromieHurrySpeech();
                    break;
                case FAIL:
                    SetData(TYPE_INFINITE_CORRUPTER_TIME, 0);
                    if (Creature* pCorrupter = GetSingleCreatureFromStorage(NPC_INFINITE_CORRUPTER))
                        if (pCorrupter->isAlive())
                            pCorrupter->ForcedDespawn();
                    break;
            }
            break;
    }

    if (uiData == DONE || (uiType == TYPE_INFINITE_CORRUPTER && uiData == FAIL))
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[TYPE_GRAIN_EVENT] << " " << m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] << " " << m_auiEncounter[TYPE_MEATHOOK_EVENT] << " "
            << m_auiEncounter[TYPE_SALRAMM_EVENT] << " " << m_auiEncounter[TYPE_EPOCH_EVENT] << " " << m_auiEncounter[TYPE_ARTHAS_ESCORT_EVENT] << " "
            << m_auiEncounter[TYPE_MALGANIS_EVENT] << " " << m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] << " " << m_auiEncounter[TYPE_INFINITE_CORRUPTER];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_culling_of_stratholme::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] >> m_auiEncounter[8];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (i != TYPE_INFINITE_CORRUPTER_TIME)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
    }

    // If already started counting down time, the event is "in progress"
    if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
        m_auiEncounter[TYPE_INFINITE_CORRUPTER] = IN_PROGRESS;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_culling_of_stratholme::OnPlayerEnter(Player* pPlayer)
{
    if (instance->GetPlayersCountExceptGMs() == 0)
    {
        DoSpawnArthasIfNeeded();
        DoSpawnChromieIfNeeded();

        // Show World States if needed, TODO verify if needed and if this is the right way
        if (m_auiEncounter[TYPE_GRAIN_EVENT] == IN_PROGRESS || m_auiEncounter[TYPE_GRAIN_EVENT] == SPECIAL)
            DoUpdateWorldState(WORLD_STATE_CRATES, 1);      // Show Crates Counter
        else
            DoUpdateWorldState(WORLD_STATE_CRATES, 0);      // Remove Crates Counter

        if (m_auiEncounter[TYPE_MEATHOOK_EVENT] == IN_PROGRESS)
            DoUpdateWorldState(WORLD_STATE_WAVE, 1);        // Add WaveCounter
        else if (m_auiEncounter[TYPE_SALRAMM_EVENT] == IN_PROGRESS)
            DoUpdateWorldState(WORLD_STATE_WAVE, 6);        // Add WaveCounter
        else
            DoUpdateWorldState(WORLD_STATE_WAVE, 0);        // Remove WaveCounter

        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
            DoUpdateWorldState(WORLD_STATE_TIME, 1);        // Show Timer
        else
            DoUpdateWorldState(WORLD_STATE_TIME, 0);        // Remove Timer
    }
}

uint32 instance_culling_of_stratholme::GetData(uint32 uiType)
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

static bool sortFromEastToWest(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionY() < pSecond->GetPositionY();
}

static bool sortFromSouthToNorth(Creature* pFirst, Creature* pSecond)
{
    return pFirst && pSecond && pFirst->GetPositionX() < pSecond->GetPositionX();
}

void instance_culling_of_stratholme::ConvertCityToDeath()
{
    for (GUIDList::iterator itr = m_luiResidentGUIDs.begin(); itr != m_luiResidentGUIDs.end(); ++itr)
    {
        if (Creature* pCityMan = instance->GetCreature(*itr))
        {
            if (pCityMan->GetEntry() != NPC_ZOMBIE)
                pCityMan->UpdateEntry(NPC_ZOMBIE);
        }
    }
    m_uiZombieConvertTimer = 1000;
}

void instance_culling_of_stratholme::TeleportMalganisAndSpawnIfNeeded(bool entrance)
{
    if (Creature* pMalganis = GetSingleCreatureFromStorage(NPC_MALGANIS))
    {
        if (entrance)
        {
            if (Creature* pStalkerM = GetSingleCreatureFromStorage(NPC_INVIS_STALKER))
            {
                pMalganis->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pMalganis->SetVisibility(VISIBILITY_ON);
                pMalganis->GetMap()->CreatureRelocation(pMalganis, pStalkerM->GetPositionX(), pStalkerM->GetPositionY(), pStalkerM->GetPositionZ(), pStalkerM->GetOrientation());
                pMalganis->MonsterMoveWithSpeed(pStalkerM->GetPositionX(), pStalkerM->GetPositionY(), pStalkerM->GetPositionZ(), 44, false ,true);
                pMalganis->Relocate(pStalkerM->GetPositionX(), pStalkerM->GetPositionY(), pStalkerM->GetPositionZ(), pStalkerM->GetOrientation());
            }
        }
        else
        {
            float x, y, z;
            pMalganis->GetRespawnCoord(x, y, z);
            pMalganis->GetMap()->CreatureRelocation(pMalganis, x, y, z, 1.37f);
            pMalganis->MonsterMoveWithSpeed(x, y, z, 44, false ,true);
            pMalganis->Relocate(x, y, z, 1.37f);
            pMalganis->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pMalganis->SetVisibility(VISIBILITY_ON);
        }
    }
    else
    {
        if (Creature* pArthas = GetSingleCreatureFromStorage(NPC_ARTHAS))
        {
            if (Creature* pMalganis = pArthas->SummonCreature(NPC_MALGANIS, 2296.665f, 1502.362f, 128.362, 1.37f,TEMPSUMMON_DEAD_DESPAWN,29000))
                TeleportMalganisAndSpawnIfNeeded(entrance);
        }
    }
}

void instance_culling_of_stratholme::GetCratesBunnyOrderedList(std::list<Creature*> &lList)
{
    std::list<Creature*> lCratesBunnyList;
    for (GUIDList::const_iterator itr = m_luiCratesBunnyGUIDs.begin(); itr != m_luiCratesBunnyGUIDs.end(); ++itr)
    {
        if (Creature* pBunny = instance->GetCreature(*itr))
            lCratesBunnyList.push_back(pBunny);
    }
    if (lCratesBunnyList.empty())
        return;

    lCratesBunnyList.sort(sortFromEastToWest);
    lList = lCratesBunnyList;
}

Creature* instance_culling_of_stratholme::GetStratIntroFootman()
{
    std::list<Creature*> lFootmanList;
    for (GUIDList::const_iterator itr = m_luiFootmanGUIDs.begin(); itr != m_luiFootmanGUIDs.end(); ++itr)
    {
        if (Creature* pFootman = instance->GetCreature(*itr))
            lFootmanList.push_back(pFootman);
    }

    if (lFootmanList.empty())
        return NULL;
    else
    {
        lFootmanList.sort(sortFromSouthToNorth);
        return *lFootmanList.begin();
    }
}

uint8 instance_culling_of_stratholme::GetInstancePosition()
{
    if (m_auiEncounter[TYPE_MALGANIS_EVENT] == DONE)
        return POS_INSTANCE_FINISHED;
    else if (m_auiEncounter[TYPE_ARTHAS_ESCORT_EVENT] == DONE)
        return POS_ARTHAS_MALGANIS;
    else if (m_auiEncounter[TYPE_EPOCH_EVENT] == DONE)
        return POS_ARTHAS_ESCORTING;
    else if (m_auiEncounter[TYPE_SALRAMM_EVENT] == DONE)
        return POS_ARTHAS_TOWNHALL;
    else if (m_auiEncounter[TYPE_MEATHOOK_EVENT] == DONE)
        return POS_ARTHAS_WAVES;
    else if (m_auiEncounter[TYPE_ARTHAS_INTRO_EVENT] == DONE)
        return POS_ARTHAS_WAVES;
    else if (m_auiEncounter[TYPE_GRAIN_EVENT] == DONE)
        return POS_ARTHAS_INTRO;
    else
        return 0;
}

void instance_culling_of_stratholme::GetResidentOrderedList(std::list<Creature*> &lList)
{
    std::list<Creature*> lResidentList;
    for (GUIDList::const_iterator itr = m_luiResidentGUIDs.begin(); itr != m_luiResidentGUIDs.end(); ++itr)
    {
        if (Creature* pResident = instance->GetCreature(*itr))
            lResidentList.push_back(pResident);
    }
    if (lResidentList.empty())
        return;

    lResidentList.sort(sortFromSouthToNorth);
    lList = lResidentList;
}

void instance_culling_of_stratholme::ArthasJustDied()
{
    m_uiArthasRespawnTimer = 10000;                         // TODO, could be instant
}

void instance_culling_of_stratholme::DoSpawnArthasIfNeeded()
{
    Creature* pArthas = GetSingleCreatureFromStorage(NPC_ARTHAS, true);
    if (pArthas && pArthas->isAlive())
        return;

    uint8 uiPosition = GetInstancePosition();
    if (uiPosition && uiPosition <= MAX_ARTHAS_SPAWN_POS)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_ARTHAS, m_aArthasSpawnLocs[uiPosition-1].m_fX, m_aArthasSpawnLocs[uiPosition-1].m_fY, m_aArthasSpawnLocs[uiPosition-1].m_fZ, m_aArthasSpawnLocs[uiPosition-1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

// Atm here only new Chromies are spawned - despawning depends on Mangos featuring such a thing
// The hourglass also is not yet spawned/ relocated.
void instance_culling_of_stratholme::DoSpawnChromieIfNeeded()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    if (GetInstancePosition() == POS_INSTANCE_FINISHED)
    {
        Creature* pChromie = GetSingleCreatureFromStorage(NPC_CHROMIE_END, true);
        if (!pChromie)
            pPlayer->SummonCreature(NPC_CHROMIE_END, m_aChromieSpawnLocs[1].m_fX, m_aChromieSpawnLocs[1].m_fY, m_aChromieSpawnLocs[1].m_fZ, m_aChromieSpawnLocs[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
    else if (GetInstancePosition() >= POS_ARTHAS_INTRO)
    {
        Creature* pChromie = GetSingleCreatureFromStorage(NPC_CHROMIE_ENTRANCE, true);
        if (!pChromie)
            pPlayer->SummonCreature(NPC_CHROMIE_ENTRANCE, m_aChromieSpawnLocs[0].m_fX, m_aChromieSpawnLocs[0].m_fY, m_aChromieSpawnLocs[0].m_fZ, m_aChromieSpawnLocs[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

void instance_culling_of_stratholme::DoSpawnUtherAndJainaIfNeeded()
{
    Creature* pUther = GetSingleCreatureFromStorage(NPC_UTHER_THE_LIGHTBRINGER, true);
    if (!pUther)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_UTHER_THE_LIGHTBRINGER, m_aUtherAndJainaSpawnLoc[0].m_fX, m_aUtherAndJainaSpawnLoc[0].m_fY, m_aUtherAndJainaSpawnLoc[0].m_fZ, m_aUtherAndJainaSpawnLoc[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }

    Creature* pJainaProudmoore = GetSingleCreatureFromStorage(NPC_JAINA_PROUDMOORE, true);
    if (!pJainaProudmoore)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_JAINA_PROUDMOORE, m_aUtherAndJainaSpawnLoc[1].m_fX, m_aUtherAndJainaSpawnLoc[1].m_fY, m_aUtherAndJainaSpawnLoc[1].m_fZ, m_aUtherAndJainaSpawnLoc[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

void instance_culling_of_stratholme::DoSpawnMalcomAndScruffyIfNeed()
{
    Creature* pMalcom = GetSingleCreatureFromStorage(NPC_MALCOM_MOORE, true);
    if (!pMalcom)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_MALCOM_MOORE, m_aMalcomAndScruffySpawnLoc[0].m_fX, m_aMalcomAndScruffySpawnLoc[0].m_fY, m_aMalcomAndScruffySpawnLoc[0].m_fZ, m_aMalcomAndScruffySpawnLoc[0].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }

    Creature* pScuffy = GetSingleCreatureFromStorage(NPC_SCRUFFY, true);
    if (!pScuffy)
    {
        if (Player* pPlayer = GetPlayerInMap())
            pPlayer->SummonCreature(NPC_SCRUFFY, m_aMalcomAndScruffySpawnLoc[1].m_fX, m_aMalcomAndScruffySpawnLoc[1].m_fY, m_aMalcomAndScruffySpawnLoc[1].m_fZ, m_aMalcomAndScruffySpawnLoc[1].m_fO, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
    }
}

void instance_culling_of_stratholme::Update(uint32 uiDiff)
{
    // 25min Run - decrease time, update worldstate every ~20s
    // as the time is always saved by m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME], there is no need for an extra timer
    if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME])
    {
        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] <= uiDiff)
            SetData(TYPE_INFINITE_CORRUPTER, FAIL);
        else
        {
            m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] -= uiDiff;
            if (m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME]/IN_MILLISECONDS % 20 == 0)
                SetData(TYPE_INFINITE_CORRUPTER_TIME, m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME]);
        }

        // This part is needed for a small "hurry up guys" note, TODO, verify 20min
        if (m_auiEncounter[TYPE_INFINITE_CORRUPTER] == IN_PROGRESS && m_auiEncounter[TYPE_INFINITE_CORRUPTER_TIME] <= 24*MINUTE*IN_MILLISECONDS)
            SetData(TYPE_INFINITE_CORRUPTER, SPECIAL);
    }

    // Small Timer, to remove Grain-Crate WorldState and Spawn Second Chromie
    if (m_uiRemoveCrateStateTimer)
    {
        if (m_uiRemoveCrateStateTimer <= uiDiff)
        {
            DoUpdateWorldState(WORLD_STATE_CRATES, 0);
            DoSpawnChromieIfNeeded();
            m_uiRemoveCrateStateTimer = 0;
        }
        else
            m_uiRemoveCrateStateTimer -= uiDiff;
    }

    // Respawn Arthas after some time
    if (m_uiArthasRespawnTimer)
    {
        if (m_uiArthasRespawnTimer <= uiDiff)
        {
            DoSpawnArthasIfNeeded();
            m_uiArthasRespawnTimer = 0;
        }
        else
            m_uiArthasRespawnTimer -= uiDiff;
    }

    if (m_uiWaveSummonTimer)
    {
        if (m_uiWaveSummonTimer <= uiDiff)
        {
            SummonWave();
            m_uiWaveSummonTimer = 5000; // MINUTE*IN_MILLISECONDS
        }
        else
            m_uiWaveSummonTimer -= uiDiff;
        if (m_uiWaveSummonCounter >= 10)
            m_uiWaveSummonTimer = 0;
    }

    if (m_uiZombieConvertTimer)
    {
        if (m_uiZombieConvertTimer <= uiDiff)
        {
            ConvertCityToDeath();
        }
        else
            m_uiZombieConvertTimer -= uiDiff;
    }

    if (m_uiRogerOwensEvent)
    {
        if (m_uiRogerOwensEvent <= uiDiff)
        {
            m_uiRogerOwensEvent = RogerOwensEvent();
        }
        else
            m_uiRogerOwensEvent -= uiDiff;
    }

    if (m_uiSergeantMoriganEvent)
    {
        if (m_uiSergeantMoriganEvent <= uiDiff)
        {
            m_uiSergeantMoriganEvent = SergeantMoriganEvent();
        }
        else
            m_uiSergeantMoriganEvent -= uiDiff;
    }
    
    if (m_uiJenaAndersonEvent)
    {
        if (m_uiJenaAndersonEvent <= uiDiff)
        {
            m_uiJenaAndersonEvent = JenaAndersonEvent();
        }
        else
            m_uiJenaAndersonEvent -= uiDiff;
    }
    
    if (m_uiMalcomMooreEvent)
    {
        if (m_uiMalcomMooreEvent <= uiDiff)
        {
            m_uiMalcomMooreEvent = MalcomMooreEvent();
        }
        else
            m_uiMalcomMooreEvent -= uiDiff;
    }
    
    if (m_uiBartlebyBattsonEvent)
    {
        if (m_uiBartlebyBattsonEvent <= uiDiff)
        {
            m_uiBartlebyBattsonEvent = BartlebyBattsonEvent();
        }
        else
            m_uiBartlebyBattsonEvent -= uiDiff;
    }
}

void instance_culling_of_stratholme::StartWaveEvent()
{
    m_uiWaveSummonTimer = 1000;
    m_uiWaveSummonCounter = 0;
}

void instance_culling_of_stratholme::SummonWave()
{
    if (Creature* pArthas = GetSingleCreatureFromStorage(NPC_ARTHAS))
    {
        if (pArthas->isAlive())
        {
            StratholmWave const* pWaveData = instance->IsRegularDifficulty() ? &aStratholmWaveNormal[m_uiWaveSummonCounter] : &aStratholmWaveHero[m_uiWaveSummonCounter];

            if (!pWaveData)
            {
                error_log("SD2: StratholmWave not able to obtain wavedata for SummonWave.");
                return;
            }

            for(uint8 i = 0; i < MAX_WAVE_MOB; ++i)
            {
                if (pWaveData->m_auiMobEntry[i] && (pWaveData->m_uiSide == LEFT || pWaveData->m_uiSide == RIGHT))
                {
                    if (Creature* pTemp = pArthas->SummonCreature(pWaveData->m_auiMobEntry[i], m_aWaveSpawnLoc[pWaveData->m_uiSide].m_fX + irand(-3, +3), m_aWaveSpawnLoc[pWaveData->m_uiSide].m_fY + irand(-3, +3), m_aWaveSpawnLoc[pWaveData->m_uiSide].m_fZ, m_aWaveSpawnLoc[pWaveData->m_uiSide].m_fO, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT_OR_DEAD_DESPAWN, 3*MINUTE*IN_MILLISECONDS))
                    {
                        if (Player* pPlayer = GetPlayerInMap(true))
                            pTemp->AI()->AttackStart(pPlayer);
                    }
                }
            }
            m_uiWaveSummonCounter++;
            DoUpdateWorldState(WORLD_STATE_COS_WAVE_COUNT, m_uiWaveSummonCounter);
            //DoScriptText(SAY_MEATHOOK_SPAWN, pMeathook); // Should be done by ACID
            //DoScriptText(SAY_SALRAMM_SPAWN, pSalramm); // Should be done by ACID
        }
    }
}

bool instance_culling_of_stratholme::StartCratesEvent(uint32 npcEntry)
{
    bool bEventAlreadyInProgress = false;
    switch(npcEntry)
    {
        case NPC_ROGER_OWENS:
        {
            if (m_uiRogerOwensEvent == 0)
            {
                m_uiRogerOwensEvent = 6000;
                m_uiRogerOwensEventCounter = 0;
            }
            else
                bEventAlreadyInProgress = true;
            break;
        }
        case NPC_SERGEANT_MORIGAN:
        {
            if (m_uiSergeantMoriganEvent == 0)
            {
                m_uiSergeantMoriganEvent = 6000;
                m_uiSergeantMoriganEventCounter = 0;
            }
            else
                bEventAlreadyInProgress = true;
            break;
        }
        case NPC_JENA_ANDERSON:
        {
            if (m_uiJenaAndersonEvent == 0)
            {
                m_uiJenaAndersonEvent = 6000;
                m_uiJenaAndersonEventCounter = 0;
            }
            else
                bEventAlreadyInProgress = true;
            break;
        }
        case NPC_MALCOM_MOORE:
        {
            // Because NPC are not spawn at begin
            DoSpawnMalcomAndScruffyIfNeed();
            if (m_uiMalcomMooreEvent == 0)
            {
                m_uiMalcomMooreEvent = 6000;
                m_uiMalcomMooreEventCounter = 0;
            }
            else
                bEventAlreadyInProgress = true;
            break;
        }
        case NPC_BARTLEBY_BATTSON:
        {
            if (m_uiBartlebyBattsonEvent == 0)
            {
                m_uiBartlebyBattsonEvent = 6000;
                m_uiBartlebyBattsonEventCounter = 0;
            }
            else
                bEventAlreadyInProgress = true;
        }
        default:
            break;
    }
    return bEventAlreadyInProgress;
}

uint32 instance_culling_of_stratholme::RogerOwensEvent()
{
    uint32 m_uiStepTimer = 1000;
    if (Creature* pRoger = GetSingleCreatureFromStorage(NPC_ROGER_OWENS))
    {
        switch(m_uiRogerOwensEventCounter)
        {
           case 0:
              pRoger->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 7000;
              break;
           case 1:
              pRoger->GetMotionMaster()->MovePoint(0, 1590.055f, 615.727f, 99.795f);
              m_uiStepTimer = 6900;
              break;
           case 2:
              pRoger->GetMotionMaster()->MovePoint(0, 1584.039f, 622.049f, 99.907f);
              m_uiStepTimer = 3900;
              break;
           case 3:
              pRoger->GetMotionMaster()->MovePoint(0, 1578.787f, 623.924f, 99.855f);
              m_uiStepTimer = 2500;
              break;
           case 4:
              DoScriptText(SAY_ROGER01, pRoger);
              m_uiStepTimer = 6000;
              break;
           case 5:
              pRoger->GetMotionMaster()->MovePoint(0, 1579.393f, 624.018f, 99.886f);
              m_uiStepTimer = 2000;
              break;
           case 6:
              DoScriptText(SAY_ROGER02, pRoger);
              m_uiStepTimer = 6000;
              break;
           case 7:
              pRoger->GetMotionMaster()->MovePoint(0, 1579.387f, 623.198f, 99.837f);
              m_uiStepTimer = 1000;
              break;
           case 8:
              DoScriptText(SAY_ROGER03, pRoger);
              m_uiStepTimer = 4000;
              break;
           case 9:
              pRoger->GetMotionMaster()->MovePoint(0, 1575.576f, 619.935f, 99.422f);
              m_uiStepTimer = 2000;
              break;
           case 10:
              pRoger->GetMotionMaster()->MovePoint(0, 1575.833f, 620.471f, 99.466f);
              m_uiStepTimer = 1000;
              break;
           case 11:
              DoScriptText(SAY_ROGER04, pRoger);
              m_uiStepTimer = 6000;
              break;
           case 12:
              pRoger->GetMotionMaster()->MovePoint(0, 1580.215f, 624.368f, 99.924f);
              m_uiStepTimer = 1900;
              break;
           case 13:
              pRoger->GetMotionMaster()->MovePoint(0, 1587.471f, 618.181f, 99.850f);
              m_uiStepTimer = 3900;
              break;
           case 14:
              pRoger->GetMotionMaster()->MovePoint(0, 1592.646f, 590.888f, 99.151f);
              m_uiStepTimer = 13000;
              break;
           case 15:
              pRoger->SetVisibility(VISIBILITY_OFF);
              break;
           default:
               m_uiStepTimer = 0;
               break;
        }
    }
    ++m_uiRogerOwensEventCounter;
    return m_uiStepTimer;
}

uint32 instance_culling_of_stratholme::SergeantMoriganEvent()
{
    uint32 m_uiStepTimer = 1000;
    if (Creature* pSeargeantMorigan = GetSingleCreatureFromStorage(NPC_SERGEANT_MORIGAN))
    {
        switch(m_uiSergeantMoriganEventCounter)
        {
           case 0:
              DoScriptText(SAY_MORIGAN01, pSeargeantMorigan);
              m_uiStepTimer = 6000;
              break;
           case 1:
              if (Creature* pPerelli = GetSingleCreatureFromStorage(NPC_SILVIO_PERELLI))
                 DoScriptText(SAY_PERELLI02, pPerelli);
              m_uiStepTimer = 2000;
              break;
           case 2:
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1564.138f, 668.343f, 102.058f);
              m_uiStepTimer = 2000;
              break;
           case 3:
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1567.956f, 667.776f, 102.094f);
              m_uiStepTimer = 1500;
              break;
           case 4:
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1569.615f, 668.859f, 102.180f);
              m_uiStepTimer = 2000;
              break;
           case 5:
              pSeargeantMorigan->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 1000;
              break;
           case 6:
              DoScriptText(SAY_MORIGAN03, pSeargeantMorigan);
              m_uiStepTimer = 3000;
              break;
           case 7:
              pSeargeantMorigan->SetStandState(UNIT_STAND_STATE_STAND);
              if (Creature* pPerelli = GetSingleCreatureFromStorage(NPC_SILVIO_PERELLI))
              {
                 pSeargeantMorigan->SetGuidValue(UNIT_FIELD_TARGET, pPerelli->GetObjectGuid());
                 pPerelli->SetGuidValue(UNIT_FIELD_TARGET, pSeargeantMorigan->GetObjectGuid());
              }
              m_uiStepTimer = 3000;
              break;
           case 8:
              if (Creature* pPerelli = GetSingleCreatureFromStorage(NPC_SILVIO_PERELLI))
                 DoScriptText(SAY_PERELLI04, pPerelli);
              m_uiStepTimer = 3000;
              break;
           case 9:
              DoScriptText(SAY_MORIGAN05, pSeargeantMorigan);
              m_uiStepTimer = 9000;
              break;
           case 10:
              if (Creature* pPerelli = GetSingleCreatureFromStorage(NPC_SILVIO_PERELLI))
                 DoScriptText(SAY_PERELLI06, pPerelli);
              m_uiStepTimer = 6000;
              break;
           case 11:
              DoScriptText(SAY_MORIGAN07, pSeargeantMorigan);
              m_uiStepTimer = 4000;
              break;
           case 12:
              pSeargeantMorigan->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
              if(Creature* pPerelli = GetSingleCreatureFromStorage(NPC_SILVIO_PERELLI))
                 pPerelli->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1576.119f, 657.675f, 102.09f);
              m_uiStepTimer = 4900;
              break;
           case 13:
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1586.040f, 646.113f, 100.910f);
              m_uiStepTimer = 5900;
              break;
           case 14:
              pSeargeantMorigan->GetMotionMaster()->MovePoint(0, 1609.189f, 697.134f, 106.902f);
              m_uiStepTimer = 24900;
              break;
           case 15:
              pSeargeantMorigan->SetVisibility(VISIBILITY_OFF);
              break;
           default:
               m_uiStepTimer = 0;
               break;
        }
    }
    ++m_uiSergeantMoriganEventCounter;
    return m_uiStepTimer;

}

uint32 instance_culling_of_stratholme::JenaAndersonEvent()
{
    uint32 m_uiStepTimer = 1000;
    if (Creature* pJenaAnderson = GetSingleCreatureFromStorage(NPC_JENA_ANDERSON))
    {
        switch(m_uiJenaAndersonEventCounter)
        {
            case 0:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1605.546f, 744.869f, 114.731f);
                m_uiStepTimer = 1900;
                break;
            case 1:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1614.967f, 743.673f, 114.063f);
                m_uiStepTimer = 4900;
                break;
            case 2:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1623.848f, 729.251f, 112.410f);
                m_uiStepTimer = 6900;
                break;
            case 3:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1633.460f, 726.261f, 113.518f);
                m_uiStepTimer = 4000;
                break;
            case 4:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                    pJenaAnderson->SetGuidValue(UNIT_FIELD_TARGET, pMartha->GetObjectGuid());
                DoScriptText(SAY_JENA01, pJenaAnderson);
                m_uiStepTimer = 3000;
                break;
            case 5:
                if(Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                {
                    pMartha->RemoveAurasDueToSpell(SPELL_USE_WRENCH);
                    pMartha->GetMotionMaster()->MovePoint(0, 1635.918f, 724.357f, 113.561f);
                }
                m_uiStepTimer = 1000;
                break;
            case 6:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                {
                    pMartha->GetMotionMaster()->MovePoint(0, 1636.721f, 725.88f, 113.561f);
                    pMartha->SetGuidValue(UNIT_FIELD_TARGET, pJenaAnderson->GetObjectGuid());
                }
                m_uiStepTimer = 1000;
                break;
            case 7:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                    DoScriptText(SAY_MARTHA02, pMartha);
                m_uiStepTimer = 4000;
                break;
            case 8:
                DoScriptText(SAY_JENA03, pJenaAnderson);
                m_uiStepTimer = 3000;
                break;
            case 9:
                pJenaAnderson->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1629.278f, 727.894f, 112.636f);
                m_uiStepTimer = 1500;
                break;
            case 10:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                {
                    pMartha->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                    pMartha->GetMap()->CreatureRelocation(pMartha, 1640.089f, 725.766f, 113.561f, 4.77f);
                    pMartha->GetMotionMaster()->MovePoint(0,1640.089f, 725.766f, 113.561f);
                }
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1629.452f, 729.416f, 112.712f);
                m_uiStepTimer = 1000;
                break;
            case 11:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                    pMartha->GetMotionMaster()->MovePoint(0, 1640.103f, 725.522f, 113.561f);
                m_uiStepTimer = 500;
                break;
            case 12:
                if(Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                    pMartha->CastSpell(pMartha, SPELL_USE_WRENCH, false);
                pJenaAnderson->SetStandState(UNIT_STAND_STATE_KNEEL);
                m_uiStepTimer = 1500;
                break;
            case 13:
                pJenaAnderson->SetStandState(UNIT_STAND_STATE_STAND);
                m_uiStepTimer = 1000;
                break;
            case 14:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                    pJenaAnderson->SetGuidValue(UNIT_FIELD_TARGET, pMartha->GetObjectGuid());
                DoScriptText(SAY_JENA04, pJenaAnderson);
                m_uiStepTimer = 3000;
                break;
            case 15:
                if (Creature* pMartha = GetSingleCreatureFromStorage(NPC_MARTHA_GOSLIN))
                {
                    pMartha->RemoveAurasDueToSpell(SPELL_USE_WRENCH);
                    DoScriptText(SAY_MARTHA05, pMartha);
                    pMartha->GetMotionMaster()->MovePoint(0, 1638.196f, 726.171f, 113.561f);
                }
                pJenaAnderson->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1615.590f, 719.509f, 110.311f);
                m_uiStepTimer = 2000;
                break;
            case 16:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1596.436f, 670.809f, 103.747f);
                m_uiStepTimer = 6900;
                break;
            case 17:
                pJenaAnderson->GetMotionMaster()->MovePoint(0, 1571.549f, 609.837f, 99.767f);
                m_uiStepTimer = 11000;
                break;
            case 18:
                pJenaAnderson->SetVisibility(VISIBILITY_OFF);
                break;
            default:
                m_uiStepTimer = 0;
                break;
        }
    }
    ++m_uiJenaAndersonEventCounter;
    return m_uiStepTimer;
}

uint32 instance_culling_of_stratholme::MalcomMooreEvent()
{
    uint32 m_uiStepTimer = 1000;
    if (Creature* pMalcomMoore = GetSingleCreatureFromStorage(NPC_MALCOM_MOORE))
    {
        switch(m_uiMalcomMooreEventCounter)
        {
            case 0:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1614.066f, 796.722f, 121.739f);
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1611.459f, 793.274f, 121.928f);
                m_uiStepTimer = 5400;
                break;
            case 1:
                DoScriptText(SAY_MALCOLM01, pMalcomMoore);
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1622.820f, 798.816f, 120.570f);
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1621.467f, 794.323f, 120.323f);
                m_uiStepTimer = 3400;
                break;
            case 2:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1626.574f, 806.781f, 120.270f);
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1629.232f, 803.629f, 120.011f);
                m_uiStepTimer = 3400;
                break;
            case 3:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1622.782f, 808.533f, 121.249f);
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1629.265f, 805.245f, 120.070f);
                m_uiStepTimer = 300;
                break;
            case 4:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->PlayDirectSound(SOUND_ID_DOG_GROWL);
                m_uiStepTimer = 500;
                break;
            case 5:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pMalcomMoore->SetGuidValue(UNIT_FIELD_TARGET, pScruffy->GetObjectGuid());
                DoScriptText(SAY_MALCOLM02, pMalcomMoore);
                m_uiStepTimer = 2000;
                break;
            case 6:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1629.163f, 809.738f, 120.369f);
                m_uiStepTimer = 2000;
                break;
            case 7:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                {
                    pScruffy->HandleEmoteCommand(EMOTE_DOG_HOWL);
                    pScruffy->PlayDirectSound(SOUND_ID_DOG_HOWL);
                }
                m_uiStepTimer = 4000;
                break;
            case 8:
                pMalcomMoore->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1629.922f, 807.799f, 120.122f);
                m_uiStepTimer = 2900;
                break;
            case 9:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1632.169f, 809.851f, 120.047f);
                m_uiStepTimer = 900;
                break;
            case 10:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1630.651f, 811.149f, 120.307f);
                m_uiStepTimer = 800;
                break;
            case 11:
                pMalcomMoore->SetStandState(UNIT_STAND_STATE_KNEEL);
                DoScriptText(SAY_MALCOLM03, pMalcomMoore);
                m_uiStepTimer = 4000;
                break;
            case 12:
                pMalcomMoore->SetStandState(UNIT_STAND_STATE_STAND);
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pMalcomMoore->SetGuidValue(UNIT_FIELD_TARGET, pScruffy->GetObjectGuid());
                DoScriptText(SAY_MALCOLM04, pMalcomMoore);
                m_uiStepTimer = 7000;
                break;
            case 13:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->GetMotionMaster()->MovePoint(0, 1630.692f, 808.011f, 120.083f);
                m_uiStepTimer = 600;
                break;
            case 14:
                if (Creature* pScruffy = GetSingleCreatureFromStorage(NPC_SCRUFFY))
                    pScruffy->SetStandState(UNIT_STAND_STATE_SIT);
                pMalcomMoore->SetGuidValue(UNIT_FIELD_TARGET, ObjectGuid());
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1641.452f, 812.600f, 119.948f);
                m_uiStepTimer = 3900;
                break;
            case 15:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1657.975f, 857.352f, 119.097f);
                m_uiStepTimer = 17900;
                break;
            case 16:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1679.852f, 912.245f, 120.533f);
                m_uiStepTimer = 22900;
                break;
            case 17:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1699.915f, 967.110f, 121.643f);
                m_uiStepTimer = 22900;
                break;
            case 18:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1678.393f, 1026.890f, 125.431f);
                m_uiStepTimer = 24900;
                break;
           case 19:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1678.943f, 1093.130f, 126.899f);
                m_uiStepTimer = 25900;
                break;
           case 20:
                pMalcomMoore->GetMotionMaster()->MovePoint(0, 1700.042f, 1103.880f, 130.872f);
                m_uiStepTimer = 10900;
                break;
           case 21:
                pMalcomMoore->SetVisibility(VISIBILITY_OFF);
                break;
           default:
               m_uiStepTimer = 0;
               break;
        }
    }
    ++m_uiMalcomMooreEventCounter;
    return m_uiStepTimer;

}

uint32 instance_culling_of_stratholme::BartlebyBattsonEvent()
{
    uint32 m_uiStepTimer = 1000;
    if(Creature* pBartlebyBattson = GetSingleCreatureFromStorage(NPC_BARTLEBY_BATTSON))
    {
        switch(m_uiBartlebyBattsonEventCounter)
        {
            case 0:
                DoScriptText(SAY_BARTLEBY03, pBartlebyBattson);
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1672.539f, 872.277f, 120.113f);
                m_uiStepTimer = 1000;
                break;
            case 1:
                pBartlebyBattson->SetStandState(UNIT_STAND_STATE_KNEEL);
                m_uiStepTimer = 3000;
                break;
            case 2:
                DoScriptText(SAY_BARTLEBY04, pBartlebyBattson);
                m_uiStepTimer = 7000;
                break;
            case 3:
                pBartlebyBattson->SetStandState(UNIT_STAND_STATE_STAND);
                m_uiStepTimer = 1000;
                break;
            case 4:
                DoScriptText(SAY_BARTLEBY05, pBartlebyBattson);
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1663.054f, 869.959f, 119.734f);
                m_uiStepTimer = 2900;
                break;
            case 5:
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1640.732f, 812.422f, 119.933f);
                m_uiStepTimer = 23900;
                break;
            case 6:
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1623.704f, 755.741f, 115.710f);
                m_uiStepTimer = 22900;
                break;
            case 7:
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1607.108f, 699.637f, 106.971f);
                m_uiStepTimer = 22900;
                break;
            case 8:
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1587.750f, 646.929f, 100.990f);
                m_uiStepTimer = 20900;
                break;
            case 9:
                pBartlebyBattson->GetMotionMaster()->MovePoint(0, 1571.103f, 660.949f, 102.084f);
                m_uiStepTimer = 10900;
                break;
            case 10:
                pBartlebyBattson->SetVisibility(VISIBILITY_OFF);
                break;
            default:
                m_uiStepTimer = 0;
                break;
        }
    }
    ++m_uiBartlebyBattsonEventCounter;
    return m_uiStepTimer;

}

InstanceData* GetInstanceData_instance_culling_of_stratholme(Map* pMap)
{
    return new instance_culling_of_stratholme(pMap);
}

void AddSC_instance_culling_of_stratholme()
{
    Script *pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_culling_of_stratholme";
    pNewScript->GetInstanceData = &GetInstanceData_instance_culling_of_stratholme;
    pNewScript->RegisterSelf();
}
