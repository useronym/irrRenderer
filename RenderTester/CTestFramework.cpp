#include "CTestFramework.h"

CTestFramework::CTestFramework()
{
    Device= createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600));
    Renderer= createRenderer(Device);

    //create some live shit
    scene::ISceneManager* smgr= Device->getSceneManager();
    smgr->addHillPlaneMesh("hillbilly",
                            core::dimension2d<f32>(10,10),
                            core::dimension2d<u32>(50,50),
                            0,
                            10.0f,
                            core::dimension2d<f32>(5.0f, 2.0f),
                            core::dimension2d<f32>(2,2));
    scene::IMeshSceneNode* ground= smgr->addMeshSceneNode(smgr->getMesh("hillbilly"));
    ground->setMaterialTexture(0, Device->getVideoDriver()->getTexture("DetionMountains.jpg"));
    ground->setMaterialType(Renderer->getMaterials()->Solid);

    scene::IMeshSceneNode* ball= smgr->addSphereSceneNode(20);
    ball->setPosition(core::vector3df(0, 30, 0));
    ball->setMaterialTexture(0, Device->getVideoDriver()->getTexture("brownground.jpg"));
    ball->setMaterialType(Renderer->getMaterials()->Solid);

    for(int i= 1; i < 6; i++)
    {
        scene::ILightSceneNode* light= smgr->addLightSceneNode();
        light->setRadius(100);
        light->addAnimator(smgr->createFlyCircleAnimator(core::vector3df(0.f, 50.f, 0.f), 100.f, 0.001f, irr::core::vector3df(0.f, 1.f, 0.f), i/5.0));
        smgr->addSphereSceneNode(5, 8, light);
    }

    scene::ICameraSceneNode* cam= smgr->addCameraSceneNodeFPS();
    cam->setFarValue(1000);

    Device->getLogger()->log("Who's that callin?"); //Ain't nobody there
}

CTestFramework::~CTestFramework()
{
    Device->getLogger()->log("I've got to go now");
    Device->closeDevice();
    //Get on that bus
}
//Me and the Wanderlust

bool CTestFramework::run()
{
    Device->getVideoDriver()->beginScene();
    Device->getSceneManager()->drawAll();
    core::stringw str= L"irrRenderer - RenderTest";
    str+= L" | FPS: ";
    str+= Device->getVideoDriver()->getFPS();
    str+= L" | Tris: ";
    str+= Device->getVideoDriver()->getPrimitiveCountDrawn();
    Device->setWindowCaption(str.c_str());

    //draw GBuffer debug info
    irr::core::recti gbuffRect= irr::core::recti(irr::core::vector2di(0, 0),Renderer->getMRT(0)->getSize());
    for(irr::u32 i= 0; i < Renderer->getMRTCount(); i++)
    {
        irr::core::recti gbuffRectSmall= irr::core::recti(irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*i, 0), irr::core::vector2di(Renderer->getMRT(i)->getSize().Width/4.0*(i+1), Renderer->getMRT(i)->getSize().Height/4.0));
        Device->getVideoDriver()->draw2DImage(Renderer->getMRT(i), gbuffRectSmall, gbuffRect);
    }

    Device->getVideoDriver()->endScene();
    return Device->run();
}
