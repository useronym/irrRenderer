// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "ILightManagerCustom.h"


irr::scene::ILightManagerCustom::ILightManagerCustom(irr::IrrlichtDevice* device)
{
    Device= device;
    FinalRender= 0;
    FinalRenderToTexture= false;

    //set up light mesh - sphere
    LightSphere= Device->getSceneManager()->addSphereSceneNode(1.0, 12);
    LightSphere->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    LightSphere->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, true);
    LightSphere->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    //LightSphere->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);
    LightSphere->setVisible(false);

    //set up light mesh - cone
    /*LightCone= Device->getSceneManager()->addMeshSceneNode(Device->getSceneManager()->getGeometryCreator()->createConeMesh(1.0, 1.0, 8, irr::video::SColor(0,0,0,0), irr::video::SColor(0,0,0,0)));//Device->getSceneManager()->addSphereSceneNode(1.0, 12);
    LightCone->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    LightCone->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, true);
    LightCone->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    LightCone->setMaterialFlag(irr::video::EMF_ZBUFFER, false);
    LightCone->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);
    LightCone->setVisible(false);*/

    //set up light mesh - quad
    LightQuad= new irr::scene::IQuadSceneNode(0, Device->getSceneManager());
    LightQuad->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    LightQuad->setVisible(false);
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
    if(FinalRenderToTexture || isPostProcessingActive())
    {
        Device->getVideoDriver()->setRenderTarget(FinalRender, true, true, 0);
    }
    else
    {
        Device->getVideoDriver()->setRenderTarget(0);
    }


    //render ambient
    LightQuad->setMaterialType(LightAmbientMaterial);
    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        LightQuad->setMaterialTexture(i, MRTs[i].RenderTexture);
    }
    LightQuad->render(); //also renders nodes with no lighting

    //render dynamic lights
    for(irr::u32 i= 0; i < Device->getVideoDriver()->getDynamicLightCount(); i++)
    {
        irr::video::SLight light= Device->getVideoDriver()->getDynamicLight(i);

        //point
        if(light.Type == irr::video::ELT_POINT)
        {
            LightSphere->setMaterialType(LightPointMaterial);
            LightPointCallback->updateConstants(light);
            LightSphere->setScale(irr::core::vector3df(light.Radius*2.0));
            LightSphere->setPosition(light.Position);
            LightSphere->updateAbsolutePosition();
            LightSphere->render();
        }

        //spot //using sphere instead of a cone as a hack
        else if(light.Type == irr::video::ELT_SPOT)
        {
            /*LightSpotCallback->updateConstants(light);
            LightCone->setScale(irr::core::vector3df(LightSpotCallback->getConeRadius(), light.Radius*1.4, LightSpotCallback->getConeRadius()));
            //we(well, me, really) need to do some more calculations because the cone mesh we created is kinda fucked up by default
            LightCone->setRotation(light.Direction.getHorizontalAngle() + irr::core::vector3df(-90.0, 0.0, 0.0));
            LightCone->setPosition(light.Position + light.Direction*light.Radius);
            //done.. true power of irrlicht!
            LightCone->updateAbsolutePosition();
            LightCone->render();*/

            LightSphere->setMaterialType(LightSpotMaterial);
            LightSpotCallback->updateConstants(light);
            LightSphere->setScale(irr::core::vector3df(light.Radius/2.0));
            LightSphere->setPosition(light.Position + light.Direction * light.Radius * 0.5);
            LightSphere->updateAbsolutePosition();
            LightSphere->render();
        }

        //directional
        else if(light.Type == irr::video::ELT_DIRECTIONAL)
        {
            LightDirectionalCallback->updateConstants(light);
            LightQuad->setMaterialType(LightDirectionalMaterial);
            LightQuad->render();
        }
    }


    //post processing - this is huge mess, nut i understand it (if i try real hard) :)
    if(isPostProcessingActive() && FinalRender)
    {
        LightQuad->setMaterialTexture(0, FinalRender);

        for(irr::u32 i= 0; i < PostProcessingEffectChains.size(); i++)
        {
            if(PostProcessingEffectChains[i]->isActive())
            {
                for(irr::u32 ii= 0; ii < PostProcessingEffectChains[i]->getEffectCount(); ii++)
                {
                    if(i+1 == getActivePostProcessingEffectChainsCount() && ii+1 == PostProcessingEffectChains[i]->getActiveEffectCount() && !FinalRenderToTexture)
                    {
                        Device->getVideoDriver()->setRenderTarget(0);
                    }

                    if(PostProcessingEffectChains[i]->getEffectFromIndex(ii)->isActive())
                    {
                    for(irr::u32 iii= 0; iii < PostProcessingEffectChains[i]->getEffectFromIndex(ii)->getTextureToPassCount(); iii++)
                    {
                        LightQuad->setMaterialTexture(iii+1, PostProcessingEffectChains[i]->getEffectFromIndex(ii)->getTextureToPass(iii));
                    }

                    LightQuad->setMaterialType(PostProcessingEffectChains[i]->getEffectFromIndex(ii)->getMaterialType());
                    LightQuad->render();

                    }
                }
            }
        }
    }
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

    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        LightSphere->setMaterialTexture(i, MRTs[i].RenderTexture);
        //LightCone->setMaterialTexture(i, MRTs[i].RenderTexture);
        LightQuad->setMaterialTexture(i, MRTs[i].RenderTexture);
    }
}

void irr::scene::ILightManagerCustom::addPostProcessingEffectChain(irr::video::CPostProcessingEffectChain* chain)
{
    PostProcessingEffectChains.push_back(chain);
}

irr::u32 irr::scene::ILightManagerCustom::getActivePostProcessingEffectChainsCount()
{
    irr::u32 count= 0;
    for(irr::u32 i= 0; i < PostProcessingEffectChains.size(); i++)
    {
        if(PostProcessingEffectChains[i]->isActive()) count++;
    }
    return count;
}

bool irr::scene::ILightManagerCustom::isPostProcessingActive()
{
    for(irr::u32 i= 0; i < PostProcessingEffectChains.size(); i++)
    {
        if(PostProcessingEffectChains[i]->isActive() && PostProcessingEffectChains[i]->getActiveEffectCount() > 0) return true;
    }
    return false;
}

void irr::scene::ILightManagerCustom::setRenderTexture(irr::video::ITexture* tex)
{
    FinalRender= tex;
}

void irr::scene::ILightManagerCustom::doFinalRenderIntoTexture(bool well)
{
    FinalRenderToTexture= well;
}

bool irr::scene::ILightManagerCustom::getDoFinalRenderToTexture()
{
    return FinalRenderToTexture;
}

irr::video::ITexture* irr::scene::ILightManagerCustom::getRenderTexture()
{
    if(FinalRender) return FinalRender;
    else return 0;
}



void irr::scene::ILightManagerCustom::setLightPointMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightPointMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightPointCallback(irr::video::IShaderPointLightCallback* callback)
{
    LightPointCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightSpotMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightSpotMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightSpotCallback(irr::video::IShaderSpotLightCallback* callback)
{
    LightSpotCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightDirectionalMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightDirectionalMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightDirectionalCallback(irr::video::IShaderDirectionalLightCallback* callback)
{
    LightDirectionalCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightAmbientMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightAmbientMaterial= type;
}

void irr::scene::ILightManagerCustom::setLightAmbientCallback(irr::video::IShaderAmbientLightCallback* callback)
{
    LightAmbientCallback= callback;
}
