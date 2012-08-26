/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Forge Of The Souls
SD%Complete: 90%
SDComment: event script! Need Add 6 Coliseum Champion after devourer death.
SDCategory: forge_of_the_souls
SDAuthor: MaxXx2021 aka Mioka
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

bool GossipHello_npc_jaina_and_sylvana_FSintro(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu( pCreature->GetObjectGuid());

    instance_forge_of_souls* pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
    if (pInstance && pInstance->GetData(TYPE_INTRO) == NOT_STARTED)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_JAINA_BEGIN:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What would you have of me, My Lady?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                break;
            case NPC_SILVANA_BEGIN:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What would you have of me, Banshee Queen?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                break;
            default:
                break;
        }
    }

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetObjectGuid()); //907
    return true;
}

bool GossipSelect_npc_jaina_and_sylvana_FSintro(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    pPlayer->CLOSE_GOSSIP_MENU();
    instance_forge_of_souls* pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
    if (pInstance && pInstance->GetData(TYPE_INTRO) == NOT_STARTED)
        pInstance->SetData(TYPE_INTRO, SPECIAL);
    return true;
}

bool QuestAcceptNPC_npc_jaina_and_sylvana_FSintro(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    instance_forge_of_souls* pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
    if (pInstance && pInstance->GetData(TYPE_INTRO) == NOT_STARTED)
        pInstance->SetData(TYPE_INTRO, SPECIAL);
    return true;
}

bool pQuestRewardedNPC_npc_jaina_and_sylvana_FSextro(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    instance_forge_of_souls* pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
    if (pInstance && pInstance->GetData(TYPE_OUTRO) == NOT_STARTED)
        pInstance->SetData(TYPE_OUTRO, SPECIAL);
    return true;
}

void AddSC_forge_of_souls()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "npc_jaina_and_sylvana_FSintro";
    pNewscript->pGossipHello = &GossipHello_npc_jaina_and_sylvana_FSintro;
    pNewscript->pGossipSelect = &GossipSelect_npc_jaina_and_sylvana_FSintro;
    pNewscript->pQuestAcceptNPC = &QuestAcceptNPC_npc_jaina_and_sylvana_FSintro;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_jaina_and_sylvana_FSextro";
    pNewscript->pQuestRewardedNPC = &pQuestRewardedNPC_npc_jaina_and_sylvana_FSextro;
    pNewscript->RegisterSelf();
}