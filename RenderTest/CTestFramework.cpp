#include "CTestFramework.h"

CTestFramework::CTestFramework()
{
    Device= createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600));
    Renderer= createRenderer(Device, false);

    //create some live shit
    scene::ISceneManager* smgr= Device->getSceneManager();
    smgr->loadScene("media/scene.irr");

    //set up basic camera
    scene::ICameraSceneNode* cam= smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    scene::ISceneNode* camPos= smgr->getSceneNodeFromName("camera");
    if(camPos)
    {
        cam->setPosition(camPos->getAbsolutePosition());
        camPos->remove();
    }
    cam->setFarValue(1000);

    //set up walking beast
    scene::ISceneNode* beast= smgr->getSceneNodeFromName("beast");
    if(beast)
    {
        if(beast->getType() == irr::scene::ESNT_ANIMATED_MESH)
        {
            irr::scene::IAnimatedMeshSceneNode* animatedBeast= static_cast<irr::scene::IAnimatedMeshSceneNode*>(beast);
            animatedBeast->setFrameLoop(0,24);
            animatedBeast->setAnimationSpeed(25);
        }
    }

    //set up level
    scene::ISceneNode* level= smgr->getSceneNodeFromName("level");
    if(level && level->getType() == scene::ESNT_MESH)
    {
        scene::IMeshSceneNode* mLevel= static_cast<scene::IMeshSceneNode*>(level);
        scene::IMesh* tangentMesh= smgr->getMeshManipulator()->
                                createMeshWithTangents(mLevel->getMesh());

        mLevel= smgr->addMeshSceneNode(tangentMesh);

        mLevel->setMaterialType(Renderer->getMaterials()->Normal);
        for(u32 i= 0; i < level->getMaterialCount(); i++)
        {
            mLevel->getMaterial(i).setTexture(0, level->getMaterial(i).getTexture(0));
            mLevel->getMaterial(i).setTexture(1, level->getMaterial(i).getTexture(1));
        }

        level->remove();
        tangentMesh->drop();
    }

    //set automatically all materials
    irr::video::CMaterialSwapper* swapper= Renderer->getMaterialSwapper();
    swapper->updateEntry(irr::video::EMT_SOLID_2_LAYER, Renderer->getMaterials()->Solid);
    swapper->swapMaterials();

    //set up post processing
    //Renderer->createPostProcessingEffect(irr::video::EPE_ANTIALIASING);
    //Renderer->createPostProcessingEffect(irr::video::EPE_BLOOM_LQ);
    //Renderer->createPostProcessingEffect(irr::video::EPE_TONE_MAPPING);

    Device->getLogger()->log("Who's that callin?"); //Ain't nobody there
}

CTestFramework::~CTestFramework()
{
    delete Renderer;
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
