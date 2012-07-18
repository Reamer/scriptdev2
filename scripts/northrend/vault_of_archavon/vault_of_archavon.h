/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VAULT_OF_ARCHAVON_H
#define DEF_VAULT_OF_ARCHAVON_H

enum
{
    MAX_ENCOUNTER               = 4,

    TYPE_ARCHAVON               = 0,
    TYPE_EMALON                 = 1,
    TYPE_KORALON                = 2,
    TYPE_TORAVON                = 3,

    NPC_TORAVON                 = 38433,
    NPC_ARCHAVON                = 31125,
    NPC_EMALON                  = 33993,
    NPC_KORALON                 = 35013,
    NPC_TEMPEST_MINION          = 33998,
};

class MANGOS_DLL_DECL instance_vault_of_archavon : public ScriptedInstance
{
    public:
        instance_vault_of_archavon(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnCreatureEnterCombat(Creature * pCreature);
        void OnCreatureEvade(Creature * pCreature);
        void OnCreatureDeath(Creature * pCreature);

        void Update(uint32 uiDiff);

        uint32 GetData(uint32 uiType);
        void SetData(uint32 uiType, uint32 uiData);

        void Load(const char* chrIn);

        bool IsEncounterInProgress() const;
       

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiRespawnEmalonMinion;
        GuidList m_lTempestMinion;
};

#endif
