// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "ILightManagerCustom.h"


irr::scene::ILightManagerCustom::ILightManagerCustom(irr::IrrlichtDevice* device)
{
    Device= device;
    FinalRender= 0;
    RenderToTexture= false;
}

irr::scene::ILightManagerCustom::~ILightManagerCustom()
{
    //dtor
}

void irr::scene::ILightManagerCustom::OnPreRender(irr::core::array<irr::scene::ILightSceneNode*> &lightList)
{
    Device->getVideoDriver()->setRenderTarget(MRTs, true, true, 0);
    Device->getLogger()->log(irr::core::stringw(MRTs.size()).c_str());
}

void irr::scene::ILightManagerCustom::OnPostRender()
{
    if(!RenderToTexture)
    {
        Device->getVideoDriver()->setRenderTarget(0);
    }
    else
    {
        Device->getVideoDriver()->setRenderTarget(FinalRender, true, true, 0);
    }

    //light rendering should come here
    Device->getVideoDriver()->draw2DImage(MRTs[0].RenderTexture, irr::core::position2di(0,0));
}

void irr::scene::ILightManagerCustom::OnRenderPassPreRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass)
{

}

void irr::scene::ILightManagerCustom::OnRenderPassPostRender(irr::scene::E_SCENE_NODE_RENDER_PASS renderPass)
{

}

void irr::scene::ILightManagerCustom::OnNodePreRender(irr::scene::ISceneNode *node)
{

}

void irr::scene::ILightManagerCustom::OnNodePostRender(irr::scene::ISceneNode *node)
{

}

void irr::scene::ILightManagerCustom::setMRTs(irr::core::array<irr::video::IRenderTarget> &mrts)
{
    MRTs= mrts;
}

void irr::scene::ILightManagerCustom::setFinalRenderTexture(irr::video::ITexture* tex)
{
    FinalRender= tex;
}

irr::video::ITexture* irr::scene::ILightManagerCustom::getFinalRenderTexture()
{
    if(FinalRender) return FinalRender;
    else return 0;
}

void irr::scene::ILightManagerCustom::setRenderToTexture(bool rtt)
{
    RenderToTexture= rtt;
}
