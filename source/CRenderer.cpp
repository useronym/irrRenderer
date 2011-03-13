// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CRenderer.h"


irr::video::CRenderer::CRenderer(irr::IrrlichtDevice* device, irr::c8* shaderDir)
{
    Device= device;
    LightMgr= 0;

    ShaderLib= new CShaderLibrary(shaderDir);
    Materials= new SMaterials;

    createDefaultPipeline();
    Device->run();
}

irr::video::CRenderer::~CRenderer()
{
    //dtor
}

void irr::video::CRenderer::createDefaultPipeline()
{
    clearMRTs();
    addMRT("deferred-mrt-color-dont-use-this-name-thanks", irr::video::ECF_A8R8G8B8);
    addMRT("deferred-mrt-normal-dont-use-this-name-thanks", irr::video::ECF_G16R16F);
    addMRT("deferred-mrt-depth-dont-use-this-name-thanks", irr::video::ECF_G16R16F);

    if(LightMgr) LightMgr->drop();
    LightMgr= new irr::scene::ILightManagerCustom(Device);
    LightMgr->setMRTs(MRTs);
    Device->getSceneManager()->setLightManager(LightMgr);

    ShaderLib->loadShader("solid", "solid.vert", "solid.frag");
    ShaderLib->loadShader("terrain", "terrain.vert", "terrain.frag");
    ShaderLib->loadShader("light_point", "light.vert", "light_point.frag");
    ShaderLib->loadShader("light_spot", "light.vert", "light_point.frag");
    ShaderLib->loadShader("light_directional", "quad.vert", "light_directional.frag");
    ShaderLib->loadShader("light_ambient", "quad.vert", "light_ambient.frag");

    Materials->Solid= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("solid"), new DefaultCallback);
    Materials->DetailMap= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("terrain"), new DefaultCallback);

    //set up point lights
    irr::video::IShaderPointLightCallback* pointCallback= new irr::video::IShaderPointLightCallback(Device->getSceneManager());
    Materials->LightPoint= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("light_point"), pointCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightPointMaterialType(Materials->LightPoint);
    LightMgr->setLightPointCallback(pointCallback);

    //set up spot lights
    irr::video::IShaderSpotLightCallback* spotCallback= new irr::video::IShaderSpotLightCallback(Device->getSceneManager());
    Materials->LightSpot= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("light_spot"), spotCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightSpotMaterialType(Materials->LightSpot);
    LightMgr->setLightSpotCallback(spotCallback);

    //set up directional lights
    irr::video::IShaderDirectionalLightCallback* directionalCallback= new irr::video::IShaderDirectionalLightCallback(Device->getSceneManager());
    Materials->LightDirectional= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("light_directional"), directionalCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightDirectionalMaterialType(Materials->LightDirectional);
    LightMgr->setLightDirectionalCallback(directionalCallback);

    //set up ambient light
    irr::video::IShaderAmbientLightCallback* ambientCallback= new irr::video::IShaderAmbientLightCallback(Device->getSceneManager());
    Materials->LightAmbient= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("light_ambient"), ambientCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightAmbientMaterialType(Materials->LightAmbient);
    LightMgr->setLightAmbientCallback(ambientCallback);
}

void irr::video::CRenderer::clearMRTs()
{
    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i].RenderTexture);
    }
    MRTs.clear();
}

void irr::video::CRenderer::addMRT(irr::c8* name, irr::video::ECOLOR_FORMAT format, irr::core::dimension2du dimension)
{
    if(MRTs.size() <= 4)
    {
        if(dimension.Height == 0 || dimension.Width == 0) dimension= Device->getVideoDriver()->getCurrentRenderTargetSize();
        MRTs.push_back(irr::video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(dimension, name, format)));
    }
}

irr::video::ITexture* irr::video::CRenderer::getMRT(irr::u32 index)
{
    return MRTs[index].RenderTexture;
}

irr::u32 irr::video::CRenderer::getMRTCount()
{
    return MRTs.size();
}

void irr::video::CRenderer::setDoFinalRenderToTexture(bool shouldI)
{
    if(shouldI)
    {

    }
}

irr::video::ITexture* irr::video::CRenderer::getFinalRenderTexture()
{
    if(LightMgr->getFinalRenderTexture() != 0) return LightMgr->getFinalRenderTexture();
    else return 0;
}

irr::video::SMaterials* irr::video::CRenderer::getMaterials()
{
    return Materials;
}

irr::s32 irr::video::CRenderer::addMaterial(irr::video::SShaderSource shader, irr::video::IShaderConstantSetCallBack *callback, irr::video::E_MATERIAL_TYPE baseType)
{
    return Device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               shader.SourceVertex.c_str(), "main", irr::video::EVST_VS_2_0,
               shader.SourcePixel.c_str(), "main", irr::video::EPST_PS_2_0,
               callback, baseType);
}


void irr::video::CRenderer::swapMaterials()
{
    irr::core::array<irr::scene::ISceneNode*> nodes;
    Device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes);

    for(irr::u32 i= 0; i < nodes.size(); i++)
    {
        irr::scene::ISceneNode* node= nodes[i];
        for(irr::u32 ii= 0; ii < node->getMaterialCount(); ii++)
        {
            if(node->getMaterial(ii).MaterialType == irr::video::EMT_SOLID) node->getMaterial(ii).MaterialType= getMaterials()->Solid;
            else if(node->getMaterial(ii).MaterialType == irr::video::EMT_DETAIL_MAP) node->getMaterial(ii).MaterialType= getMaterials()->DetailMap;
        }
    }
}

irr::video::CShaderLibrary* irr::video::CRenderer::getShaderLibrary()
{
    return ShaderLib;
}
