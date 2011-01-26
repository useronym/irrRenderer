#include "CRenderer.h"

CRenderer::CRenderer(IrrlichtDevice* device, char* shaderDir)
{
    Device= device;
    Shaders= new CShaderLibrary(shaderDir);
    Materials= new SMaterials;

    Materials->DeferredCompose= (irr::video::E_MATERIAL_TYPE)device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               Shaders->deferred_compose_vs, "main", irr::video::EVST_VS_2_0,
               Shaders->deferred_compose_ps, "main", irr::video::EPST_PS_2_0,
               new DeferredComposeCallback(Device->getSceneManager()));

    Materials->Solid= (irr::video::E_MATERIAL_TYPE)device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               Shaders->solid_vs, "main", irr::video::EVST_VS_2_0,
               Shaders->solid_ps, "main", irr::video::EPST_PS_2_0,
               new DefaultCallback);

    Materials->Terrain= (irr::video::E_MATERIAL_TYPE)device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
               Shaders->terrain_vs, "main", irr::video::EVST_VS_2_0,
               Shaders->terrain_ps, "main", irr::video::EPST_PS_2_0,
               new TerrainCallback);

    MRTs.push_back(video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(Device->getVideoDriver()->getCurrentRenderTargetSize(), "Deferred-color", video::ECF_A8R8G8B8)));//ECF_A16B16G16R16F
    MRTs.push_back(video::IRenderTarget(Device->getVideoDriver()->addRenderTargetTexture(Device->getVideoDriver()->getCurrentRenderTargetSize(), "Deferred-normal", video::ECF_A8R8G8B8)));//ECF_A8R8G8B8

    scene::IMesh* quadMesh= Device->getSceneManager()->getMesh("plane.obj");/*Device->getSceneManager()->getGeometryCreator()->createPlaneMesh(core::dimension2d<f32>(1,1),
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

CRenderer::~CRenderer()
{
    //dtor
}

SMaterials* CRenderer::getMaterials()
{
    return Materials;
}

void CRenderer::drawAll()
{
    Device->getVideoDriver()->setRenderTarget(MRTs, true, true, 0);
    Device->getSceneManager()->drawAll();

    Device->getVideoDriver()->setRenderTarget(0, true, true, 0);
    ScreenQuad->render();

    Device->getVideoDriver()->setMaterial(video::SMaterial());
}

video::ITexture* CRenderer::getColorBuffer()
{
    return MRTs[0].RenderTexture;
}

video::ITexture* CRenderer::getNormalBuffer()
{
    return MRTs[1].RenderTexture;
}
