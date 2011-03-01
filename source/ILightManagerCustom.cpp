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

    //Device->getVideoDriver()->setMaterial(irr::video::SMaterial());
    //Device->getVideoDriver()->setTransform(irr::video::ETS_PROJECTION, irr::core::IdentityMatrix);
    //Device->getVideoDriver()->setTransform(irr::video::ETS_VIEW, irr::core::IdentityMatrix);
    //Device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);

    //point lights
    irr::scene::IMeshSceneNode* lightVolume= Device->getSceneManager()->addSphereSceneNode(1.0, 8);
    lightVolume->setMaterialType(LightPointMaterial);
    lightVolume->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    lightVolume->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, true);
    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        lightVolume->setMaterialTexture(i, MRTs[i].RenderTexture);
    }
    lightVolume->setMaterialTexture(0, MRTs[0].RenderTexture);
    lightVolume->setMaterialTexture(1, MRTs[1].RenderTexture);

    for(irr::u32 i= 0; i < Device->getVideoDriver()->getDynamicLightCount(); i++)
    {
        irr::video::SLight light= Device->getVideoDriver()->getDynamicLight(i);
        if(light.Type == irr::video::ELT_POINT)
        {
            LightPointCallback->updateConstants(light);
            lightVolume->setScale(irr::core::vector3df(light.Radius));
            lightVolume->setPosition(light.Position);
            lightVolume->updateAbsolutePosition();
            lightVolume->render();
        }
    }
    lightVolume->remove();
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

void irr::scene::ILightManagerCustom::setFinalRenderToTexture(bool rtt)
{
    RenderToTexture= rtt;
}

void irr::scene::ILightManagerCustom::setLightPointMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightPointMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightPointCallback(irr::video::IShaderLightCallback* callback)
{
    LightPointCallback= callback;
}
