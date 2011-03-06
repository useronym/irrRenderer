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
    addMRT("deferred-mrt-color-dont-use-this-name-thanks");
    addMRT("deferred-mrt-normal-depth-matid-dont-use-this-name-thanks");

    if(LightMgr) LightMgr->drop();
    LightMgr= new irr::scene::ILightManagerCustom(Device);
    LightMgr->setMRTs(MRTs);
    Device->getSceneManager()->setLightManager(LightMgr);

    ShaderLib->loadShader("deferred_compose", "deferred_compose.vert", "deferred_compose.frag");
    ShaderLib->loadShader("solid", "solid.vert", "solid.frag");
    ShaderLib->loadShader("terrain", "terrain.vert", "terrain.frag");
    ShaderLib->loadShader("light_point", "light.vert", "light_point.frag");

    Materials->DeferredCompose= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("deferred_compose"), new DeferredComposeCallback(Device->getSceneManager()));
    Materials->Solid= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("solid"), new DefaultCallback);
    Materials->Terrain= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("terrain"), new DefaultCallback);

    irr::video::IShaderLightCallback* lightPointCallback= new irr::video::IShaderLightCallback(Device->getSceneManager());
    Materials->LightPoint= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("light_point"), lightPointCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightPointMaterialType(Materials->LightPoint);
    LightMgr->setLightPointCallback(lightPointCallback);
}

void irr::video::CRenderer::clearMRTs()
{
    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i].RenderTexture);
    }
    MRTs.clear();
}

void irr::video::CRenderer::addMRT(irr::c8* name, irr::core::dimension2du dimension)
{
    if(MRTs.size() <= 4)
    {
        if(dimension.Height == 0 || dimension.Width == 0) dimension= Device->getVideoDriver()->getCurrentRenderTargetSize();
        MRTs.push_back(irr::video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(dimension, name, irr::video::ECF_A16B16G16R16F)));//ECF_A16B16G16R16F
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

irr::s32 irr::video::CRenderer::addMaterial(irr::video::SShader shader, irr::video::IShaderConstantSetCallBack *callback, irr::video::E_MATERIAL_TYPE baseType)
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
            if(node->getMaterial(i).MaterialType == irr::video::EMT_SOLID) node->getMaterial(i).MaterialType= getMaterials()->Solid;
            else if(node->getMaterial(i).MaterialType == irr::video::EMT_DETAIL_MAP) node->getMaterial(i).MaterialType= getMaterials()->Terrain;
        }
        node->getMaterial(0).MaterialType= getMaterials()->Terrain;
    }
}

irr::video::CShaderLibrary* irr::video::CRenderer::getShaderLibrary()
{
    return ShaderLib;
}
