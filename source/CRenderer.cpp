// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CRenderer.h"
#include "CPostProcessingEffectChain.h"


irr::video::CRenderer::CRenderer(irr::IrrlichtDevice* device, bool HDR, irr::c8* shaderDir)
{
    Device= device;
    LightMgr= 0;

    ShaderLib= new CShaderLibrary(shaderDir);
    Materials= new SMaterials;

    loadShaders();
    createDefaultPipeline(HDR);
    RootPostProcessingEffectChain= createPostProcessingEffectChain();

    Device->run();
}

irr::video::CRenderer::~CRenderer()
{
    delete Materials;
    delete ShaderLib;
    delete LightMgr;

    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i].RenderTexture);
    }

    Device->getSceneManager()->setLightManager(0);
    Device= 0;
}

void irr::video::CRenderer::createDefaultPipeline(bool HDR)
{
    clearMRTs();
    if(!HDR)
    {
        createMRT("deferred-mrt-color-dont-use-this-name-thanks", irr::video::ECF_A8R8G8B8);
        createMRT("deferred-mrt-normal-dont-use-this-name-thanks", irr::video::ECF_G16R16F);
        createMRT("deferred-mrt-depth-dont-use-this-name-thanks", irr::video::ECF_G16R16F);
    }
    else
    {
        createMRT("deferred-mrt-color-dont-use-this-name-thanks", irr::video::ECF_A16B16G16R16F);
        createMRT("deferred-mrt-normal-dont-use-this-name-thanks", irr::video::ECF_G32R32F);
        createMRT("deferred-mrt-depth-dont-use-this-name-thanks", irr::video::ECF_G32R32F);
    }

    if(LightMgr) LightMgr->drop();
    LightMgr= new irr::scene::ILightManagerCustom(Device);
    LightMgr->setMRTs(MRTs);
    Device->getSceneManager()->setLightManager(LightMgr);


    Materials->Solid= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("solid"), new DefaultCallback);
    Materials->NormalAnimated= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("normalAnimated"), new DefaultCallback);
    Materials->DetailMap= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("terrain"), new DefaultCallback);


    //set up point lights
    irr::video::IShaderPointLightCallback* pointCallback= new irr::video::IShaderPointLightCallback(Device->getSceneManager());
    Materials->LightPoint= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_point"), pointCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightPointMaterialType(Materials->LightPoint);
    LightMgr->setLightPointCallback(pointCallback);

    //set up spot lights
    irr::video::IShaderSpotLightCallback* spotCallback= new irr::video::IShaderSpotLightCallback(Device->getSceneManager());
    Materials->LightSpot= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_spot"), spotCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightSpotMaterialType(Materials->LightSpot);
    LightMgr->setLightSpotCallback(spotCallback);

    //set up directional lights
    irr::video::IShaderDirectionalLightCallback* directionalCallback= new irr::video::IShaderDirectionalLightCallback(Device->getSceneManager());
    Materials->LightDirectional= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_directional"), directionalCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightDirectionalMaterialType(Materials->LightDirectional);
    LightMgr->setLightDirectionalCallback(directionalCallback);

    //set up ambient light
    irr::video::IShaderAmbientLightCallback* ambientCallback= new irr::video::IShaderAmbientLightCallback(Device->getSceneManager());
    Materials->LightAmbient= (irr::video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_ambient"), ambientCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightAmbientMaterialType(Materials->LightAmbient);
    LightMgr->setLightAmbientCallback(ambientCallback);
}


irr::video::CPostProcessingEffectChain* irr::video::CRenderer::createPostProcessingEffectChain()
{
    return new irr::video::CPostProcessingEffectChain(this);
}

irr::video::CPostProcessingEffect* irr::video::CRenderer::createPostProcessingEffect(irr::video::SShaderSource &effectShader, irr::video::IShaderConstantSetCallBack* callback)
{
    enablePostProcessing(true);
    irr::video::E_MATERIAL_TYPE effectId= (irr::video::E_MATERIAL_TYPE)createMaterial(effectShader, callback);
    irr::video::CPostProcessingEffect* effect= new irr::video::CPostProcessingEffect(effectId, callback);
    LightMgr->addPostProcessingEffect(effect);
    return effect;
}

