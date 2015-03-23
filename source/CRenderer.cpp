// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CRenderer.h"

using namespace irr;


video::CRenderer::CRenderer(IrrlichtDevice* device,
                                 const c8* shaderDir,
                                 video::ECOLOR_FORMAT depth)
{
    Device= device;
    LightMgr= 0;

    ShaderLib = new video::CShaderLibrary(shaderDir, Device);
    Materials = new video::SMaterials;

    loadShaders();
    createDefaultPipeline(depth);
    loadMaterials();
    MaterialSwapper = new video::CMaterialSwapper(Device->getSceneManager(), Materials);

    Device->run();
}

video::CRenderer::~CRenderer()
{
    delete Materials;
    delete ShaderLib;
    delete LightMgr;

    for(u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i]);
    }

    Device->getSceneManager()->setLightManager(0);
    Device= 0;
}

void video::CRenderer::loadMaterials()
{
Materials->Solid= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("solid"), new video::IShaderDefaultCallback);
    Materials->TransparentRef= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("transparent_alpha_ref"), new video::IShaderDefaultCallback);
    Materials->Transparent= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("transparent_alpha"),
                                                                        new video::IShaderDefaultCallback,
                                                                        video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    Materials->TransparentSoft= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("transparent_alpha_soft"),
                                                                            new video::IShaderDefaultCallback,
                                                                            video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    Materials->Normal= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("normal"), new video::IShaderDefaultCallback);
    Materials->NormalAnimated= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("normalAnimated"), new video::IShaderDefaultCallback);
    Materials->Parallax= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("parallax"), new video::IShaderDefaultCallback);
    Materials->DetailMap= (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("detail"), new video::IShaderDefaultCallback);


    scene::ISceneManager *smgr = Device->getSceneManager();

    video::PointCallback* pointCallback = new video::PointCallback(smgr);
    Materials->LightPoint = (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_point"),
                                                                   pointCallback,
                                                                   video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightPointMaterialType(Materials->LightPoint);
    LightMgr->setLightPointCallback(pointCallback);

    video::SpotCallback* spotCallback = new video::SpotCallback(smgr);
    Materials->LightSpot = (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_spot"),
                                                                  spotCallback,
                                                                  video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightSpotMaterialType(Materials->LightSpot);
    LightMgr->setLightSpotCallback(spotCallback);

    video::DirectionalCallback* directionalCallback = new video::DirectionalCallback(smgr);
    Materials->LightDirectional = (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_directional"),
                                                                         directionalCallback,
                                                                         video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightDirectionalMaterialType(Materials->LightDirectional);
    LightMgr->setLightDirectionalCallback(directionalCallback);

    video::AmbientCallback* ambientCallback = new video::AmbientCallback(smgr);
    Materials->LightAmbient = (video::E_MATERIAL_TYPE)createMaterial(ShaderLib->getShader("light_ambient"),
                                                                     ambientCallback,
                                                                     video::EMT_TRANSPARENT_ADD_COLOR);
    LightMgr->setLightAmbientMaterialType(Materials->LightAmbient);
    LightMgr->setLightAmbientCallback(ambientCallback);
}


void video::CRenderer::createDefaultPipeline(irr::video::ECOLOR_FORMAT depth)
{
    clearMRTs();
    createMRT("deferred-mrt-color", video::ECF_A8R8G8B8);
    createMRT("deferred-mrt-normal", video::ECF_A8R8G8B8);
    video::IVideoDriver *video = Device->getVideoDriver();
    DepthBuffer = video->addRenderTargetTexture(video->getCurrentRenderTargetSize(),
                                                "deferred-depth",
                                                depth);
    RenderTarget = video->addRenderTarget();
    RenderTarget->setTexture(MRTs, DepthBuffer);

    if(LightMgr)
        LightMgr->drop();
    LightMgr = new scene::ILightManagerCustom(Device, Materials);
    LightMgr->setRenderTarget(RenderTarget);
    Device->getSceneManager()->setLightManager(LightMgr);    
}

void video::CRenderer::clearMRTs()
{
    for(u32 i= 0; i < MRTs.size(); i++)
    {
        Device->getVideoDriver()->removeTexture(MRTs[i]);
    }
    MRTs.clear();
    Device->getVideoDriver()->removeTexture(DepthBuffer);
}

void video::CRenderer::createMRT(const c8* name, video::ECOLOR_FORMAT format, core::dimension2du dimension)
{
    if(MRTs.size() <= 4)
    {
        if(dimension.Height == 0 || dimension.Width == 0) dimension= Device->getVideoDriver()->getCurrentRenderTargetSize();
        MRTs.push_back(Device->getVideoDriver()->addRenderTargetTexture(dimension, name, format));
    }
}

video::ITexture* video::CRenderer::getMRT(u32 index)
{
    return MRTs[index];
}

u32 video::CRenderer::getMRTCount() const
{
    return MRTs.size();
}

video::ITexture* video::CRenderer::getDepthBuffer() const
{
    return DepthBuffer;
}

void video::CRenderer::setDoFinalRenderToTexture(bool shouldI)
{
    if(shouldI && !LightMgr->getDoFinalRenderToTexture())
    {
        core::dimension2du dimension= Device->getVideoDriver()->getCurrentRenderTargetSize();
        LightMgr->setDoFinalRenderIntoTexture(true);
    }
    else if(LightMgr->getDoFinalRenderToTexture())
    {
        LightMgr->setDoFinalRenderIntoTexture(false);
        Device->getVideoDriver()->removeTexture(LightMgr->getRenderTexture());
    }
}

video::ITexture* video::CRenderer::getFinalRenderTexture() const
{
    if(LightMgr->getDoFinalRenderToTexture()) return LightMgr->getRenderTexture();
    else return 0;
}

s32 video::CRenderer::createMaterial(video::SShaderSource shader, video::IShaderConstantSetCallBack *callback, video::E_MATERIAL_TYPE baseType)
{
    Device->getLogger()->log("compile shader", shader.Name.c_str(), ELL_INFORMATION);
    return Device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               shader.SourceVertex.c_str(), "main", video::EVST_VS_2_0,
               shader.SourcePixel.c_str(), "main", video::EPST_PS_2_0,
               callback, baseType);
}


video::CShaderLibrary* video::CRenderer::getShaderLibrary() const
{
    return ShaderLib;
}


video::SMaterials* video::CRenderer::getMaterials() const
{
    return Materials;
}

video::CMaterialSwapper* video::CRenderer::getMaterialSwapper() const
{
    return MaterialSwapper;
}

IrrlichtDevice* video::CRenderer::getDevice() const
{
    return Device;
}

void video::CRenderer::loadShaders()
{
    ShaderLib->loadShader("light_point", "light.vert", "light_point.frag");
    ShaderLib->loadShader("light_spot", "light.vert", "light_spot.frag");
    ShaderLib->loadShader("light_directional", "quad.vert", "light_directional.frag");
    ShaderLib->loadShader("light_ambient", "quad.vert", "light_ambient.frag");

    ShaderLib->loadShader("solid", "solid.vert", "solid.frag");
    ShaderLib->loadShader("transparent_alpha_ref", "transparent_alpha_ref.vert", "transparent_alpha_ref.frag");
    ShaderLib->loadShader("transparent_alpha", "transparent_alpha.vert", "transparent_alpha.frag");
    ShaderLib->loadShader("transparent_alpha_soft", "transparent_alpha_soft.vert", "transparent_alpha_soft.frag");
    ShaderLib->loadShader("normal", "normalmap.vert", "normalmap.frag");
    ShaderLib->loadShader("normalAnimated", "normalmap_animated.vert", "normalmap_animated.frag");
    ShaderLib->loadShader("parallax", "parallaxmap.vert", "parallaxmap.frag");
    ShaderLib->loadShader("detail", "terrain.vert", "terrain.frag");
}
