#include "CTestFramework.h"

CTestFramework::CTestFramework()
{
    Device= createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600));
    Device->setEventReceiver(this);
    EventOccuredLastFrame= false;

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

    //set up tangent meshes
    core::array<scene::ISceneNode*> nodes;
    Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, nodes);
    for(u32 i= 0; i < nodes.size(); i++)
    {
        scene::IMeshSceneNode* mnode= static_cast<scene::IMeshSceneNode*>(nodes[i]);
        scene::IMesh* tangentMesh= smgr->getMeshManipulator()->
                                createMeshWithTangents(mnode->getMesh());
        mnode= smgr->addMeshSceneNode(tangentMesh);

        mnode->setPosition(nodes[i]->getPosition());
        mnode->setRotation(nodes[i]->getRotation());
        mnode->setScale(nodes[i]->getScale());

        for(u32 ii= 0; ii < nodes[i]->getMaterialCount(); ii++)
        {
            mnode->getMaterial(ii)= nodes[i]->getMaterial(ii);
        }

        mnode->setMaterialType(Renderer->getMaterials()->Parallax);

        nodes[i]->remove();
        tangentMesh->drop();
    }

    //set automatically all materials
    irr::video::CMaterialSwapper* swapper= Renderer->getMaterialSwapper();
    swapper->updateEntry(irr::video::EMT_SOLID_2_LAYER, Renderer->getMaterials()->Solid);
    swapper->swapMaterials();

    //set up post processing
    AA= Renderer->createPostProcessingEffect(irr::video::EPE_ANTIALIASING);
    Bloom= Renderer->createPostProcessingEffect(irr::video::EPE_BLOOM_LQ);
    AA->setActive(false);
    Bloom->setActive(false);

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

bool CTestFramework::OnEvent(const SEvent& event)
{
    scene::ICameraSceneNode* cam= static_cast<scene::ICameraSceneNode*>(Device->getSceneManager()->getSceneNodeFromType(scene::ESNT_CAMERA));
    if(cam)cam->OnEvent(event);

    if(event.EventType == EET_KEY_INPUT_EVENT && !EventOccuredLastFrame)
    {
        if(event.KeyInput.Key == KEY_SPACE)
        {
            core::array<scene::ISceneNode*> meshNodes;
            Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_MESH, meshNodes);
            for(u32 i= 0; i < meshNodes.size(); i++)
            {
                if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Solid) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Normal);
                else if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Normal) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Parallax);
                else if(meshNodes[i]->getMaterial(0).MaterialType == Renderer->getMaterials()->Parallax) meshNodes[i]->setMaterialType(Renderer->getMaterials()->Solid);
            }
        }
        else if(event.KeyInput.Key == KEY_KEY_A)
        {
            AA->setActive(!AA->isActive());
        }
        else if(event.KeyInput.Key == KEY_KEY_B)
        {
            Bloom->setActive(!Bloom->isActive());
        }

        EventOccuredLastFrame= true;
    }
    else
    {
        EventOccuredLastFrame= false;
    }
}
