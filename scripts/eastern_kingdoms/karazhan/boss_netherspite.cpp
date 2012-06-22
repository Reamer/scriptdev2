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
SDName: Boss_Netherspite
SD%Complete: 30%
SDComment: find spell ID for tail swipe added in patch 3.0.2
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    //netherspite spells
    SPELL_NETHERBURN            = 30522,
    SPELL_VOID_ZONE             = 37063,
    SPELL_NETHERBREATH          = 38523,
    SPELL_EMPOWERMENT           = 38549,
    SPELL_NETHER_INFUSION       = 38688,
    SPELL_NETHERSPITE_ROAR      = 38684,
    SPELL_BANISH_VISUAL         = 39833,
    SPELL_ROOT                  = 42716,

    //void zone spells
    SPELL_CONSUMPTION           = 30497,

    //beam buffs
    SPELL_PERSEVERENCE_NS       = 30466,
    SPELL_PERSEVERENCE_PLR      = 30421,
    SPELL_SERENITY_NS           = 30467,
    SPELL_SERENITY_PLR          = 30422,
    SPELL_DOMINANCE_NS          = 30468,
    SPELL_DOMINANCE_PLR         = 30423,

    //beam debuffs
    SPELL_EXHAUSTION_DOM        = 38639,
    SPELL_EXHAUSTION_SER        = 38638,
    SPELL_EXHAUSTION_PER        = 38637,

    //beam spells
    SPELL_BEAM_DOM              = 30402,
    SPELL_BEAM_SER              = 30401,
    SPELL_BEAM_PER              = 30400,
    SPELL_BEAM_RED              = 30465,
    SPELL_BEAM_GREEN            = 30464,
    SPELL_BEAM_BLUE             = 30463,
    SPELL_BLUE_PORTAL           = 30491,
    SPELL_GREEN_PORTAL          = 30490,
    SPELL_RED_PORTAL            = 30487,

    //emotes
    EMOTE_PHASE_BEAM            = -1532089,
    EMOTE_PHASE_BANISH          = -1532090,

    //npcs
    NPC_PORTAL_RED              = 17369,
    NPC_PORTAL_GREEN            = 17367,
    NPC_PORTAL_BLUE             = 17368,
    NPC_VOID_ZONE               = 16697
};

struct SpawnLocation
{
    float x, y, z;
};

enum Netherspite_Portal{
    RED_PORTAL = 0, // Perseverence
    GREEN_PORTAL = 1, // Serenity
    BLUE_PORTAL = 2 // Dominance
};

struct NetherspidePortal
{
    uint8  color;
    uint32 entry;
    uint32 SpellIdVisual;
    uint32 SpellIdBeam;
    uint32 SpellIdPlayerBuff;
    uint32 SpellIdNetherspiteBuff;
    uint32 SpellIdPlayerDebuff;
};

const static NetherspidePortal Portals[3] =
{
    {RED_PORTAL, NPC_PORTAL_RED, SPELL_RED_PORTAL, SPELL_BEAM_RED, SPELL_PERSEVERENCE_PLR, SPELL_PERSEVERENCE_NS,  SPELL_EXHAUSTION_PER},
    {GREEN_PORTAL, NPC_PORTAL_GREEN, SPELL_GREEN_PORTAL, SPELL_BEAM_GREEN, SPELL_SERENITY_PLR, SPELL_SERENITY_NS, SPELL_EXHAUSTION_SER},
    {BLUE_PORTAL, NPC_PORTAL_GREEN, SPELL_BLUE_PORTAL, SPELL_BEAM_BLUE, SPELL_DOMINANCE_PLR, SPELL_DOMINANCE_NS, SPELL_EXHAUSTION_DOM}
};

// at first spawn portals got fixed coords, should be shuffled in subsequent beam phases
static SpawnLocation PortalCoordinates[] =
{
    {-11105.508789f, -1600.851685f, 279.950256f},
    {-11195.353516f, -1613.237183f, 278.237258f},
    {-11137.846680f, -1685.607422f, 278.239258f}
};

enum NetherspitePhases
{
    BEAM_PHASE   = 0,
    BANISH_PHASE = 1,
};

struct MANGOS_DLL_DECL boss_netherspiteAI : public ScriptedAI
{
    boss_netherspiteAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    bool m_bIsEnraged;
    NetherspitePhases m_ActivePhase;