irr::video::CPostProcessingEffect* irr::video::CRenderer::createPostProcessingEffect(irr::video::E_POSTPROCESSING_EFFECT type)
{
    video::CPostProcessingEffect* newEffect= 0;

    switch(type)
    {
        case EPE_ANTIALIASING:
            ShaderLib->loadShader("antialias", "quad.vert", "postprocess/antialias.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("antialias"));
            newEffect->addTextureToShader(getMRT(2));
            break;

        case EPE_BLOOM:
            ShaderLib->loadShader("bloom", "quad.vert", "postprocess/bloom.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("bloom"));
            break;

        case EPE_BLOOM_LQ:
            ShaderLib->loadShader("bloom_lq", "quad.vert", "postprocess/bloom_lq.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("bloom_lq"));
            break;

        case EPE_CONTRAST:
            ShaderLib->loadShader("contrast", "quad.vert", "postprocess/contrast.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("contrast"));
            break;

        case EPE_COLD_COLORS:
            ShaderLib->loadShader("coldcolors", "quad.vert", "postprocess/coldcolors.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("coldcolors"));
            break;

        case EPE_WARM_COLORS:
            ShaderLib->loadShader("warmcolors", "quad.vert", "postprocess/warmcolors.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("warmcolors"));
            break;

        case EPE_TONE_MAPPING:
            ShaderLib->loadShader("tonemapping", "quad.vert", "postprocess/tonemapping.frag");
            newEffect= createPostProcessingEffect(ShaderLib->getShader("tonemapping"));
            break;

        default: break; //this should never happen
    }

    return newEffect;
}

void irr::video::CRenderer::enablePostProcessing(bool enable, irr::video::ECOLOR_FORMAT format)
{
    if(enable && !LightMgr->getRenderTexture())
    {
        irr::core::dimension2du dimension= Device->getVideoDriver()->getCurrentRenderTargetSize();
        LightMgr->setRenderTexture(Device->getVideoDriver()->addRenderTargetTexture(dimension, "Final-Render-Tex", format));
    }
    else if(!enable && LightMgr->getRenderTexture())
    {
        //LightMgr->removeRenderTexture();
    }
}

void irr::video::CRenderer::clearMRTs()
{
    for(irr::u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i].RenderTexture);
    }
    MRTs.clear();
}

void irr::video::CRenderer::createMRT(irr::c8* name, irr::video::ECOLOR_FORMAT format, irr::core::dimension2du dimension)
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
    if(LightMgr->getDoFinalRenderToTexture()) return LightMgr->getRenderTexture();
    else return 0;
}

irr::video::SMaterials* irr::video::CRenderer::getMaterials()
{
    return Materials;
}

irr::s32 irr::video::CRenderer::createMaterial(irr::video::SShaderSource shader, irr::video::IShaderConstantSetCallBack *callback, irr::video::E_MATERIAL_TYPE baseType)
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
            else if(node->getMaterial(ii).MaterialType == irr::video::EMT_NORMAL_MAP_SOLID) node->getMaterial(ii).MaterialType= getMaterials()->NormalAnimated;
            else if(node->getMaterial(ii).MaterialType == irr::video::EMT_DETAIL_MAP) node->getMaterial(ii).MaterialType= getMaterials()->DetailMap;
        }
    }
}

irr::video::CShaderLibrary* irr::video::CRenderer::getShaderLibrary()
{
    return ShaderLib;
}

irr::video::CPostProcessingEffectChain* irr::video::CRenderer::getRootPostProcessingEffectChain()
{
    return RootPostProcessingEffectChain;
}


void irr::video::CRenderer::loadShaders()
{
    ShaderLib->loadShader("light_point", "light.vert", "light_point.frag");
    ShaderLib->loadShader("light_spot", "light.vert", "light_spot.frag");
    ShaderLib->loadShader("light_directional", "quad.vert", "light_directional.frag");
    ShaderLib->loadShader("light_ambient", "quad.vert", "light_ambient.frag");

    ShaderLib->loadShader("solid", "solid.vert", "solid.frag");
    ShaderLib->loadShader("normalAnimated", "normalmap_animated.vert", "normalmap_animated.frag");
    ShaderLib->loadShader("terrain", "terrain.vert", "terrain.frag");
}
