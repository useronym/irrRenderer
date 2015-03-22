// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CMaterialSwapper.h"

irr::video::CMaterialSwapper::CMaterialSwapper(irr::scene::ISceneManager* smgr, irr::video::SMaterials* materials)
    :Smgr(smgr),
    Materials(materials)
{
    updateEntry(irr::video::EMT_SOLID, Materials->Solid);
    updateEntry(irr::video::EMT_DETAIL_MAP, Materials->DetailMap);
    updateEntry(irr::video::EMT_NORMAL_MAP_SOLID, Materials->Normal);
    updateEntry(irr::video::EMT_PARALLAX_MAP_SOLID, Materials->Parallax);
    updateEntry(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF, Materials->TransparentRef);
    updateEntry(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL, Materials->Transparent);
}

irr::video::CMaterialSwapper::~CMaterialSwapper()
{
    for(irr::u32 i= 0; i < Entries.size(); i++)
    {
        delete Entries[i];
    }

    Entries.clear();
}

void irr::video::CMaterialSwapper::swapMaterials(irr::scene::ISceneNode* node) const
{
    if(node)
    {
        swapMaterialsOnNode(node);
    }
    else
    {
        irr::core::array<irr::scene::ISceneNode*> nodes;
        Smgr->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes);
        for(irr::u32 i= 0; i < nodes.size(); i++)
        {
            swapMaterialsOnNode(nodes[i]);
        }
    }
}

void irr::video::CMaterialSwapper::updateEntry(irr::video::E_MATERIAL_TYPE swapFrom, irr::video::E_MATERIAL_TYPE swapTo)
{
    //first, try to find and replace an existing entry
    for(irr::u32 i= 0; i < Entries.size(); i++)
    {
        if(Entries[i]->SwapFrom == swapFrom)
        {
            Entries[i]->SwapTo= swapTo;
            return;
        }
    }

    //if not, create a new entry
    irr::video::SMaterialSwapperEntry* entry= new irr::video::SMaterialSwapperEntry;
    entry->SwapFrom= swapFrom;
    entry->SwapTo= swapTo;
    Entries.push_back(entry);
}

void irr::video::CMaterialSwapper::removeEntry(irr::video::E_MATERIAL_TYPE swapFrom)
{
    for(irr::u32 i= 0; i < Entries.size(); i++)
    {
        if(Entries[i]->SwapFrom == swapFrom)
        {
            delete Entries[i];
            Entries.erase(i);
            return;
        }
    }
}

// TODO (entity#1#): optimize
void irr::video::CMaterialSwapper::swapMaterialsOnNode(irr::scene::ISceneNode* node) const
{
    for(irr::u32 i= 0; i < node->getMaterialCount(); i++)
    {
        for(irr::u32 ii= 0; ii < Entries.size(); ii++)
        {
            if(node->getMaterial(i).MaterialType == Entries[ii]->SwapFrom) node->getMaterial(i).MaterialType= Entries[ii]->SwapTo;
        }
    }
}
