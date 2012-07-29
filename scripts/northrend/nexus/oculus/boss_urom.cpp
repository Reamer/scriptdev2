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
SDName: Boss_Urom
SD%Complete: 70%
SDComment:
SDAuthor: originally from TC, reworked by MaxXx2021 Aka Mioka, corrected by /dev/rsa
SDCategory: Oculus
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
    SAY_AGGRO                                     = -1578030,
    SAY_KILL_1                                    = -1578031,
    SAY_KILL_2                                    = -1578032,
    SAY_KILL_3                                    = -1578033,
    SAY_DEATH                                     = -1578034,
    SAY_EXPLOSION_1                               = -1578035,
    SAY_EXPLOSION_2                               = -1578036,
    SAY_SUMMON_1                                  = -1578037,
    SAY_SUMMON_2                                  = -1578038,
    SAY_SUMMON_3                                  = -1578039,

    SPELL_ARCANE_BARRIER                          = 53813, //Dummy --> Channeled, shields the caster from damage.

    SPELL_SUMMON_MENAGERIE_1                      = 50476, //Summons an assortment of creatures and teleports the caster to safety.
    SPELL_SUMMON_MENAGERIE_2                      = 50495,
    SPELL_SUMMON_MENAGERIE_3                      = 50496,
    SPELL_TELEPORT                                = 51112, //Teleports to the center of Oculus

    SPELL_TIME_BOMB                               = 51121,
    SPELL_TIME_BOMB_H                             = 59376,
    SPELL_EMPOWERED_ARCANE_EXPLOSION              = 51110,
    SPELL_EMPOWERED_ARCANE_EXPLOSION_H            = 59377,
    SPELL_FROSTBOMB                               = 51103,


    //NPCs
    NPC_PHANTASMAL_MAMMOTH                 = 27642,
    NPC_PHANTASMAL_WOLF                    = 27644,
    NPC_PHANTASMAL_CLOUDSCRAPER            = 27645,
    NPC_PHANTASMAL_OGRE                    = 27647,
    NPC_PHANTASMAL_NAGA                    = 27648,
    NPC_PHANTASMAL_MURLOC                  = 27649,
    NPC_PHANTASMAL_AIR                     = 27650,
    NPC_PHANTASMAL_FIRE                    = 27651,
    NPC_PHANTASMAL_WATER                   = 27653
};

enum UromPhase
{
    PHASE_IDLE	    = 0,
    PHASE_PLATFORM1	= 1,
    PHASE_PLATFORM2	= 2,
    PHASE_CENTER	= 3,
};

struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

static LocationsXY SummonLoc[]=
{
    {1177.469f, 937.721f, 527.405f},
    {968.659f, 1042.53f, 527.320f},
    {1164.02f, 1170.84f, 527.320f},
};

struct TrashPacks
{
    uint32 firstId, secondId, thirdId;
    uint32 id;
};

static TrashPacks PackId[]=
{
    {NPC_PHANTASMAL_CLOUDSCRAPER, NPC_PHANTASMAL_MAMMOTH, NPC_PHANTASMAL_WOLF},
    {NPC_PHANTASMAL_AIR, NPC_PHANTASMAL_FIRE, NPC_PHANTASMAL_WATER},
    {NPC_PHANTASMAL_MURLOC, NPC_PHANTASMAL_NAGA, NPC_PHANTASMAL_OGRE},
};

struct MANGOS_DLL_DECL boss_uromAI : public ScriptedAI
{
    boss_uromAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        DoCast(m_creature, SPELL_ARCANE_BARRIER);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFrostBombTimer;
    uint32 m_uiTimeBombTimer;
    uint32 m_uiExplosionStepTimer;

    uint32 m_uiCheckTimer;

    bool m_bHasCasted;
    bool m_bIsInArcanExplode;

    UromPhase m_UromPhase;
    std::vector<uint32> TrashPacksEntries;

