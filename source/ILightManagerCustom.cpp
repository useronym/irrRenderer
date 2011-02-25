// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "ILightManagerCustom.h"


irr::video::ILightManagerCustom::ILightManagerCustom(irr::scene::ISceneManager* smgr)
{
    Smgr= smgr;
}

irr::video::ILightManagerCustom::~ILightManagerCustom()
{
    //dtor
}

void irr::video::ILightManagerCustom::OnPreRender(irr::core::array<irr::scene::ILightSceneNode*> &lightList)
{

}

void irr::video::ILightManagerCustom::OnPostRender()
{

}

void irr::video::ILightManagerCustom::OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass)
{

}

void irr::video::ILightManagerCustom::OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass)
{

}

void irr::video::ILightManagerCustom::OnNodePreRender(irr::scene::ISceneNode *node)
{

}

void irr::video::ILightManagerCustom::OnNodePostRender(irr::scene::ISceneNode *node)
{

}
