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
    LightSphere->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);

    //set up light mesh - cone
    LightCone= Device->getSceneManager()->addSphereSceneNode(1.0, 12);//Device->getSceneManager()->addMeshSceneNode(Device->getSceneManager()->getGeometryCreator()->createConeMesh(1.0, 1.0, 8));
    LightCone->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    LightCone->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, true);
    LightCone->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    LightCone->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);

    //set up light mesh - quad
    LightQuad= new irr::scene::IQuadSceneNode(0, Device->getSceneManager());
    LightQuad->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
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
    if(!FinalRenderToTexture)
    {
        Device->getVideoDriver()->setRenderTarget(0);
    }
    else
    {
        Device->getVideoDriver()->setRenderTarget(FinalRender, true, true, 0);
    }


    //render ambient
    LightQuad->setMaterialType(LightAmbientMaterial);
    LightQuad->render(); //also renders nodes with no lighting

    //render dynamic lights
    for(irr::u32 i= 0; i < Device->getVideoDriver()->getDynamicLightCount(); i++)
    {
        irr::video::SLight light= Device->getVideoDriver()->getDynamicLight(i);
        irr::core::vector3df camPos= Device->getSceneManager()->getActiveCamera()->getAbsolutePosition();

        //point
        if(light.Type == irr::video::ELT_POINT)
        {
            LightPointCallback->updateConstants(light);
            LightSphere->setScale(irr::core::vector3df(light.Radius));
            LightSphere->setPosition(light.Position);
            LightSphere->updateAbsolutePosition();
            LightSphere->render();
        }

        //spot
        else if(light.Type == irr::video::ELT_SPOT)
        {
            LightSpotCallback->updateConstants(light);
            LightCone->setScale(irr::core::vector3df(LightSpotCallback->getConeRadius(), light.Radius*1.4, LightSpotCallback->getConeRadius()));
            //we(well, me, really) need to do some more calculations because the cone mesh we created is kinda fucked up by default
            LightCone->setRotation(light.Direction.getHorizontalAngle() + irr::core::vector3df(-90.0, 0.0, 0.0));
            LightCone->setPosition(light.Position + light.Direction*light.Radius*0.85);
            //done.. true power of irrlicht!
            LightCone->updateAbsolutePosition();
            LightCone->render();
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
    if(isPostProcessing() && FinalRender)
    {
        if(PostProcessingEffects.size() > 1)
        {
            Device->getVideoDriver()->setRenderTarget(FinalRender);
        }

        LightQuad->setMaterialTexture(0, FinalRender);
        for(irr::u32 i= 0; i < MRTs.size(); i++)
        {
            LightQuad->setMaterialTexture(i+1, MRTs[i].RenderTexture);
        }

        for(irr::u32 i= 0; i < PostProcessingEffects.size(); i++)
        {
            if(i+1 == PostProcessingEffects.size() && !FinalRenderToTexture)
            {
                Device->getVideoDriver()->setRenderTarget(0);
            }

            LightQuad->setMaterialType(PostProcessingEffects[i]);
            LightQuad->render();
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
        LightCone->setMaterialTexture(i, MRTs[i].RenderTexture);
        LightQuad->setMaterialTexture(i, MRTs[i].RenderTexture);
    }
}

void irr::scene::ILightManagerCustom::addPostProcessingEffect(irr::video::E_MATERIAL_TYPE &matType)
{
    PostProcessingEffects.push_back(matType);
}

bool irr::scene::ILightManagerCustom::isPostProcessing()
{
    if(PostProcessingEffects.size() > 0) return true;
    else return false;
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
    LightSphere->setMaterialType(LightPointMaterial);
}

void irr::scene::ILightManagerCustom::setLightPointCallback(irr::video::IShaderPointLightCallback* callback)
{
    LightPointCallback= callback;
}

void irr::scene::ILightManagerCustom::setLightSpotMaterialType(irr::video::E_MATERIAL_TYPE &type)
{
    LightSpotMaterial= type;
    LightCone->setMaterialType(LightSpotMaterial);
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