    GUIDList m_Portals;

    uint32 m_uiEnrageTimer;
    uint32 m_uiVoidZoneTimer;
    uint32 m_uiPhaseSwitchTimer;
    uint32 m_uiNetherbreathTimer;

    void Reset()
    {
        m_bIsEnraged    = false;
        m_ActivePhase = BEAM_PHASE;

        m_uiEnrageTimer       = MINUTE*9*IN_MILLISECONDS;
        m_uiVoidZoneTimer     = 15000;
        m_uiPhaseSwitchTimer  = MINUTE*IN_MILLISECONDS;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NETHERSPITE, IN_PROGRESS);

        DoCastSpellIfCan(m_creature, SPELL_NETHERBURN);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NETHERSPITE, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NETHERSPITE, FAIL);
    }

    void SummonPortals()
    {
        uint8 r = rand()%4;
        uint8 pos[3];
        pos[RED_PORTAL] = ((r % 2) ? (r > 1 ? 2 : 1) : 0);
        pos[GREEN_PORTAL] = ((r % 2) ? 0 : (r > 1 ? 2 : 1));
        pos[BLUE_PORTAL] = (r > 1 ? 1 : 2); // Blue Portal not on the left side (0)

        for (int i=0; i<3; ++i)
        {
            if (Creature* pPortal = m_creature->SummonCreature(Portals[i].entry, PortalCoordinates[pos[i]].x, PortalCoordinates[pos[i]].y, PortalCoordinates[pos[i]].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
            {
                m_Portals.push_back(pPortal->GetObjectGuid());
                pPortal->SetFacingToObject(m_creature);
            }
        }
    }

    void SwitchPhases()
    {
        if (m_ActivePhase == BEAM_PHASE)
        {
            m_ActivePhase = BANISH_PHASE;
            DoScriptText(EMOTE_PHASE_BANISH, m_creature);

            m_uiNetherbreathTimer = 500;
            m_uiPhaseSwitchTimer  = 30*IN_MILLISECONDS;
            m_Portals.clear();
        }
        else
        {
            m_ActivePhase = BEAM_PHASE;
            DoScriptText(EMOTE_PHASE_BEAM, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_NETHERSPITE_ROAR);
            SummonPortals();

            m_uiPhaseSwitchTimer = MINUTE*IN_MILLISECONDS;
        }

        //reset threat every phase switch
        DoResetThreat();
    }

    void UpdateBeam(const uint32 uiDiff)
    {
        for (GUIDList::const_iterator iter = m_Portals.begin(); iter != m_Portals.end(); ++iter)
        {
            if (Creature* pPortal = m_creature->GetMap()->GetCreature(*iter))
            {
                float distanceToNP = pPortal->GetDistance(m_creature);
                float AngleToNP = pPortal->GetAngle(m_creature);
                Map::PlayerList const& lPlayers = m_pInstance->instance->GetPlayers();

                std::list<Player*> possiblePlayersInArcAndDistance;
                for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if (pPortal->GetDistance(itr->getSource()) > distanceToNP)
                        continue;

                    if (pPortal->GetAngle(itr->getSource()) - AngleToNP < M_PI_F/12)
                    {

                    }


                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPhaseSwitchTimer <= uiDiff)
            SwitchPhases();
        else
            m_uiPhaseSwitchTimer -= uiDiff;

        if (!m_bIsEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_NETHER_INFUSION, CAST_FORCE_CAST);
                m_bIsEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        if (m_ActivePhase == BEAM_PHASE)
        {
            if (m_uiVoidZoneTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_VOID_ZONE, true);

                m_uiVoidZoneTimer = 15000;
            }
            else
                m_uiVoidZoneTimer -= uiDiff;

            UpdateBeam(uiDiff);
        }
        else
        {
            if (m_uiNetherbreathTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_NETHERBREATH);

                m_uiNetherbreathTimer = urand(4000, 5000);
            }
            else
                m_uiNetherbreathTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_netherspite(Creature* pCreature)
{
    return new boss_netherspiteAI(pCreature);
}

void AddSC_boss_netherspite()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_netherspite";
    pNewScript->GetAI = &GetAI_boss_netherspite;
    pNewScript->RegisterSelf();
}
