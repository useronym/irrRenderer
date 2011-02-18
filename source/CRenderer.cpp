// This file is part of the "irrRenderer".
// For conditions of distribution and use, see copyright notice in irrRenderer.h

#include "CRenderer.h"

irr::video::CRenderer::CRenderer(irr::IrrlichtDevice* device, irr::c8* shaderDir)
{
    Device= device;

    ShaderLib= new CShaderLibrary(shaderDir);
    //predefined shaders
    ShaderLib->loadShader("deferred_compose", "deferred_compose.vert", "deferred_compose.frag");
    ShaderLib->loadShader("solid", "solid.vert", "solid.frag");
    ShaderLib->loadShader("terrain", "terrain.vert", "terrain.frag");


    Materials= new SMaterials;
    Materials->DeferredCompose= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("deferred_compose"), new DeferredComposeCallback(Device->getSceneManager()));
    Materials->Solid= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("solid"), new DefaultCallback);
    Materials->Terrain= (irr::video::E_MATERIAL_TYPE)addMaterial(ShaderLib->getShader("terrain"), new TerrainCallback);

    MRTs.push_back(irr::video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(Device->getVideoDriver()->getCurrentRenderTargetSize(), "Deferred-color-do-not-use-this-funking-name-thanks", irr::video::ECF_A8R8G8B8)));//ECF_A16B16G16R16F
    MRTs.push_back(irr::video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(Device->getVideoDriver()->getCurrentRenderTargetSize(), "Deferred-normal-do-not-use-this-funking-name-thanks", irr::video::ECF_A8R8G8B8)));//ECF_A8R8G8B8

    irr::scene::IMesh* quadMesh= Device->getSceneManager()->getMesh("plane.obj");/*Device->getSceneManager()->getGeometryCreator()->createPlaneMesh(core::dimension2d<f32>(1,1),
                                                                                             core::dimension2d<u32>(1,1),
                                                                                             new video::SMaterial(),
                                                                                             core::dimension2d<f32>(1,1));*/

    /*core::array<video::S3DVertex> *vertices= (core::array<video::S3DVertex> *)quadMesh->getMeshBuffer(0)->getVertices();

    vertices[0].Pos= core::vector3df(-0.5,0.5,0);
    vertices[1].Pos= core::vector3df(0.5,0.5,0);
    vertices[2].Pos= core::vector3df(0.5,-0.5,0);
    vertices[3].Pos= core::vector3df(-0.5,-0.5,0);*/

    ScreenQuad= Device->getSceneManager()->addMeshSceneNode(quadMesh);
    ScreenQuad->setMaterialType(getMaterials()->DeferredCompose);
    ScreenQuad->setMaterialTexture(0, MRTs[0].RenderTexture);
    ScreenQuad->setMaterialTexture(1, MRTs[1].RenderTexture);
    ScreenQuad->setVisible(false);
}

irr::video::CRenderer::~CRenderer()
{
    //dtor
}

irr::video::SMaterials* irr::video::CRenderer::getMaterials()
{
    return Materials;
}

irr::s32 irr::video::CRenderer::addMaterial(irr::video::SShader shader, irr::video::IShaderConstantSetCallBack *callback)
{
    return Device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               shader.SourceVertex.c_str(), "main", irr::video::EVST_VS_2_0,
               shader.SourcePixel.c_str(), "main", irr::video::EPST_PS_2_0,
               callback);
}


void irr::video::CRenderer::drawAll()
{
    Device->getVideoDriver()->setRenderTarget(MRTs, true, true, 0);
    Device->getSceneManager()->drawAll();

    Device->getVideoDriver()->setRenderTarget(0, true, true, 0);
    ScreenQuad->render();

    Device->getVideoDriver()->setMaterial(irr::video::SMaterial());
}

irr::video::CShaderLibrary* irr::video::CRenderer::getShaderLibrary()
{
    return ShaderLib;
}

irr::video::ITexture* irr::video::CRenderer::getColorBuffer()
{
    return MRTs[0].RenderTexture;
}

irr::video::ITexture* irr::video::CRenderer::getNormalBuffer()
{
    return MRTs[1].RenderTexture;
}
