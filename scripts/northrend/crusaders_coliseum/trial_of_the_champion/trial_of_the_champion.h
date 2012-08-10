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

#ifndef DEF_TOC_H
#define DEF_TOC_H

enum
{
    MAX_ENCOUNTER               = 3,

    TYPE_GRAND_CHAMPIONS        = 0,
    TYPE_ARGENT_CHALLENGE       = 1,
    TYPE_BLACK_KNIGHT           = 2,

    // announcer
    NPC_ARELAS_BRIGHTSTAR       = 35005,    // alliance
    NPC_JAEREN_SUNSWORN         = 35004,    // horde

    //Horses
    NPC_ARGENT_WARHORSE_1       = 35644,    // hostile to horde
    NPC_ARGENT_WARHORSE_2       = 36557,    // friendly to alliance
    NPC_ARGENT_BATTLEWORG_1     = 36558,    // hostile to alliance
    NPC_ARGENT_BATTLEWORG_2     = 36559,    // friendly to horde


    DATA_CHAMPION_1             = 3,
    DATA_CHAMPION_2             = 4,
    DATA_CHAMPION_3             = 5,
    DATA_BLACK_KNIGHT           = 6,
    DATA_BLACK_KNIGHT_MINION    = 7,
    DATA_TOC5_ANNOUNCER         = 8,
    DATA_JAEREN                 = 9,
    DATA_ARELAS                 = 10,
    DATA_CHAMPIONID_1           = 11,
    DATA_CHAMPIONID_2           = 12,
    DATA_CHAMPIONID_3           = 13,
    DATA_MEMORY                 = 14,
    DATA_ARGENT_CHALLENGER      = 15,
    DATA_CHAMPIONS_COUNT        = 16,

    // grand champions
    // alliance
    NPC_JACOB_ALERIUS           = 34705,
    NPC_AMBROSE_BOLTSPARK       = 34702,
    NPC_COLOSOS                 = 34701,
    NPC_JAELYNE_EVENSONG        = 34657,
    NPC_LANA_STOUTHAMMER        = 34703,
    // other alliance
    NPC_STORMWIND_CHAMPION      = 35328,    // humans
    NPC_IRONFORGE_CHAMPION      = 35329,    // dwarfs
    NPC_GNOMEREGAN_CHAMPION     = 35331,    // gnom
    NPC_EXODAR_CHAMPION         = 35330,    // draeni
    NPC_DARNASSUS_CHAMPION      = 35332,    // night elfs
    // horde 
    NPC_MOKRA_THE_SKULLCRUSHER  = 35572,
    NPC_ERESSEA_DAWNSINGER      = 35569,
    NPC_RUNOK_WILDMANE          = 35571,
    NPC_ZULTORE                 = 35570,
    NPC_DEATHSTALKER_VISCERI    = 35617,
    // other horde
    NPC_ORGRIMMAR_CHAMPION      = 35314,    // orks
    NPC_SENJIN_CHAMPION         = 35323,    // trolls
    NPC_SILVERMOON_CHAMPION     = 35326,    // blood elf
    NPC_THUNDER_BLUFF_CHAMPION  = 35325,    // taure
    NPC_UNDERCITY_CHAMPION      = 35327,    // undead

    // second encounter
    NPC_EADRIC                  = 35119,
    NPC_PALETRESS               = 34928,
    // escort for seconds encounter
    NPC_ARGENT_PRIESTESS        = 35307,
    NPC_ARGENT_MONK             = 35305,
    NPC_ARGENT_LIGHTWIELDER     = 35309,

    // memories
    MEMORY_ALGALON              = 35052,
    MEMORY_ARCHIMONDE           = 35041,
    MEMORY_CHROMAGGUS           = 35033,
    MEMORY_CYANIGOSA            = 35046,
    MEMORY_DELRISSA             = 35043,
    MEMORY_ECK                  = 35047,
    MEMORY_ENTROPIUS            = 35044,
    MEMORY_GRUUL                = 35039,
    MEMORY_HAKKAR               = 35034,
    MEMORY_HEIGAN               = 35049,
    MEMORY_HEROD                = 35030,
    MEMORY_HOGGER               = 34942,
    MEMORY_IGNIS                = 35050,
    MEMORY_ILLIDAN              = 35042,
    MEMORY_INGVAR               = 35045,
    MEMORY_KALITHRESH           = 35037,
    MEMORY_LUCIFRON             = 35031,
    MEMORY_MALCHEZAAR           = 35038,
    MEMORY_MUTANUS              = 35029,
    MEMORY_ONYXIA               = 35048,
    MEMORY_THUNDERAAN           = 35032,
    MEMORY_VANCLEEF             = 35028,
    MEMORY_VASHJ                = 35040,
    MEMORY_VEKNILASH            = 35036,
    MEMORY_VEZAX                = 35051,

