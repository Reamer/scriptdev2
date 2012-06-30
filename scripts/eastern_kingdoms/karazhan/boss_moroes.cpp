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
SDName: Boss_Moroes
SD%Complete: 90
SDComment:
Find better way for remove Garrote spell
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    MAX_GUESTS          = 4,

    SAY_AGGRO           = -1532011,
    SAY_SPECIAL_1       = -1532012,
    SAY_SPECIAL_2       = -1532013,
    SAY_KILL_1          = -1532014,
    SAY_KILL_2          = -1532015,
    SAY_KILL_3          = -1532016,
    SAY_DEATH           = -1532017,

    SPELL_GARROTE       = 37066,
    SPELL_BLIND         = 34694,
    SPELL_GOUGE         = 29425,
    SPELL_FRENZY        = 37023
};

static const float afLocations[4][MAX_GUESTS]=
{
    {-10991.0f, -1884.33f, 81.73f, 0.614315f},
    {-10989.4f, -1885.88f, 81.73f, 0.904913f},
    {-10978.1f, -1887.07f, 81.73f, 2.035550f},
    {-10975.9f, -1885.81f, 81.73f, 2.253890f}
};

static const uint32 auiAdds[6]=
{
    NPC_LADY_KEIRA_BERRYBUCK,
    NPC_LADY_CATRIONA_VON_INDI,
    NPC_LORD_CRISPIN_FERENCE,
    NPC_BARON_RAFE_DREUGER,
    NPC_BARONESS_DOROTHEA_MILLSTIPE,
    NPC_LORD_ROBIN_DARIS
};

struct Guest
{
    ObjectGuid guid;
    uint32 entry;
};

struct MANGOS_DLL_DECL boss_moroesAI : public ScriptedAI
{
    boss_moroesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bFirstTime = true;
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    Guest m_Guest[MAX_GUESTS];

    uint32 m_uiVanishTimer;
    uint32 m_uiBlindTimer;
    uint32 m_uiGougeTimer;
    uint32 m_uiGorroteTimer;

    bool m_bFirstTime;
    bool m_bEnrage;

    void Reset()
    {
        m_uiVanishTimer    = 30000;
        m_uiBlindTimer     = 35000;
        m_uiGougeTimer     = 23000;
        m_uiGorroteTimer      = 0;

        m_bEnrage           = false;

        SpawnAdds();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void EnterEvadeMode()
    {
        if (m_creature->HasAura(SPELL_VANISH))
            return;
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, DONE);

        //remove aura from spell Garrote when Moroes dies
        Map* pMap = m_creature->GetMap();
        if (pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                i->getSource()->RemoveAurasDueToSpell(SPELL_GARROTE);
            }
        }
    }

    void SpawnAdds()
    {
        if (m_bFirstTime)
        {
            std::vector<uint32> vAddList;

            for (uint8 i = 0; i < 6; ++i)
                vAddList.push_back(auiAdds[i]);

            while (vAddList.size() > MAX_GUESTS)
                vAddList.erase((vAddList.begin())+(rand()%vAddList.size()));

            uint8 i = 0;
            for (std::vector<uint32>::iterator itr = vAddList.begin(); itr != vAddList.end(); ++itr, ++i)
            {
                if (Creature* pCreature = m_creature->SummonCreature(*itr, afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_DEAD_DESPAWN, 10000))
                {
                    m_Guest[i].guid = pCreature->GetObjectGuid();
                    m_Guest[i].entry = *itr;
                }
            }

            m_bFirstTime = false;
        }
        else
        {
            for (uint8 i = 0; i < MAX_GUESTS; ++i)
            {
                if (Creature* pGuest = m_creature->GetMap()->GetCreature(m_Guest[i].guid))
                {
                    if (!pGuest->isAlive())              // Exists but is dead
                    {
                        pGuest->Respawn();
                        pGuest->AI()->EnterEvadeMode();
                    }
                    else if (!pGuest->IsInEvadeMode())   // Exists and is alive
                    {
                        pGuest->AI()->EnterEvadeMode();
                    }
                }
                else
                {                                           // Does not exist
                    if (Creature* pCreature = m_creature->SummonCreature(m_Guest[i].entry, afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_DEAD_DESPAWN, 10000))
                        m_Guest[i].guid = pCreature->GetObjectGuid();
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bEnrage && m_creature->GetHealthPercent() < 30.0f)
        {
            DoCastSpellIfCan(m_creature, SPELL_FRENZY);
            m_bEnrage = true;
        }

        if (!m_bEnrage)
        {
            // Cast Vanish, then Garrote random victim
            if (m_uiVanishTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_VANISH) == CAST_OK)
                {
                    m_uiVanishTimer = 30000;
                    m_uiGorroteTimer   = 5000;
                }
            }
            else
                m_uiVanishTimer -= uiDiff;

            if (m_uiGorroteTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pTarget->CastSpell(pTarget, SPELL_GARROTE, true);
                DoScriptText(urand(0, 1) ? SAY_SPECIAL_1 : SAY_SPECIAL_2, m_creature);
                m_uiGorroteTimer = m_uiVanishTimer + 5000;
            }
            else
                m_uiGorroteTimer -= uiDiff;


            //Gouge highest aggro, and attack second highest
            if (m_uiGougeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GOUGE) == CAST_OK)
                    m_uiGougeTimer = 40000;
            }
            else
                m_uiGougeTimer -= uiDiff;

            if (m_uiBlindTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_BLIND, SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_BLIND) == CAST_OK)
                        m_uiBlindTimer = 40000;
                }
            }
            else
                m_uiBlindTimer -= uiDiff;
        }

        if (!m_creature->HasAura(SPELL_VANISH))
            DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_MANABURN      = 29405,
    SPELL_MINDFLY       = 29570,
    SPELL_SWPAIN        = 34441,
    SPELL_SHADOWFORM    = 29406
};