    void Reset()
    {
        TrashPacksEntries.clear();
        SetPacks();

        m_UromPhase             = PHASE_IDLE;
        m_uiExplosionStepTimer  = urand(30000,35000);
        m_uiFrostBombTimer	    = urand(5000,8000);
        m_uiFrostBombTimer      = 9000;
        m_uiTimeBombTimer       = 14000;
        m_uiCheckTimer          = 2000;

        m_bHasCasted            = false;
        m_bIsInArcanExplode     = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if(pWho->GetTypeId() == TYPEID_PLAYER
           && m_creature->IsWithinDistInMap(pWho, 30.0f)
           && !pWho->GetVehicle()
           && m_pInstance->GetData(TYPE_VAROS) == DONE
           && !m_bHasCasted)
        {
            switch(m_UromPhase)
            {
                case PHASE_IDLE:
                    m_bHasCasted = true;
                    m_creature->CastStop();
                    DoScriptText(SAY_SUMMON_1, m_creature);
                    DoCast(m_creature, SPELL_SUMMON_MENAGERIE_1);
                    break;
                case PHASE_PLATFORM1:
                    m_bHasCasted = true;
                    m_creature->CastStop();
                    DoScriptText(SAY_SUMMON_2, m_creature);
                    DoCast(m_creature, SPELL_SUMMON_MENAGERIE_2);
                    break;
                case PHASE_PLATFORM2:
                    m_bHasCasted = true;
                    m_creature->CastStop();
                    DoScriptText(SAY_SUMMON_3, m_creature);
                    DoCast(m_creature, SPELL_SUMMON_MENAGERIE_3);
                    break;
                default:
                    break;
            }
        }
        ScriptedAI::MoveInLineOfSight(pWho);
    }
    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (pDoneBy->GetVehicle())
        {
            EnterEvadeMode();
            return;
        }
        if(m_UromPhase != PHASE_CENTER)
            uiDamage = 0;
    }

    void AttackStart(Unit* pWho)
    {
        if(m_pInstance->GetData(TYPE_VAROS) != DONE)
            return;

        if(m_UromPhase != PHASE_CENTER)
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void SetPacks()
    {
        TrashPacksEntries.reserve(3);
        TrashPacksEntries.push_back(0);
        TrashPacksEntries.push_back(1);
        TrashPacksEntries.push_back(2);

        if(!TrashPacksEntries.empty())
            std::random_shuffle(TrashPacksEntries.begin(), TrashPacksEntries.end());
    }

    void SummonTrashPack(uint32 uiPackId, uint32 uiLocId)
    {
        float angle = (float) rand()*360/RAND_MAX + 1;
        float homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
        float homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
        m_creature->SummonCreature(PackId[uiPackId].firstId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

        angle = (float) rand()*360/RAND_MAX + 1;
        homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
        homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
        m_creature->SummonCreature(PackId[uiPackId].secondId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

        angle = (float) rand()*360/RAND_MAX + 1;
        homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
        homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
        m_creature->SummonCreature(PackId[uiPackId].thirdId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        switch(pSpell->Id)
        {
            case SPELL_SUMMON_MENAGERIE_1:
                m_bHasCasted = false;
                SummonTrashPack(TrashPacksEntries[0], 0);
                m_UromPhase = PHASE_PLATFORM1;
                break;
            case SPELL_SUMMON_MENAGERIE_2:
                m_bHasCasted = false;
                SummonTrashPack(TrashPacksEntries[1], 1);
                m_UromPhase = PHASE_PLATFORM2;
                break;
            case SPELL_SUMMON_MENAGERIE_3:
                m_bHasCasted = false;
                SummonTrashPack(TrashPacksEntries[2], 2);
                m_UromPhase = PHASE_CENTER;
                break;
            case SPELL_TELEPORT:
                DoCast(m_creature, m_bIsRegularMode ? SPELL_EMPOWERED_ARCANE_EXPLOSION : SPELL_EMPOWERED_ARCANE_EXPLOSION_H);
                m_uiExplosionStepTimer = 9000;
                break;
            default:
                break;
        }
    }

    void TeleportBoss(float X, float Y, float Z, float O)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, X, Y, Z, O);
        m_creature->MonsterMoveWithSpeed(X, Y, Z, 44, false ,true);
        m_creature->Relocate(X, Y, Z, O);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, IN_PROGRESS);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_UROM, DONE);
    }

    void KilledUnit(Unit *victim)
    {
        uint8 uiText = urand(0, 2);
        switch (uiText)
        {
           case 0: DoScriptText(SAY_KILL_1, m_creature); break;
           case 1: DoScriptText(SAY_KILL_2, m_creature); break;
           case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void CheckVehicle()
    {
        Map *map = m_creature->GetMap();
        if(map->IsDungeon())
        {
           Map::PlayerList const &PlayerList = map->GetPlayers();

           if(PlayerList.isEmpty())
              return;

           for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
           {
              if(i->getSource()->isAlive() && i->getSource()->GetVehicle())
                 EnterEvadeMode();
           }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_UromPhase == PHASE_CENTER)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if(m_uiCheckTimer < uiDiff)
            {
                CheckVehicle();
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;

            if (!m_bIsInArcanExplode)
            {
                if(m_uiFrostBombTimer < uiDiff)
                {
                    if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_FROSTBOMB) == CAST_OK)
                            m_uiFrostBombTimer = urand(5000, 7000);
                    }
                }
                else
                    m_uiFrostBombTimer -= uiDiff;

                if(m_uiTimeBombTimer < uiDiff)
                {
                    if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, m_bIsRegularMode ? SPELL_TIME_BOMB : SPELL_TIME_BOMB_H, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_TIME_BOMB : SPELL_TIME_BOMB_H) == CAST_OK)
                            m_uiTimeBombTimer = urand(15000, 25000);
                    }
                }
                else
                    m_uiTimeBombTimer -= uiDiff;

                if(m_uiExplosionStepTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK)
                    {
                        DoScriptText(urand(0,1) ? SAY_EXPLOSION_1 : SAY_EXPLOSION_2, m_creature);
                        m_uiExplosionStepTimer = 1500;
                        m_bIsInArcanExplode = true;
                    }
                }
                else
                    m_uiExplosionStepTimer -= uiDiff;
            }
            else
            {
                if (m_uiExplosionStepTimer < uiDiff)
                {
                    if (Unit* pVictim = m_creature->getVictim())
                    {
                        float x, y, z;
                        pVictim->GetPosition(x, y, z);
                        TeleportBoss(x, y, z, m_creature->GetOrientation());
                        m_uiExplosionStepTimer = 30000;
                        m_bIsInArcanExplode = false;
                    }
                }
                else
                    m_uiExplosionStepTimer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_boss_urom(Creature* pCreature)
{
    return new boss_uromAI (pCreature);
}

void AddSC_boss_urom()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_urom";
    newscript->GetAI = &GetAI_boss_urom;
    newscript->RegisterSelf();
}
