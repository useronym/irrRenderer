#include "CTestFramework.h"

CTestFramework::CTestFramework()
{
    Device= createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600));
    Renderer= createRenderer(Device);

    //create some live shit
    scene::ISceneManager* smgr= Device->getSceneManager();
    /*smgr->addHillPlaneMesh("hillbilly",
                            core::dimension2d<f32>(10,10),
                            core::dimension2d<u32>(50,50),
                            0,
                            10.0f,
                            core::dimension2d<f32>(5.0f, 2.0f),
                            core::dimension2d<f32>(2,2));
    scene::IMeshSceneNode* ground= smgr->addMeshSceneNode(smgr->getMesh("hillbilly"));
    ground->setMaterialTexture(0, Device->getVideoDriver()->getTexture("media/island_TX.jpg"));
    ground->setMaterialType(Renderer->getMaterials()->Solid);

    scene::IMeshSceneNode* ball= smgr->addSphereSceneNode(20);
    ball->setPosition(core::vector3df(0, 30, 0));
    ball->setMaterialTexture(0, Device->getVideoDriver()->getTexture("media/detailmap.jpg"));
    ball->setMaterialType(Renderer->getMaterials()->Solid);

    for(int i= 1; i < 31; i++)
    {
        float lrange= 75+i*5;
        irr::video::SColorf lcolor= irr::video::SColorf(i/50.0, 1-i/30.0, i/30.0, 0.0);

        scene::ILightSceneNode* light= smgr->addLightSceneNode(0, irr::core::vector3df(0), lcolor);
        light->setRadius(120);
        light->addAnimator(smgr->createFlyCircleAnimator(core::vector3df(0.f, 50.f, 0.f), lrange, 0.001f, irr::core::vector3df(0.f, 1.f, 0.f), i/20.0));
    }*/

    smgr->loadScene("media/scene.irr");
    scene::ICameraSceneNode* cam= smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    scene::ISceneNode* camPos= smgr->getSceneNodeFromName("camera");
    if(camPos)
    {
        cam->setPosition(camPos->getAbsolutePosition());
    }
    cam->setFarValue(1000);

    Renderer->swapMaterials();

    Renderer->enablePostProcessing(true);
    irr::video::CShaderLibrary* shaderLib= Renderer->getShaderLibrary();
    shaderLib->loadShader("antialias", "quad.vert", "postprocess/antialias.frag");
    shaderLib->loadShader("bloom", "quad.vert", "postprocess/bloom.frag");
    shaderLib->loadShader("coldcolors", "quad.vert", "postprocess/coldcolors.frag");
    video::CPostProcessingEffect* antialias= Renderer->addPostProcessingEffect(shaderLib->getShader("antialias"));
    antialias->addTextureToShader(Renderer->getMRT(2));
    //Renderer->addPostProcessingEffect(shaderLib->getShader("coldcolors"));
    //Renderer->addPostProcessingEffect(shaderLib->getShader("bloom"));

    Device->getLogger()->log("Who's that callin?"); //Ain't nobody there
}

CTestFramework::~CTestFramework()
{
    Device->closeDevice();
}

bool CTestFramework::run()
{
    Device->getVideoDriver()->beginScene();
    Device->getSceneManager()->drawAll();
    core::stringw str= L"irrRenderer - RenderTest";
    str+= L" | FPS: ";
    str+= Device->getVideoDriver()->getFPS();
    str+= L" | ms/frame: ";
    str+= (1.0 / Device->getVideoDriver()->getFPS())*1000.0;
    str+= L" | Tris: ";
    str+= Device->getVideoDriver()->getPrimitiveCountDrawn();
    Device->setWindowCaption(str.c_str());

    //draw GBuffer debug info
    /*irr::core::recti gbuffRect= irr::core::recti(irr::core::vector2di(0, 0),Renderer->getMRT(0)->getSize());
    gbuffRect.LowerRightCorner.Y*= -1;
    gbuffRect.UpperLeftCorner.Y*= -1;
    for(irr::u32 i= 0; i < Renderer->getMRTCount(); i++)
    {
        irr::core::recti gbuffRectSmall= irr::core::recti(irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*i, 0), irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*(i+1), Renderer->getMRT(i)->getSize().Height/4.0));
        Device->getVideoDriver()->draw2DImage(Renderer->getMRT(i), gbuffRectSmall, gbuffRect);
    }*/

    Device->getVideoDriver()->endScene();
    return Device->run();
}