struct MANGOS_DLL_DECL boss_baroness_dorothea_millstipeAI : public ScriptedAI
{
    //Shadow Priest
    boss_baroness_dorothea_millstipeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;
    uint32 m_uiManaBurn_Timer;
    uint32 m_uiMindFlay_Timer;
    uint32 m_uiShadowWordPain_Timer;

    void Reset()
    {
        m_uiManaBurn_Timer       = 7000;
        m_uiMindFlay_Timer       = 1000;
        m_uiShadowWordPain_Timer = 6000;

        DoCastSpellIfCan(m_creature, SPELL_SHADOWFORM, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMindFlay_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MINDFLY) == CAST_OK)
                m_uiMindFlay_Timer = 12000;                     //3sec channeled
        }
        else
            m_uiMindFlay_Timer -= uiDiff;

        if (m_uiManaBurn_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_MANABURN, SELECT_FLAG_POWER_MANA))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_MANABURN) == CAST_OK)
                    m_uiManaBurn_Timer = 5000;                      //3 sec cast
            }
        }
        else
            m_uiManaBurn_Timer -= uiDiff;

        if (m_uiShadowWordPain_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_SWPAIN) == CAST_OK)
                    m_uiShadowWordPain_Timer = 7000;
            }
        }
        else
            m_uiShadowWordPain_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_HAMMEROFJUSTICE       = 13005,
    SPELL_JUDGEMENTOFCOMMAND    = 29386,
    SPELL_SEALOFCOMMAND         = 29385
};

struct MANGOS_DLL_DECL boss_baron_rafe_dreugerAI : public ScriptedAI
{
    //Retr Pally
    boss_baron_rafe_dreugerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;
    uint32 m_uiHammerOfJustice_Timer;
    uint32 m_uiSealOfCommand_Timer;
    uint32 m_uiJudgementOfCommand_Timer;

    void Reset()
    {
        m_uiHammerOfJustice_Timer    = 1000;
        m_uiSealOfCommand_Timer      = 7000;
        m_uiJudgementOfCommand_Timer = m_uiSealOfCommand_Timer + 29000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSealOfCommand_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SEALOFCOMMAND) == CAST_OK)
            {
                m_uiSealOfCommand_Timer      = 32000;
                m_uiJudgementOfCommand_Timer = 29000;
            }
        }
        else
            m_uiSealOfCommand_Timer -= uiDiff;

        if (m_uiJudgementOfCommand_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_JUDGEMENTOFCOMMAND) == CAST_OK)
                m_uiJudgementOfCommand_Timer = m_uiSealOfCommand_Timer + 29000;
        }
        else
            m_uiJudgementOfCommand_Timer -= uiDiff;

        if (m_uiHammerOfJustice_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMMEROFJUSTICE) == CAST_OK)
                m_uiHammerOfJustice_Timer = 12000;
        }
        else
            m_uiHammerOfJustice_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_DISPELMAGIC   = 15090,                            // Self or other guest+Moroes
    SPELL_GREATERHEAL   = 29564,                            // Self or other guest+Moroes
    SPELL_HOLYFIRE      = 29563,
    SPELL_PWSHIELD      = 29408
};

