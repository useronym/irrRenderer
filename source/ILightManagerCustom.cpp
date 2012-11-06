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
    //decide render targets
    if(!getPostProcessingActive() && !getDoFinalRenderToTexture())
    {
        Device->getVideoDriver()->setRenderTarget(0);
    }
    else if(!getPostProcessingActive() && getDoFinalRenderToTexture())
    {
        Device->getVideoDriver()->setRenderTarget(FinalRender, true, true, 0);
    }
    else if(getPostProcessingActive())
    {
        Device->getVideoDriver()->setRenderTarget(PPTex1, true, true, 0);
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
            //if(isAABBinFrustum(LightSphere->getTransformedBoundingBox(), Device->getSceneManager()->getActiveCamera()->getViewFrustum())) LightSphere->render();
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
            LightSphere->setScale(irr::core::vector3df(light.Radius));
            LightSphere->setPosition(light.Position + light.Direction * light.Radius);
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


    //post processing
    if(getPostProcessingActive() && getActivePostProcessingEffectsCount() > 0)
    {
        irr::u32 numEffects= getActivePostProcessingEffectsCount();
        //how many of the effects have we already rendered
        irr::u32 numEffectsDone= 0;

        for(irr::u32 i= 0; i < PostProcessingEffectChains.size(); i++)
        {
            if(PostProcessingEffectChains[i]->isActive())
            {
                for(irr::u32 j= 0; j < PostProcessingEffectChains[i]->getEffectCount(); j++)
                {
                    irr::video::CPostProcessingEffect* effect= PostProcessingEffectChains[i]->getEffectFromIndex(j);
                    if(effect->isActive())
                    {
                        //we better figure out which of the two alternating RTTs are we going to use
                        if(numEffectsDone % 2 == 0)
                        {
                            Device->getVideoDriver()->setRenderTarget(PPTex2);
                            LightQuad->setMaterialTexture(0, PPTex1);
                        }
                        else
                        {
                            Device->getVideoDriver()->setRenderTarget(PPTex1);
                            LightQuad->setMaterialTexture(0, PPTex2);
                        }

                        //if this is the last effect, time to render to screen now (unless you want your last render done to texture)
                        if(numEffectsDone+1 == numEffects)
                        {
                            //if you want your last render directly to screen
                            if(!getDoFinalRenderToTexture()) Device->getVideoDriver()->setRenderTarget(0);
                            //if you want it to a render texture
                            else Device->getVideoDriver()->setRenderTarget(FinalRender);
                        }


                        for(irr::u32 k= 0; k < effect->getTextureToPassCount(); k++)
                        {
                            //1. channel is color, start from 2.
                            LightQuad->setMaterialTexture(k+1, effect->getTextureToPass(k));
                        }

                        LightQuad->setMaterialType(effect->getMaterialType());
                        LightQuad->render();
                        numEffectsDone++;
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


void irr::scene::ILightManagerCustom::setPostProcessingTextures(irr::video::ITexture* tex1, irr::video::ITexture* tex2)
{
    PPTex1= tex1;
    PPTex2= tex2;
}

void irr::scene::ILightManagerCustom::setPostProcessingActive(bool active)
{
    PostProcessing= active;
}

bool irr::scene::ILightManagerCustom::getPostProcessingActive()
{
    return PostProcessing;
}

void irr::scene::ILightManagerCustom::addPostProcessingEffectChain(irr::video::CPostProcessingEffectChain* chain)
{
    PostProcessingEffectChains.push_back(chain);
}

irr::u32 irr::scene::ILightManagerCustom::getActivePostProcessingEffectsCount()
{
    irr::u32 count= 0;
    for(irr::u32 i= 0; i < PostProcessingEffectChains.size(); i++)
    {
        if(PostProcessingEffectChains[i]->isActive())
        {
            count+= PostProcessingEffectChains[i]->getActiveEffectCount();
        }
    }
    return count;
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


void irr::scene::ILightManagerCustom::setRenderTexture(irr::video::ITexture* tex)
{
    FinalRender= tex;
}

void irr::scene::ILightManagerCustom::setDoFinalRenderIntoTexture(bool well)
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



bool irr::scene::ILightManagerCustom::isAABBinFrustum(irr::core::aabbox3d<irr::f32> box, const irr::scene::SViewFrustum *frustum)
{
    for(irr::u32 i= 0; i < irr::scene::SViewFrustum::VF_PLANE_COUNT; i++)
    {
        if(box.classifyPlaneRelation(frustum->planes[i]) == irr::core::ISREL3D_BACK) return true;
    }
    return false;
}
