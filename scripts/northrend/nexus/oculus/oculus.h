/* Copyright (C) 2010 - 2011 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef DEF_OCULUS_H
#define DEF_OCULUS_H

enum
{
    TYPE_DRAKOS             = 0,
    TYPE_VAROS              = 1,
    TYPE_UROM               = 2,
    TYPE_EREGOS             = 3,
    MAX_ENCOUNTER           = 4,

    MAX_CONSTRUCTS          = 10,

    NPC_ROBOT               = 27641,
    NPC_BALGAR_IMAGE        = 28012,
    NPC_DRAKOS              = 27654,
    NPC_VAROS               = 27447,
    NPC_UROM                = 27655,
    NPC_EREGOS              = 27656,
    NPC_VAROS_CORE          = 28183,
    NPC_AZURE_RING_GUARDIAN = 27638,
    NPC_AZURE_RING_CAPTAIN  = 28236,
    NPC_CENTRIFUGE_CONSTRUCT=27641,

    GO_DRAGON_CAGE_DOOR     = 193995,
    GO_EREGOS_CACHE         = 191349,
    GO_EREGOS_CACHE_H       = 193603,
    GO_SPOTLIGHT            = 191351,
    GO_ORB_OF_NEXUS         = 188715,

    BELGAR_TEXT_0           = 13267,
    BELGAR_TEXT_1           = 13268,
    BELGAR_TEXT_2           = 13269,

    // Varos
    SPELL_SUMMON_BEAM       = 51017,
    SPELL_ARCANE_SHIELD     = 50053,
    SAY_SHIELD_DOWN         = -1578027,
    SAY_VAROS_SPAWN         = -1578029,

    UI_STATE_CONSTRUCT_SHOW	= 3524,
    UI_STATE_CONSTRUCT_COUNT= 3486,

    ACHIEV_MAKE_IT_COUNT	= 18153,
};

struct AzureRingCaptain
{
    ObjectGuid guid;
    uint32 SummonBeamTimer;
};

#define VAROS_MIDDLE_X 1285.348145f
#define VAROS_MIDDLE_Y 1070.302856f
#define VAROS_MIDDLE_Z 439.432007f

class MANGOS_DLL_DECL instance_oculus : public ScriptedInstance
{
    public:
        instance_oculus(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureDeath(Creature* pCreature);

        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        //void SetSpecialAchievementCriteria(uint32 uiType, bool bIsMet);
        //bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);
        
        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        Creature* GetFarestVaromOrb();
        void DoProcessCallDragonEvent();
        void ResetDragons();
        void CheckVarosCentrifugeConstructState();

        void Update(uint32 uiDiff);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        //bool m_abAchievCriteria[MAX_SPECIAL_ACHIEV_CRITS];
        std::string m_strInstData;

        GuidList m_lVarosCoreList;
        uint32 m_uiCentrifugeDead;
        std::list<AzureRingCaptain> m_lAzureRingCaptainList;
};

#endif
