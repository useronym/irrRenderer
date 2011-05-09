#include "CMaterialSwapper.h"

irr::video::CMaterialSwapper::CMaterialSwapper(irr::scene::ISceneManager* smgr, irr::video::SMaterials* materials)
{
    Smgr= smgr;
    Materials= materials;

    updateEntry(irr::video::EMT_SOLID, Materials->Solid);
    updateEntry(irr::video::EMT_DETAIL_MAP, Materials->DetailMap);
    updateEntry(irr::video::EMT_NORMAL_MAP_SOLID, Materials->NormalAnimated);
}

irr::video::CMaterialSwapper::~CMaterialSwapper()
{
    //dtor
}

void irr::video::CMaterialSwapper::swapMaterials(irr::scene::ISceneNode* node)
{
    if(node)
    {
        swapMaterialsOnNode(node);
        return;
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

void irr::video::CMaterialSwapper::swapMaterialsOnNode(irr::scene::ISceneNode* node)
{
    for(irr::u32 i= 0; i < node->getMaterialCount(); i++)
    {
        for(irr::u32 ii= 0; ii < Entries.size(); ii++)
        {
            if(node->getMaterial(i).MaterialType == Entries[ii]->SwapFrom) node->getMaterial(i).MaterialType= Entries[ii]->SwapTo;
        }
    }
}
