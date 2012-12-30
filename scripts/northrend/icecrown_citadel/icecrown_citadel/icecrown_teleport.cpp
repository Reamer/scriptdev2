/* Copyright (C) 2006 - 2012 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: icecrown_teleport
SD%Complete: 100%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
#include "precompiled.h"
#include "icecrown_citadel.h"

struct t_Locations
{
    int textNum;
    uint32 map_num;
    float x, y, z, o;
    uint32 spellID;
    bool active;
    uint32 encounter;
};

enum
{
    GOSSIP_LIGHTS_HAMMER            = -3631600,
    GOSSIP_ORATORY_OF_THE_DAMNED    = -3631601,
    GOSSIP_RAMPART_OF_SKULLS        = -3631602,
    GOSSIP_DEATHBRINGERS_RISE       = -3631603,
    GOSSIP_ICECROWH_CITADEL         = -3631604,
    GOSSIP_THE_SANCTUM_OF_BLOOD     = -3631605,
    GOSSIP_FROST_QUEENS_LAIR        = -3631606,
    GOSSIP_FROZEN_THRONE            = -3631607,
};

static const t_Locations PortalLoc[]=
{
{GOSSIP_LIGHTS_HAMMER,MAP_NUM,-17.1928f, 2211.44f, 30.1158f,3.14f,70856,true,TYPE_TELEPORT_ALWAYS_ENABLE}, //
{GOSSIP_ORATORY_OF_THE_DAMNED,MAP_NUM,-503.62f, 2211.47f, 62.8235f,3.14f,70856,true,TYPE_MARROWGAR},  //
//{GOSSIP_RAMPART_OF_SKULLS,MAP_NUM,-615.145f, 2211.47f, 199.972f,0,70857,true,TYPE_LADY_DEATHWHISPER}, //
{GOSSIP_DEATHBRINGERS_RISE,MAP_NUM,-549.131f, 2211.29f, 539.291f,0,70858,true,TYPE_LADY_DEATHWHISPER /*TYPE_FLIGHT_WAR*/}, //
//{GOSSIP_ICECROWH_CITADEL,MAP_NUM,4198.42f, 2769.22f, 351.065f,0,70859,true,TYPE_DEATHBRINGER_SAURFANG}, //
{GOSSIP_FROST_QUEENS_LAIR,MAP_NUM,4356.580078f, 2565.75f, 220.401993f,4.90f,70861,true,TYPE_VALITHRIA}, //
{GOSSIP_FROZEN_THRONE,MAP_NUM,528.767273f, -2124.845947f, 1043.1f,3.14f, 70860,true,TYPE_KINGS_OF_ICC}, //
};


bool GOGossipSelect_go_icecrown_teleporter(Player *pPlayer, GameObject* pGo, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return false;

    if(pPlayer->IsInCombat()) 
        return false;

    if(action >= 0 && action < sizeof(PortalLoc)/sizeof(t_Locations))
        pPlayer->TeleportTo(PortalLoc[action].map_num, PortalLoc[action].x, PortalLoc[action].y, PortalLoc[action].z, PortalLoc[action].o);
    //if (PortalLoc[action].spellID != 0 )
      //  pPlayer->_AddAura(PortalLoc[action].spellID, 2000);

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

bool GOGossipHello_go_icecrown_teleporter(Player *pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*) pGo->GetInstanceData();

    if (!pInstance || !pPlayer)
        return false;
    if (pPlayer->isInCombat())
        return true;

    for(uint8 i = 0; i < sizeof(PortalLoc)/sizeof(t_Locations); i++)
    {
        if (PortalLoc[i].active == true && (pInstance->GetData(PortalLoc[i].encounter) == DONE || pPlayer->isGameMaster()))
             pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_TAXI, PortalLoc[i].textNum, GOSSIP_SENDER_MAIN, i);
    }

    pPlayer->SEND_GOSSIP_MENU(TELEPORT_GOSSIP_MESSAGE, pGo->GetObjectGuid());
    return true;
}

bool GOHello_go_plague_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_citadel* pInstance = (instance_icecrown_citadel*)pGo->GetInstanceData();

    if(!pInstance)
        return false;

    if (pInstance->GetData(TYPE_FESTERGUT) == DONE &&
        pInstance->GetData(TYPE_ROTFACE) == DONE)
    {
        pInstance->DoUseDoorOrButton(GO_SCIENTIST_DOOR_ORANGE);
        pInstance->DoUseDoorOrButton(GO_SCIENTIST_DOOR_GREEN);
        pInstance->DoUseDoorOrButton(GO_SCIENTIST_DOOR_COLLISION);
    }
    return true;
}

bool GOHello_go_bloodwing_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_citadel* pInstance = (instance_icecrown_citadel*)pGo->GetInstanceData();

    if(!pInstance)
        return false;

    if (pInstance->GetData(TYPE_DEATHBRINGER_SAURFANG) == DONE)
        pInstance->DoUseDoorOrButton(GO_BLOODWING_DOOR);

    return true;
}

bool GOHello_go_frostwing_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_citadel* pInstance = (instance_icecrown_citadel*)pGo->GetInstanceData();
    if(!pInstance) return false;

    if (pInstance->GetData(TYPE_DEATHBRINGER_SAURFANG) == DONE)
        pInstance->DoUseDoorOrButton(GO_FROSTWING_DOOR);

    return true;
}


void AddSC_icecrown_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_icecrown_teleporter";
    newscript->pGossipHelloGO  = &GOGossipHello_go_icecrown_teleporter;
    newscript->pGossipSelectGO = &GOGossipSelect_go_icecrown_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_plague_sigil";
    newscript->pGOUse  = &GOHello_go_plague_sigil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_bloodwing_sigil";
    newscript->pGOUse  = &GOHello_go_bloodwing_sigil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_frostwing_sigil";
    newscript->pGOUse  = &GOHello_go_frostwing_sigil;
    newscript->RegisterSelf();
}