    // third encounter
    NPC_BLACK_KNIGHT            = 35451,
    NPC_BLACK_KNIGHT_GRYPHON    = 35491,
    NPC_RISEN_JAEREN            = 35545,
    NPC_RISEN_ARELAS            = 35564,

    GO_CHAMPIONS_LOOT           = 195709,
    GO_CHAMPIONS_LOOT_H         = 195710,
    GO_EADRIC_LOOT              = 195374,
    GO_EADRIC_LOOT_H            = 195375,
    GO_PALETRESS_LOOT           = 195323,
    GO_PALETRESS_LOOT_H         = 195324,

    DESPAWN_TIME                = 300000,

    MAX_SPECIAL_ACHIEV_CRITS    = 3,
    /*
    Argent Confessor
    I've Had Worse
    The Faceroller
    */
};

enum PhaseControl
{
    HORDE_CONTROL_PHASE_SHIFT_1    = 55773,
    HORDE_CONTROL_PHASE_SHIFT_2    = 60028,
    ALLIANCE_CONTROL_PHASE_SHIFT_1 = 55774,
    ALLIANCE_CONTROL_PHASE_SHIFT_2 = 60027,
};

class MANGOS_DLL_DECL instance_trial_of_the_champion : public ScriptedInstance, private DialogueHelper
{
    public:
        instance_trial_of_the_champion(Map* pMap);

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        //void OnCreatureEnterCombat(Creature* pCreature);
        //void OnCreatureEvade(Creature* pCreature);
        //void OnCreatureDeath(Creature* pCreature);

        void OnPlayerEnter(Player *pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);
        void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void Update(uint32 uiDiff) { DialogueUpdate(uiDiff); };

    private:
        void JustDidDialogueStep(int32 iEntry);

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        Team m_uiTeam;

        bool m_bAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];

        uint32 m_uiChampionId1;
        uint32 m_uiChampionId2;
        uint32 m_uiChampionId3;
        uint32 m_uiChampionsCount;
        uint32 m_uiArgentChallengerID;
        uint32 m_uiBlackKnightMinionID;
        uint32 m_uiAnnouncerID;
       
};
#endif

/*
SOME TEXT'S
entry, text, type, language, probability, emote, duration, sound, comment
(35005, 'The next combatant is second to none in her passion for upholding the Light. I give you Argent Confessor Paletress!', 14, 0, 100, 0, 0, 15882, 'Arelas Brightstar to Argent Confessor Paletress'),
(35005, 'What''s that, up near the rafters?', 12, 0, 100, 25, 0, 0, 'Arelas Brightstar'),
(35451, 'You spoiled my grand entrance, rat.', 12, 0, 100, 0, 0, 16256, 'The Black Knight'),
(35451, 'Did you honestly think an agent of the Lich King would be bested on the field of your pathetic little tournament?', 12, 0, 100, 396, 0, 16257, 'The Black Knight'),
(35451, 'I''ve come to finish my task.', 12, 0, 100, 396, 0, 16258, 'The Black Knight'),
(35451, 'This farce ends here!', 14, 0, 100, 0, 0, 16259, 'The Black Knight'),
(35451, 'My rotting flesh was just getting in the way!', 14, 0, 100, 0, 0, 16262, 'The Black Knight'),
(35451, 'I have no need for bones to best you!', 14, 0, 100, 0, 0, 16263, 'The Black Knight'),
(35451, 'No! I must not fail... again...', 14, 0, 100, 0, 0, 8574, 'The Black Knight'),

(34928, 'Thank you, good herald. Your words are too kind.', 12, 0, 100, 2, 0, 16245, 'Argent Confessor Paletress to Arelas Brightstar'),
(34928, 'May the Light give me strength to provide a worthy challenge.', 12, 0, 100, 16, 0, 16246, 'Argent Confessor Paletress to Arelas Brightstar'),
(34928, 'Well, then. Let us begin.', 14, 0, 100, 0, 0, 16247, 'Argent Confessor Paletress'),
(34928, 'Take this time to consider your past deeds.', 14, 0, 100, 0, 0, 16248, 'Argent Confessor Paletress'),
(34928, 'Even the darkest memory fades when confronted!', 14, 0, 100, 0, 0, 16249, 'Argent Confessor Paletress to Memory of Lucifron'),
(34928, 'Excellent work!', 14, 0, 100, 0, 0, 16252, 'Argent Confessor Paletress'),
*/
