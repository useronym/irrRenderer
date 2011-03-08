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

    //teh lights
    irr::scene::IMeshSceneNode* lightSphere= Device->getSceneManager()->addSphereSceneNode(1.0, 12);
    lightSphere->setMaterialType(LightPointMaterial);
    lightSphere->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    lightSphere->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, true);
    lightSphere->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    lightSphere->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);

    irr::scene::IQuadSceneNode* lightQuad= new irr::scene::IQuadSceneNode(0, Device->getSceneManager());
    lightQuad->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);

    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        lightSphere->setMaterialTexture(i, MRTs[i].RenderTexture);
        lightQuad->setMaterialTexture(i, MRTs[i].RenderTexture);
    }

    //ambient
    lightQuad->setMaterialType(LightAmbientMaterial);
    if(Device->getSceneManager()->getAmbientLight().r > 0) lightQuad->render();

    //dynamic lights
    for(irr::u32 i= 0; i < Device->getVideoDriver()->getDynamicLightCount(); i++)
    {
        irr::video::SLight light= Device->getVideoDriver()->getDynamicLight(i);
        irr::core::vector3df camPos= Device->getSceneManager()->getActiveCamera()->getAbsolutePosition();

        //point
        if(light.Type == irr::video::ELT_POINT)
        {
            LightPointCallback->updateConstants(light);

            /*if((camPos - light.Position).getLength() < light.Radius)
            {
                lightQuad->setMaterialType(LightPointMaterial);
                lightQuad->render();
            }
            else
            {*/
                lightSphere->setScale(irr::core::vector3df(light.Radius));
                lightSphere->setPosition(light.Position);
                lightSphere->updateAbsolutePosition();
                lightSphere->render();
            //}
        }
        //directional
        else if(light.Type == irr::video::ELT_DIRECTIONAL)
        {
            LightDirectionalCallback->updateConstants(light);
            lightQuad->setMaterialType(LightDirectionalMaterial);
            lightQuad->render();
        }
    }

    lightQuad->remove();
    lightSphere->remove();
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

void irr::scene::ILightManagerCustom::setLightAmbientMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightAmbientMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightAmbientCallback(irr::video::IShaderAmbientLightCallback* callback)
{
    LightAmbientCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightPointMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightPointMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightPointCallback(irr::video::IShaderPointLightCallback* callback)
{
    LightPointCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightDirectionalMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightDirectionalMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightDirectionalCallback(irr::video::IShaderDirectionalLightCallback* callback)
{
    LightDirectionalCallback= callback;
}