struct MANGOS_DLL_DECL boss_lady_catriona_von_indiAI : public ScriptedAI
{
    //Holy Priest
    boss_lady_catriona_von_indiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;
    uint32 m_uiDispelMagic_Timer;
    uint32 m_uiGreaterHeal_Timer;
    uint32 m_uiHolyFire_Timer;
    uint32 m_uiPowerWordShield_Timer;

    void Reset()
    {
        m_uiDispelMagic_Timer     = 11000;
        m_uiGreaterHeal_Timer     = 1500;
        m_uiHolyFire_Timer        = 5000;
        m_uiPowerWordShield_Timer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPowerWordShield_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_PWSHIELD) == CAST_OK)
                m_uiPowerWordShield_Timer = 15000;
        }
        else
            m_uiPowerWordShield_Timer -= uiDiff;

        if (m_uiGreaterHeal_Timer < uiDiff)
        {
            if (Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetMoroesOrGuestRandom()))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_GREATERHEAL) == CAST_OK)
                    m_uiGreaterHeal_Timer = 17000;
            }
        }
        else
            m_uiGreaterHeal_Timer -= uiDiff;

        if (m_uiHolyFire_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLYFIRE) == CAST_OK)
                m_uiHolyFire_Timer = 22000;
        }
        else
            m_uiHolyFire_Timer -= uiDiff;

        if (m_uiDispelMagic_Timer < uiDiff)
        {
            if (Unit* pTarget = urand(0, 1) ? m_creature->GetMap()->GetCreature(m_pInstance->GetMoroesOrGuestRandom()) : m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_DISPELMAGIC) == CAST_OK)
                    m_uiDispelMagic_Timer = 25000;
            }
        }
        else
            m_uiDispelMagic_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_CLEANSE               = 29380,                    //Self or other guest+Moroes
    SPELL_GREATERBLESSOFMIGHT   = 29381,                    //Self or other guest+Moroes
    SPELL_HOLYLIGHT             = 29562,                    //Self or other guest+Moroes
    SPELL_DIVINESHIELD          = 41367
};

struct MANGOS_DLL_DECL boss_lady_keira_berrybuckAI : public ScriptedAI
{
    //Holy Pally
    boss_lady_keira_berrybuckAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiCleanse_Timer;
    uint32 m_uiGreaterBless_Timer;
    uint32 m_uiHolyLight_Timer;
    uint32 m_uiDivineShield_Timer;

    void Reset()
    {
        m_uiCleanse_Timer      = 13000;
        m_uiGreaterBless_Timer = 1000;
        m_uiHolyLight_Timer    = 7000;
        m_uiDivineShield_Timer = 31000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDivineShield_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_DIVINESHIELD) == CAST_OK)
                m_uiDivineShield_Timer = 31000;
        }
        else
            m_uiDivineShield_Timer -= uiDiff;

        if (m_uiHolyLight_Timer < uiDiff)
        {
            if (Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetMoroesOrGuestRandom()))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_HOLYLIGHT) == CAST_OK)
                    m_uiHolyLight_Timer = 10000;
            }
        }
        else
            m_uiHolyLight_Timer -= uiDiff;

        if (m_uiGreaterBless_Timer < uiDiff)
        {
            if (Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetMoroesOrGuestRandom()))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_GREATERBLESSOFMIGHT) == CAST_OK)
                    m_uiGreaterBless_Timer = 50000;
            }
        }
        else
            m_uiGreaterBless_Timer -= uiDiff;

        if (m_uiCleanse_Timer < uiDiff)
        {
            if (Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetMoroesOrGuestRandom()))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CLEANSE) == CAST_OK)
                    m_uiCleanse_Timer = 10000;
            }
        }
        else
            m_uiCleanse_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_HAMSTRING     = 9080,
    SPELL_MORTALSTRIKE  = 29572,
    SPELL_WHIRLWIND     = 29573
};

struct MANGOS_DLL_DECL boss_lord_robin_darisAI : public ScriptedAI
{
    //Arms Warr
    boss_lord_robin_darisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiHamstring_Timer;
    uint32 m_uiMortalStrike_Timer;
    uint32 m_uiWhirlWind_Timer;

    void Reset()
    {
        m_uiHamstring_Timer    = 7000;
        m_uiMortalStrike_Timer = 10000;
        m_uiWhirlWind_Timer    = 21000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHamstring_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMSTRING) == CAST_OK)
                m_uiHamstring_Timer = 12000;
        }
        else
            m_uiHamstring_Timer -= uiDiff;

        if (m_uiMortalStrike_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTALSTRIKE) == CAST_OK)
                m_uiMortalStrike_Timer = 18000;
        }
        else
            m_uiMortalStrike_Timer -= uiDiff;

        if (m_uiWhirlWind_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND) == CAST_OK)
                m_uiWhirlWind_Timer = 21000;
        }
        else
            m_uiWhirlWind_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_DISARM            = 8379,
    SPELL_HEROICSTRIKE      = 29567,
    SPELL_SHIELDBASH        = 11972,
    SPELL_SHIELDWALL        = 29390
};

struct MANGOS_DLL_DECL boss_lord_crispin_ferenceAI : public ScriptedAI
{
    //Arms Warr
    boss_lord_crispin_ferenceAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (instance_karazhan*)pCreature->GetInstanceData();
        Reset();
    }

    instance_karazhan* m_pInstance;

    uint32 m_uiDisarm_Timer;
    uint32 m_uiHeroicStrike_Timer;
    uint32 m_uiShieldBash_Timer;
    uint32 m_uiShieldWall_Timer;

    void Reset()
    {
        m_uiDisarm_Timer       = 6000;
        m_uiHeroicStrike_Timer = 10000;
        m_uiShieldBash_Timer   = 8000;
        m_uiShieldWall_Timer   = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDisarm_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DISARM) == CAST_OK)
                m_uiDisarm_Timer = 12000;
        }
        else
            m_uiDisarm_Timer -= uiDiff;

        if (m_uiHeroicStrike_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_HEROICSTRIKE) == CAST_OK)
                m_uiHeroicStrike_Timer = 10000;
        }
        else
            m_uiHeroicStrike_Timer -= uiDiff;

        if (m_uiShieldBash_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHIELDBASH) == CAST_OK)
                m_uiShieldBash_Timer = 13000;
        }
        else
            m_uiShieldBash_Timer -= uiDiff;

        if (m_uiShieldWall_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHIELDWALL) == CAST_OK)
                m_uiShieldWall_Timer = 21000;
        }
        else
            m_uiShieldWall_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_moroes(Creature* pCreature)
{
    return new boss_moroesAI(pCreature);
}

CreatureAI* GetAI_baroness_dorothea_millstipe(Creature* pCreature)
{
    return new boss_baroness_dorothea_millstipeAI(pCreature);
}

CreatureAI* GetAI_baron_rafe_dreuger(Creature* pCreature)
{
    return new boss_baron_rafe_dreugerAI(pCreature);
}

CreatureAI* GetAI_lady_catriona_von_indi(Creature* pCreature)
{
    return new boss_lady_catriona_von_indiAI(pCreature);
}

CreatureAI* GetAI_lady_keira_berrybuck(Creature* pCreature)
{
    return new boss_lady_keira_berrybuckAI(pCreature);
}

CreatureAI* GetAI_lord_robin_daris(Creature* pCreature)
{
    return new boss_lord_robin_darisAI(pCreature);
}

CreatureAI* GetAI_lord_crispin_ference(Creature* pCreature)
{
    return new boss_lord_crispin_ferenceAI(pCreature);
}

void AddSC_boss_moroes()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_moroes";
    pNewScript->GetAI = &GetAI_boss_moroes;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_baroness_dorothea_millstipe";
    pNewScript->GetAI = &GetAI_baroness_dorothea_millstipe;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_baron_rafe_dreuger";
    pNewScript->GetAI = &GetAI_baron_rafe_dreuger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_catriona_von_indi";
    pNewScript->GetAI = &GetAI_lady_catriona_von_indi;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_keira_berrybuck";
    pNewScript->GetAI = &GetAI_lady_keira_berrybuck;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_robin_daris";
    pNewScript->GetAI = &GetAI_lord_robin_daris;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_crispin_ference";
    pNewScript->GetAI = &GetAI_lord_crispin_ference;
    pNewScript->RegisterSelf();
}
